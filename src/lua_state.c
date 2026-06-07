#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "create_pkg.h"
#include "install_pkg.h"
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
    printf("%scannot run configuration script: %s\n", print_error, lua_tostring(L, -1));
    printf("%sto generate a configration file, head into the root directory of the pkgit source and run `make defconfig`\n", print_pkgit);
    exit(EXIT_FAILURE);
  }
  if (file_exists(repo_file)) {
    if (luaL_loadfile(L, repo_file) || lua_pcall(L, 0, 0, 0)) {
      printf("%scannot load repository file: %s\n", print_error, lua_tostring(L, -1));
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
    if (is_verbose) printf("%scannot run bldit script: %s\n", print_warning,
           lua_tostring(B, -1));
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

void cache_install_directories() {
  if (!config_loaded || !lua_istable(L, -1)) {
    lua_getglobal(L, "install_directories");
  }
  if (!lua_istable(L, -1)) {
    printf("%slua variable 'install_directories' is not a table.\n",
           print_error);
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
      Pkg pkg = create_pkg(dep_url, "default");
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

bool repo_build(const char *repository) {
  lua_getglobal(L, "repositories");
  if (!config_loaded || !lua_istable(L, -1)) {
    if (is_verbose) printf("%slua variable 'repositories' is not a table.\n", print_warning);
    lua_pop(L, 1);
    return false;
  }
  lua_getfield(L, -1, repository);
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf("%s'repositories' lua variable '%s' is not a table.\n", print_warning, repository);
    lua_pop(L, 2);
    return false;
  }

  lua_getfield(L, -1, "dependencies");
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf(
      "%s'repositories' variable 'dependencies' is not a table.\n",
      print_warning
    );
  } else {
    lua_pushnil(L);
    install_dependencies(L);
  }
  lua_pop(L, 1);

  lua_getfield(L, -1, "build");
  if (!lua_isfunction(L, -1)) {
    if (is_verbose) printf(
      "%s'repositories' lua variable 'build' is not a function.\n",
      print_warning
    );
    lua_pop(L, 3);
    return false;
  }
  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    printf("%s'repositories' build failed: %s\n", print_warning, lua_tostring(L, -1));
    lua_pop(L, 3);
    return false;
  }

  lua_pop(L, 2);
  return true;
}

bool repo_install(const char *repository) {
  lua_getglobal(L, "repositories");
  if (!config_loaded || !lua_istable(L, -1)) {
    if (is_verbose) printf("%slua variable 'repositories' is not a table.\n", print_warning);
    lua_pop(L, 1);
    return false;
  }
  lua_getfield(L, -1, repository);
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf("%s'repositories' lua variable '%s' is not a table.\n", print_warning, repository);
    lua_pop(L, 2);
    return false;
  }

  lua_getfield(L, -1, "pre_install");
  if (!lua_isfunction(L, -1)) {
    if (is_verbose) printf("%s'repositories' lua variable 'pre_install' is not a function.\n", print_warning);
  }
  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    if (is_verbose) printf("%s'repositories' pre_install failed: %s\n", print_warning, lua_tostring(L, -1));
  }

  lua_getfield(L, -1, "install");
  if (!lua_isfunction(L, -1)) {
    if (is_verbose) printf("%s'repositories' lua variable 'install' is not a function.\n", print_warning);
    return false;
  }
  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    if (is_verbose) printf("%s'repositories' install failed: %s\n", print_warning, lua_tostring(L, -1));
  }
  lua_pop(L, 1);

  lua_getfield(L, -1, "post_install");
  if (!lua_isfunction(L, -1)) {
    if (is_verbose) printf("%s'repositories' lua variable 'post_install' is not a function.\n", print_warning);
  }
  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    if (is_verbose) printf("%s'repositories' post_install failed: %s\n", print_warning, lua_tostring(L, -1));
  }

  lua_pop(L, 2);
  return true;
}

bool repo_uninstall(const char *repository) {
  lua_getglobal(L, "repositories");
  if (!config_loaded || !lua_istable(L, -1)) {
    if (is_verbose) printf("%slua variable 'repositories' is not a table.\n", print_warning);
    lua_pop(L, 1);
    return false;
  }
  lua_getfield(L, -1, repository);
  if (!lua_istable(L, -1)) {
    if (is_verbose) printf("%s'repositories' lua variable '%s' is not a table.\n", print_warning, repository);
    lua_pop(L, 2);
    return false;
  }

  lua_getfield(L, -1, "uninstall");
  if (!lua_isfunction(L, -1)) {
    if (is_verbose) printf("%s'repositories' lua variable 'uninstall' is not a function.\n", print_warning);
    return false;
  }
  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    if (is_verbose) printf("%s'repositories' uninstall failed: %s\n", print_warning, lua_tostring(L, -1));
    return false;
  }
  lua_pop(L, 1);

  lua_pop(L, 2);
  return true;
}

bool bldit(const char *target) {
  lua_State *B = luaL_newstate();
  luaL_openlibs(B);

  if (luaL_loadfile(B, "bldit.lua") || lua_pcall(B, 0, 0, 0)) {
    if (is_verbose) printf("%scannot run bldit script: %s\n", print_warning, lua_tostring(B, -1));
    lua_close(B);
    return false;
  }

  lua_getglobal(B, "bldit_version");
  if (!lua_isstring(B, -1)) {
    if (is_verbose) printf("%sbldit variable 'bldit_version' is not a string.\n", print_warning);
  }

  lua_getglobal(B, "package_version");
  if (!lua_isstring(B, -1)) {
    if (is_verbose) printf("%sbldit variable 'package_version' is not a string.\n", print_warning);
  }

  lua_getglobal(B, "global_dependencies");
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf("%sbldit variable 'global_dependencies' is not a table.\n", print_warning);
  } else {
    lua_pushnil(B);
    install_dependencies(B);
  }
  lua_pop(B, 1);

  lua_getglobal(B, "targets");
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf("%sbldit variable 'targets' is not a table.\n", print_warning);
    lua_close(B);
    return false;
  }

  lua_getfield(B, -1, target);
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf("%sbldit variable 'targets.%s' is not a table.\n", print_warning, target);
    lua_pop(B, 2);
    lua_close(B);
    return false;
  }

  lua_getfield(B, -1, "dependencies");
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf("%sbldit variable 'targets.%s.dependencies' is not a table.\n", target, print_warning);
  } else {
    lua_pushnil(B);
    install_dependencies(B);
  }
  lua_pop(B, 1);

  lua_getfield(B, -1, "build");
  if (!lua_isfunction(B, -1)) {
    if (is_verbose) printf("%s'repositories' lua variable 'build' is not a function.\n",
           print_warning);
    lua_pop(B, 3);
    lua_close(B);
    return false;
  }
  if (lua_pcall(B, 0, 0, 0) != LUA_OK) {
    printf("%sbuild failed: %s\n", print_error, lua_tostring(B, -1));
    lua_pop(B, 1);
    lua_pop(B, 2);
    lua_close(B);
    return false;
  }

  lua_pop(B, 2);
  lua_close(B);
  return true;
}

bool bldit_install(const char *target) {
  lua_State *B = luaL_newstate();
  luaL_openlibs(B);

  if (luaL_loadfile(B, "bldit.lua") || lua_pcall(B, 0, 0, 0)) {
    if (is_verbose) printf("%scannot run bldit script: %s\n", print_warning, lua_tostring(B, -1));
    lua_close(B);
    return false;
  }

  lua_getglobal(B, "targets");
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf("%sbldit variable 'targets' is not a table.\n", print_warning);
    lua_pop(B, 1);
    lua_close(B);
    return false;
  }

  lua_getfield(B, -1, target);
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf("%sbldit variable '%s' is not a table.\n", print_warning, target);
    lua_pop(B, 2);
    lua_close(B);
    return false;
  }

  lua_getfield(B, -1, "pre_install");
  if (!lua_isfunction(B, -1)) {
    if (is_verbose) printf("%s'repositories' lua variable 'pre_install' is not a function.\n",
           print_warning);
  }
  if (lua_pcall(B, 0, 0, 0) != LUA_OK) {
    if (is_verbose) printf("%spre_install failed: %s\n", print_warning, lua_tostring(B, -1));
  }

  lua_getfield(B, -1, "install");
  if (!lua_isfunction(B, -1)) {
    if (is_verbose) printf("%s'repositories' lua variable 'install' is not a function.\n",
           print_warning);
  } else {
    return false;
  }
  if (lua_pcall(B, 0, 0, 0) != LUA_OK) {
    if (is_verbose) printf("%sinstall failed: %s\n", print_warning, lua_tostring(B, -1));
  }

  lua_getfield(B, -1, "post_install");
  if (!lua_isfunction(B, -1)) {
    if (is_verbose) printf("%s'repositories' lua variable 'post_install' is not a function.\n",
           print_warning);
  }
  if (lua_pcall(B, 0, 0, 0) != LUA_OK) {
    if (is_verbose) printf("%spost_install failed: %s\n", print_warning, lua_tostring(B, -1));
  }

  lua_pop(B, 2);
  lua_close(B);
  return true;
}

bool bldit_uninstall(const char *target) {
  lua_State *B = luaL_newstate();
  luaL_openlibs(B);

  if (luaL_loadfile(B, "bldit.lua") || lua_pcall(B, 0, 0, 0)) {
    if (is_verbose) printf("%scannot run bldit script: %s\n", print_warning, lua_tostring(B, -1));
    lua_close(B);
    return false;
  }

  lua_getglobal(B, "targets");
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf("%sbldit variable 'targets' is not a table.\n", print_warning);
    lua_close(B);
    return false;
  }

  lua_getfield(B, -1, target);
  if (!lua_istable(B, -1)) {
    if (is_verbose) printf("%sbldit variable '%s' is not a table.\n", print_warning, target);
    lua_pop(B, 2);
    lua_close(B);
    return false;
  }

  lua_getfield(B, -1, "uninstall");
  if (!lua_isfunction(B, -1)) {
    if (is_verbose) printf("%s'repositories' lua variable 'uninstall' is not a function.\n",
           print_warning);
    return false;
  }
  if (lua_pcall(B, 0, 0, 0) != LUA_OK) {
    if (is_verbose) printf("%suninstall failed: %s\n", print_warning, lua_tostring(B, -1));
    return false;
  }

  lua_pop(B, 2);
  lua_close(B);
  return true;
}

void cache_repos() {
  cached_repos_count = 0;
  lua_getglobal(L, "repositories");
  if (!config_loaded || !lua_istable(L, -1)) {
    printf("%slua variable 'repositories' is not a table.\n", print_error);
    lua_pop(L, 1);
    return;
  }
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    const char *repo_name = lua_tostring(L, -2);
    if (!repo_name || !lua_istable(L, -1)) {
      printf("%srepository key is not a table\n", print_error);
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
    printf("%slua variable 'build_systems' is not a table.\n", print_error);
    return;
  }

  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    const char *key = lua_tostring(L, -2);
    if (lua_isfunction(L, -1) == 0) {
      printf("%sbuild value is not a function\n", print_error);
      lua_pop(L, 1);
      continue;
    }

    map_put(&cached_build_systems, strdup(key), "");
    lua_pop(L, 1);
  }
  lua_pop(L, 1);
}

bool config_build(const char *path) {
  lua_getglobal(L, "build_systems");
  if (!config_loaded || !lua_istable(L, -1)) {
    lua_pop(L, 1);
    return false;
  }
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
    lua_getfield(L, -1, "build");
    if (!lua_isfunction(L, -1)) {
      lua_pop(L, 1);
      continue;
    }
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
      lua_pop(L, 1);
      continue;
    }
    lua_pop(L, 1);
    lua_pop(L, 1);
    return true;
  }
  lua_pop(L, 1);
  return false;
}

bool config_install(const char *path) {
  lua_getglobal(L, "build_systems");
  if (!config_loaded || !lua_istable(L, -1)) {
    lua_pop(L, 1);
    return false;
  }
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
    lua_getfield(L, -1, "pre_install");
    if (lua_isfunction(L, -1)) {
      if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        if (is_verbose) printf("%slua function 'pre_install' is not available\n",
          print_warning);
      }
    } else { lua_pop(L, 1); }

    lua_getfield(L, -1, "install");
    if (lua_isfunction(L, -1)) {
      if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        if (is_verbose) printf("%s lua function 'install' is not available\n",
          print_warning);
      }
    } else { lua_pop(L, 1); }
    
    lua_getfield(L, -1, "post_install");
    if (!lua_isfunction(L, -1)) {
      if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        if (is_verbose) printf("%slua function 'post_install' is not available\n",
          print_warning);
      }
    } else { lua_pop(L, 1); }

    lua_pop(L, 1);
    lua_pop(L, 1);
    return true;
  }
  lua_pop(L, 1);
  return false;
}

bool config_uninstall(const char *path) {
  lua_getglobal(L, "build_systems");
  if (!config_loaded || !lua_istable(L, -1)) {
    lua_pop(L, 1);
    return false;
  }
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

    lua_getfield(L, -1, "uninstall");
    if (!lua_isfunction(L, -1)) {
      lua_pop(L, 1);
      continue;
    }
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
      lua_pop(L, 1);
      continue;
    }

    lua_pop(L, 1);
    lua_pop(L, 1);
    return true;
  }
  lua_pop(L, 1);
  return false;
}
