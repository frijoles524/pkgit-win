#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "create_pkg.h"
#include "install_pkg.h"
#include "is_updated.h"
#include "lua_state.h"
#include "vars.h"

static lua_State *L = NULL;
static bool config_loaded = false;

static lua_State *B = NULL;
static bool bldit_loaded = false;

void push_lua_path(lua_State *L, const char *new_path) {
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "path");
  const char *current_path = lua_tostring(L, -1);
  if (!current_path)
    current_path = "";
  lua_pop(L, 1);
  lua_pushfstring(L, "%s;%s", current_path, new_path);
  lua_setfield(L, -2, "path");
  lua_pop(L, 1);
}

void init_lua_state() {
  if (L != NULL)
    return;
  L = luaL_newstate();
  luaL_openlibs(L);
  char lua_path[MAX_PATH_LEN + 20];
  snprintf(lua_path, sizeof(lua_path), "%s/?.lua", config_dir);
  push_lua_path(L, lua_path);
  if (luaL_loadfile(L, config_file) || lua_pcall(L, 0, 0, 0)) {
    printf(
      "%s cannot run configuration script: %s\n",
      print_error, lua_tostring(L, -1)
    );
    printf("%s to generate a configuration file, head into the", print_pkgit);
    printf(" root directory of the pkgit source and run `make defconfig`\n");
    exit(EXIT_FAILURE);
  }
  if (file_exists(repo_file)) {
    if (luaL_loadfile(L, repo_file) || lua_pcall(L, 0, 0, 0)) {
      printf(
        "%s cannot load repository file: %s\n",
        print_error, lua_tostring(L, -1)
      );
      lua_pop(L, 1);
    }
  }
  config_loaded = true;
}

void init_bldit() {
  if (B != NULL)
    return;
  B = luaL_newstate();
  luaL_openlibs(B);
  if (luaL_loadfile(B, "bldit.lua") || lua_pcall(B, 0, 0, 0)) {
    if (is_verbose) printf(
      "%s cannot run bldit script: %s\n",
      print_warning, lua_tostring(B, -1)
    );
    return;
  }
  bldit_loaded = true;
}

void free_lua_state() {
  if (L != NULL) {
    lua_close(L);
    L = NULL;
  }
  config_loaded = false;
}

lua_State *get_lua_state() { return L; }

void cache_prefix_directory() {
  lua_getglobal(L, "prefix");
  if (!lua_isstring(L, -1)) {
    printf(
      "%s init.lua: 'prefix' is not a string.\n",
      print_error
    );
    return;
  }
  snprintf(prefix_dir, MAX_PATH_LEN, "%s", lua_tostring(L, -1));
  lua_pop(L, 1);
}

void cache_install_directories() {
  if (!config_loaded || !lua_istable(L, -1)) {
    lua_getglobal(L, "install_directories");
  }
  if (!lua_istable(L, -1)) {
    printf(
      "%s init.lua: 'install_directories' is not a table.\n",
      print_error
    );
    return;
  }
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    const char *key = lua_tostring(L, -2);
    const char *value = lua_tostring(L, -1);
    if (key && value) {
      map_put(&cached_install_directories, strdup(key), strdup(value));
    }
    lua_pop(L, 1);
  }
  lua_pop(L, 1);
}

void install_dependencies(lua_State *L) {
  while (lua_next(L, -2) != 0) {
    const char *depname = lua_tostring(L, -2);
    if (depname && lua_istable(L, -1)) {
      lua_getfield(L, -1, "url");
      const char *dep_url = lua_tostring(L, -1);
      lua_pop(L, 1);
      lua_getfield(L, -1, "version");
      const char *dep_version = lua_tostring(L, -1);
      lua_pop(L, 1);
      Pkg pkg = create_pkg(dep_url);
      pkg.ver = strdup(dep_version);
      const int top = lua_gettop(L);
      char cwd[PATH_MAX];
      if (getcwd(cwd, sizeof(cwd)) != NULL) {
        install_pkg(pkg);
        chdir(cwd);
      }
      lua_settop(L, top);
    }
    lua_pop(L, 1);
  }
}

bool target_loop_build(
  lua_State *L, const char* lua_file, const char *target
) {
  lua_getfield(L, -1, target);
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s %s: 'targets.%s' is not a table.\n",
      print_warning, lua_file, target
    );
    lua_pop(L, 1);
    return false;
  }
  lua_getfield(L, -1, "dependencies");
  if (lua_istable(L, -1)) {
    lua_pushnil(L);
    install_dependencies(L);
  }
  lua_pop(L, 1);
  lua_getfield(L, -1, "build");
  if (!lua_isfunction(L, -1)) {
    if (is_verbose) printf(
      "%s %s: 'targets.%s.build' is not a function.\n",
      print_warning, lua_file, target
    );
    lua_pop(L, 3);
    return false;
  }
  if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
    printf(
      "%s %s: 'targets.%s.build' function borked: %s\n",
      print_error, lua_file, target, lua_tostring(L, -1)
    );
    lua_pop(L, 3);
    return false;
  }
  if (!lua_isnumber(L, -1) || lua_tonumber(L, -1) != 0) if (is_verbose) {
    printf(
      "%s %s: 'targets.%s.build' failed: %s\n",
      print_error, lua_file, target, lua_tostring(L, -1)
    );
    lua_pop(L, 3);
    return false;
  }
  lua_pop(L, 3);
  return true;
}

bool target_loop_install(lua_State *L, const char* lua_file, const char *target) {
  lua_getfield(L, -1, target);
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s %s: 'targets.%s' is not a table.\n",
      print_warning, lua_file, target
    );
    lua_pop(L, 1);
    return false;
  }
  lua_getfield(L, -1, "pre_install");
  if (!lua_isfunction(L, -1)) {
    if (is_verbose) printf(
      "%s %s: 'targets.%s.pre_install' is not a function.\n",
      print_warning, lua_file, target
    );
  } else if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
    if (is_verbose) printf(
      "%s %s: 'targets.%s.pre_install' function borked: %s\n",
      print_warning, lua_file, target, lua_tostring(L, -1)
    );
    if (!lua_isnumber(L, -1) || lua_tonumber(L, -1) != 0) if (is_verbose) {
      printf(
        "%s %s: 'targets.%s.pre_install' failed: %s\n",
        print_error, lua_file, target, lua_tostring(L, -1)
      );
      return false;
    }
  }
  lua_pop(L, 1);
  lua_getfield(L, -1, "install");
  if (!lua_isfunction(L, -1)) {
    if (is_verbose) printf(
      "%s %s: 'targets.%s.install' is not a function.\n",
      print_warning, lua_file, target
    );
    lua_pop(L, 1);
    return false;
  }
  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    if (is_verbose) printf(
      "%s %s: 'targets.%s.install' function borked: %s\n",
      print_warning, lua_file, target, lua_tostring(L, -1)
    );
    lua_pop(L, 1);
    return false;
  }
  if (!lua_isnumber(L, -1) || lua_tonumber(L, -1) != 0) if (is_verbose) {
    printf(
      "%s %s: 'targets.%s.install' failed: %s\n",
      print_error, lua_file, target, lua_tostring(L, -1)
    );
    lua_pop(L, 1);
    return false;
  }
  lua_pop(L, 1);
  lua_getfield(L, -1, "post_install");
  if (!lua_isfunction(L, -1)) {
    if (is_verbose) printf(
      "%s %s: 'targets.%s.post_install' is not a function.\n",
      print_warning, lua_file, target
    );
  } else if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    if (is_verbose) printf(
      "%s %s: 'targets.%s.post_install' function borked: %s\n",
      print_warning, lua_file, target, lua_tostring(L, -1)
    );
    if (!lua_isnumber(L, -1) || lua_tonumber(L, -1) != 0) if (is_verbose) {
      printf(
        "%s %s: 'targets.%s.post_install' failed: %s\n",
        print_error, lua_file, target, lua_tostring(L, -1)
      );
    }
  }
  lua_pop(L, 1);
  return true;
}

bool target_loop_uninstall(lua_State *L, const char *lua_file, const char *target) {
  lua_getfield(L, -1, target);
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s %s: 'targets.%s' is not a table.\n",
      print_warning, lua_file, target
    );
    lua_pop(L, 1);
    return false;
  }
  lua_getfield(L, -1, "uninstall");
  if (!lua_isfunction(L, -1)) {
    if (is_verbose) printf(
      "%s %s: 'targets.%s.uninstall' is not a function.\n",
      print_warning, lua_file, target
    );
    lua_pop(L, 1);
    return false;
  }
  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    if (is_verbose) printf(
      "%s %s: 'targets.%s.uninstall' function borked: %s\n",
      print_warning, lua_file, target, lua_tostring(L, -1)
    );
    lua_pop(L, 1);
    return false;
  }
  if (!lua_isnumber(L, -1) || lua_tonumber(L, -1) != 0) if (is_verbose) {
    printf(
      "%s %s: 'targets.%s.uninstall' failed: %s\n",
      print_error, lua_file, target, lua_tostring(L, -1)
    );
    lua_pop(L, 1);
    return false;
  }
  lua_pop(L, 1);
  return true;
}

bool repo_build(const char *repository, const char *target) {
  lua_getglobal(L, "repositories");
  if (!config_loaded || !lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s init.lua: 'repositories' is not a table.\n",
      print_warning
    );
    lua_pop(L, 1);
    return false;
  }
  lua_getfield(L, -1, repository);
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s init.lua: 'repositories.%s' is not a table.\n",
      print_warning, repository
    );
    lua_pop(L, 2);
    return false;
  }

  lua_getfield(L, -1, "dependencies");
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s init.lua: 'repositories.%s.dependencies' is not a table.\n",
      print_warning, repository
    );
  } else {
    lua_pushnil(L);
    install_dependencies(L);
  }
  lua_pop(L, 1);

  lua_getfield(L, -1, "targets");
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s init.lua: 'repositories.%s.targets' is not a table.\n",
      print_warning, repository
    );
    return false;
  }
  bool target_loop_success = target_loop_build(L, "init.lua", target);
  lua_pop(L, 3);
  return target_loop_success;
}

bool repo_install(const char *repository, const char* target) {
  lua_getglobal(L, "repositories");
  if (!config_loaded || !lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s init.lua: 'repositories' is not a table.\n",
      print_warning
    );
    lua_pop(L, 1);
    return false;
  }
  lua_getfield(L, -1, repository);
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s init.lua: 'repositories.%s' is not a table.\n",
      print_warning, repository
    );
    lua_pop(L, 2);
    return false;
  }

  lua_getfield(L, -1, "targets");
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s init.lua: 'repositories.%s.targets' is not a table.\n",
      print_warning, repository
    );
    return false;
  }
  bool target_loop_success = target_loop_install(L, "init.lua", target);
  lua_pop(L, 3);
  return target_loop_success;
}

bool repo_uninstall(const char *repository, const char *target) {
  lua_getglobal(L, "repositories");
  if (!config_loaded || !lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s init.lua: 'repositories' is not a table.\n",
      print_warning
    );
    lua_pop(L, 1);
    return false;
  }
  lua_getfield(L, -1, repository);
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s init.lua: 'repositories.%s' is not a table.\n",
      print_warning, repository
    );
    lua_pop(L, 2);
    return false;
  }

  lua_getfield(L, -1, "targets");
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s init.lua: 'repositories.%s.targets' is not a table.\n",
      print_warning, repository
    );
    return false;
  }
  bool target_loop_success = target_loop_uninstall(L, "init.lua", target);
  lua_pop(L, 2);
  return target_loop_success;
}

char* bldit_getver() {
  init_bldit();
  lua_getglobal(B, "bldit_version");
  if (!lua_isstring(B, -1)) {
    if (is_verbose)
      printf("%s bldit.lua: 'bldit_version' is not a string.\n", print_warning);
    return NULL;
  }
  const char* bldit_version = lua_tostring(B, -1);
  return strdup(bldit_version);
}

char* bldit_pkg_getver() {
  init_bldit();
  lua_getglobal(B, "package_version");
  if (!lua_isstring(B, -1)) {
    if (is_verbose)
      printf("%s bldit.lua: 'package_version' is not a string.\n", print_warning);
    return NULL;
  }
  const char* package_version = lua_tostring(B, -1);
  return strdup(package_version);
}

bool bldit(const char *target) {
  lua_State *B = luaL_newstate();
  luaL_openlibs(B);

  if (luaL_loadfile(B, "bldit.lua") || lua_pcall(B, 0, 0, 0)) {
    if (is_verbose) printf(
      "%s cannot run bldit script: %s\n",
      print_warning, lua_tostring(B, -1)
    );
    lua_close(B);
    return false;
  }

  if (!is_bldit_usable()) {
    printf(
      "%s bldit version is newer than the installed pkgit version\n",
      print_error
    );
    printf(
      "%s consider updating pkgit\n",
      print_pkgit
    );
    if (!is_forced) return false;
  }

  lua_pushfstring(B, "%s", prefix_dir);
  lua_setglobal(B, "prefix");
  lua_pop(B, 1);

  lua_getglobal(B, "dependencies");
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf("%s bldit.lua: 'dependencies' is not a table.\n", print_warning);
  } else {
    lua_pushnil(B);
    install_dependencies(B);
  }
  lua_pop(B, 1);

  lua_getglobal(B, "targets");
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf("%s bldit.lua: 'targets' is not a table.\n", print_warning);
    lua_close(B);
    return false;
  }

  bool target_loop_success = target_loop_build(B, "bldit.lua", target);
  lua_pop(B, 2);
  lua_close(B);
  return target_loop_success;
}

bool bldit_install(const char *target) {
  lua_State *B = luaL_newstate();
  luaL_openlibs(B);

  if (luaL_loadfile(B, "bldit.lua") || lua_pcall(B, 0, 0, 0)) {
    if (is_verbose) printf(
      "%s cannot run bldit script: %s\n",
      print_warning, lua_tostring(B, -1)
    );
    lua_close(B);
    return false;
  }

  if (!is_bldit_usable()) {
    printf(
      "%s bldit version is newer than the installed pkgit version\n",
      print_error
    );
    printf(
      "%s consider updating pkgit\n",
      print_pkgit
    );
    if (!is_forced) return false;
  }

  lua_pushfstring(B, "%s", prefix_dir);
  lua_setglobal(B, "prefix");
  lua_pop(B, 1);

  lua_getglobal(B, "targets");
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf(
      "%s bldit.lua: 'targets' is not a table.\n",
      print_warning
    );
    lua_pop(B, 1);
    lua_close(B);
    return false;
  }

  bool target_loop_success = target_loop_install(B, "bldit.lua", target);
  lua_pop(B, 1);
  lua_close(B);
  return target_loop_success;
}

bool bldit_uninstall(const char *target) {
  lua_State *B = luaL_newstate();
  luaL_openlibs(B);

  if (luaL_loadfile(B, "bldit.lua") || lua_pcall(B, 0, 0, 0)) {
    if (is_verbose) printf(
      "%s cannot run bldit script: %s\n",
      print_warning, lua_tostring(B, -1)
    );
    lua_close(B);
    return false;
  }

  if (!is_bldit_usable()) {
    printf(
      "%s bldit version is newer than the installed pkgit version\n",
      print_error
    );
    printf(
      "%s consider updating pkgit\n",
      print_pkgit
    );
    if (!is_forced) return false;
  }

  lua_pushfstring(B, "%s", prefix_dir);
  lua_setglobal(B, "prefix");
  lua_pop(B, 1);

  lua_getglobal(B, "targets");
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf(
      "%s bldit.lua: 'targets' is not a table.\n",
      print_warning
    );
    lua_close(B);
    return false;
  }

  lua_getfield(B, -1, target);
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf(
      "%s bldit.lua: 'targets.%s' is not a table.\n",
      print_warning, target
    );
    lua_pop(B, 2);
    lua_close(B);
    return false;
  }

  lua_getglobal(B, "targets");
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf("%s bldit.lua: 'targets' is not a table.\n", print_warning);
    lua_close(B);
    return false;
  }
  bool target_loop_success = target_loop_uninstall(B, "bldit.lua", target);
  lua_pop(B, 1);
  lua_close(B);
  return target_loop_success;
}

void cache_repos() {
  cached_repos_count = 0;
  lua_getglobal(L, "repositories");
  if (!config_loaded || !lua_istable(L, -1)) {
    printf(
      "%s init.lua: 'repositories' is not a table.\n",
      print_error
    );
    lua_pop(L, 1);
    return;
  }
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    const char *repo_name = lua_tostring(L, -2);
    if (!repo_name || !lua_istable(L, -1)) {
      printf("%s init.lua: 'repositories.%s' is not a table\n", print_error, repo_name);
      lua_pop(L, 1);
      continue;
    }
    Repo *repo = &cached_repos[cached_repos_count];
    repo->source_key = strdup(repo_name);
    lua_getfield(L, -1, "url");
    const char *url = lua_tostring(L, -1);
    repo->source_value = url ? strdup(url) : strdup("");
    lua_pop(L, 1);
    lua_getfield(L, -1, "version");
    const char *version = lua_tostring(L, -1);
    repo->version = version ? strdup(version) : strdup("HEAD");
    lua_pop(L, 1);
    lua_pop(L, 1);
    cached_repos_count++;
  }
  lua_pop(L, 1);
}

void cache_build_systems() {
  if (!config_loaded) {
    lua_getglobal(L, "build_systems");
  } else if (!lua_istable(L, -1)) {
    lua_getglobal(L, "build_systems");
  }

  if (!lua_istable(L, -1)) {
    printf("%s init.lua: 'build_systems' is not a table.\n", print_error);
    return;
  }

  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    const char *key = lua_tostring(L, -2);
    if (lua_isfunction(L, -1) == 0) {
      printf("%s init.lua: 'build_systems.%s' is not a function\n", print_error, key);
      lua_pop(L, 1);
      continue;
    }

    map_put(&cached_build_systems, strdup(key), "");
    lua_pop(L, 1);
  }
  lua_pop(L, 1);
}

bool config_build(const char *path, const char *target) {
  lua_getglobal(L, "build_systems");
  if (!config_loaded || !lua_istable(L, -1)) {
    lua_pop(L, 1);
    return false;
  }
  bool target_loop_success = false;
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    const char *key = lua_tostring(L, -2);
    if (!lua_istable(L, -1)) {
      lua_pop(L, 1);
      continue;
    }
    char file_path[MAX_PATH_LEN];
    snprintf(file_path, sizeof(file_path), "%s/%s", path, key);
    if (access(file_path, F_OK) != 0) {
      lua_pop(L, 1);
      continue;
    }
    lua_getfield(L, -1, "targets");
    if (!lua_istable(L, -1)) {
      if (is_verbose) printf("%s init.lua: 'targets' is not a table.\n", print_warning);
      lua_pop(L, 1);
      continue;
    }
    target_loop_success = target_loop_build(L, "init.lua", target);
    lua_pop(L, 1);
  }
  lua_pop(L, 1);
  return target_loop_success;
}

bool config_install(const char *path, const char *target) {
  lua_getglobal(L, "build_systems");
  if (!config_loaded || !lua_istable(L, -1)) {
    lua_pop(L, 1);
    return false;
  }
  lua_pushnil(L);
  bool target_loop_success = false;
  while (lua_next(L, -2) != 0) {
    const char *key = lua_tostring(L, -2);
    if (!lua_istable(L, -1)) {
      lua_pop(L, 1);
      continue;
    }
    char file_path[MAX_PATH_LEN];
    snprintf(file_path, sizeof(file_path), "%s/%s", path, key);
    if (access(file_path, F_OK) != 0) {
      lua_pop(L, 1);
      continue;
    }

    lua_getfield(L, -1, "targets");
    if (!lua_istable(L, -1)) {
      if (is_verbose) printf(
        "%s init.lua: 'build_systems.%s.targets' is not a table.\n",
        print_warning, key
      );
      lua_close(L);
      return false;
    }
    target_loop_success = target_loop_install(L, "init.lua", target);
    lua_pop(L, 2);
  }
  lua_pop(L, 1);
  return target_loop_success;
}

bool config_uninstall(const char *path, const char *target) {
  lua_getglobal(L, "build_systems");
  if (!config_loaded || !lua_istable(L, -1)) {
    lua_pop(L, 1);
    return false;
  }
  bool target_loop_success = false;
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    const char *key = lua_tostring(L, -2);
    if (!lua_istable(L, -1)) {
      lua_pop(L, 1);
      continue;
    }
    char file_path[MAX_PATH_LEN];
    snprintf(file_path, sizeof(file_path), "%s/%s", path, key);
    if (access(file_path, F_OK) != 0) {
      lua_pop(L, 1);
      continue;
    }

    lua_getfield(L, -1, "targets");
    if (!lua_istable(L, -1)) {
      if (is_verbose) printf(
        "%s init.lua: 'build_systems.%s.targets' is not a table.\n",
        print_warning, key
      );
      lua_close(L);
      return false;
    }
    target_loop_success = target_loop_uninstall(L, "init.lua", target);
    lua_pop(L, 2);
  }
  lua_pop(L, 1);
  return target_loop_success;
}
