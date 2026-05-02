#include "lua_state.hh"
#include "vars.hh"
#include <iostream>
#include <unordered_map>

static lua_State *L = nullptr;
static bool config_loaded = false;

static lua_State *B = nullptr;
static bool bldit_loaded = false;

std::unordered_map<std::string, std::string> cached_install_directories;
std::unordered_map<std::string, repo> cached_repos;
std::unordered_map<std::string, int> cached_build_systems;

void init_lua_state() {
  if (L != nullptr)
    return;
  L = luaL_newstate();
  luaL_openlibs(L);
  if (luaL_loadfile(L, config_file.c_str()) || lua_pcall(L, 0, 0, 0)) {
    std::cout << print_error << "cannot run configuration script: " << lua_tostring(L, -1) << "\n";
    return;
  }
  //if (luaL_loadfile(L, repo_file.c_str()) || lua_pcall(L, 0, 0, 0)) {
  //  std::cout << print_error << "cannot run repository script: " << lua_tostring(L, -1) << "\n";
  //  return;
  //}
  config_loaded = true;
}

void init_bldit() {
  if (B != nullptr)
    return;
  B = luaL_newstate();
  luaL_openlibs(B);
  if (luaL_loadfile(B, "bldit.lua") || lua_pcall(B, 0, 0, 0)) {
    std::cout << print_warning << "cannot run bldit script: " << lua_tostring(B, -1) << "\n";
    return;
  }
  bldit_loaded = true;
}

void free_lua_state() {
  if (L != nullptr) {
    lua_close(L);
    L = nullptr;
  }
  config_loaded = false;
}

lua_State *get_lua_state() { return L; }

void cache_install_directories() {
  if (!config_loaded || !lua_istable(L, -1)) {
    lua_getglobal(L, "install_directories");
  }
  if (!lua_istable(L, -1)) {
    std::cout << print_error
	      << "lua variable 'install_directories' is not a table.\n";
    return;
  }
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    const char *key = lua_tostring(L, -2);
    const char *value = lua_tostring(L, -1);
    cached_install_directories[key] = value;
    lua_pop(L, 1);
  }
  lua_pop(L, 1);
}

bool repo_build(const char *repository) {
  lua_getglobal(L, "repositories");
  if (!config_loaded || !lua_istable(L, -1)) {
    std::cout << print_warning
	      << "lua variable 'repositories' is not a table.\n";
    lua_pop(L, 1);
    return false;
  }
  std::cout << print_pkgit
	    << "lua variable 'repositories' used successfully.\n";
  lua_getfield(L, -1, repository);
  if (!lua_istable(L, -1)) {
    std::cout << print_warning << "'repositories' lua variable '" << repository << "' is not a table.\n";
    lua_pop(L, 2);
    return false;
  }
  std::cout << print_pkgit << "'repositories' lua variable '" << repository << "' used successfully.\n";
  lua_getfield(L, -1, "build");
  if (!lua_isfunction(L, -1)) {
    std::cout << print_warning << "'repositories' lua variable 'build' is not a function." << std::endl;
    lua_pop(L, 3);
    return false;
  }
  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    std::cout << "'repositories' build failed: " << lua_tostring(L, -1) << std::endl;
    lua_pop(L, 1);
    lua_pop(L, 2);
    return false;
  }
  lua_pop(L, 2);
  std::cout << print_pkgit
	    << "'repositories' lua function 'build' ran successfully.\n";
  return true;
}

bool bldit(const char *target) {
  lua_State* B = luaL_newstate();
  luaL_openlibs(B);
  if (luaL_loadfile(B, "bldit.lua") || lua_pcall(B, 0, 0, 0)) {
    std::cout << print_warning << "cannot run bldit script: " << lua_tostring(B, -1) << "\n";
    return false;
  }
  lua_getglobal(B, "targets");
  if (!lua_istable(B, -1)) {
    std::cout << print_warning << "bldit variable 'targets' is not a table.\n";
    return false;
  }
  std::cout << print_pkgit << "bldit variable 'targets' used successfully.\n";
  lua_getfield(B, -1, target);
  if (!lua_istable(B, -1)) {
    std::cout << print_warning << "bldit variable '" << target << "' is not a table.\n";
    lua_pop(B, 2);
    return false;
  }
  std::cout << print_pkgit << "bldit variable '" << target << "' used successfully.\n";
  lua_getfield(B, -1, "build");
  if (!lua_isfunction(B, -1)) {
    std::cout << "'repositories' lua variable 'build' is not a function." << std::endl;
    lua_pop(B, 3);
    return false;
  }
  if (lua_pcall(B, 0, 0, 0) != LUA_OK) {
    std::cout << "build failed: " << lua_tostring(B, -1) << std::endl;
    lua_pop(B, 1);
    lua_pop(B, 2);
    return false;
  }
  lua_pop(B, 2);
  std::cout << print_pkgit << "bldit function 'build' ran successfully.\n";
  return true;
}

//bool config_build(const char* path) {
//  free_lua_state();
//  init_lua_state();
//  lua_State* L = get_lua_state();
//  std::unordered_map<std::string, int> build_files;
//  lua_getglobal(L, "build_systems");
//  if (!config_loaded || !lua_istable(L, -1)) {
//    lua_pop(L, 1);
//    std::cout << print_warning << "config lua variable 'build_systems' is not
//    a table.\n"; return false;
//  }
//  lua_pushnil(L);
//  bool build_found = false;
//  while (lua_next(L, -2) != 0) {
//    const char *key = lua_tostring(L, -2);
//    int value = lua_type(L, -1);
//    if (lua_isfunction(L, -1) == 0) {
//      std::cout << print_warning << "config build value " << key << " is not
//      a function\n"; lua_pop(L, 1); continue;
//    }
//    build_files[key] = value;
//    for (auto const& dir_entry : std::filesystem::directory_iterator(std::filesystem::current_path().string())) {
//      std::string string_key = key;
//      if (dir_entry.path().filename() != string_key) { continue; }
//      build_found = true;
//      lua_pushvalue(L, -1);
//      lua_pushstring(L, std::filesystem::current_path().string().c_str());
//      std::cout << "calling config lua build function according to key filename '"
//      << key << "'...\n";
//      if (lua_pcall(L, 1, 0, 0) != 0) {
//        std::cout << print_warning << "config lua build function failed to run\n";
//        break;
//      }
//      if (build_found) { break; }
//    }
//    lua_pop(L, 1);
//  }
//  return build_found;
//}

void cache_repos() {
  lua_getglobal(L, "repositories");
  if (!config_loaded || !lua_istable(L, -1)) {
    std::cout << print_error << "lua variable 'repositories' is not a table.\n";
    lua_pop(L, 1);
    return;
  }
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    const char *repo_name = lua_tostring(L, -2);
    if (!repo_name || !lua_istable(L, -1)) {
      std::cout << print_error << "repository key is not a table\n";
      lua_pop(L, 1);
      continue;
    }
    lua_getfield(L, -1, "url");
    const char* url = lua_tostring(L, -1);
    lua_pop(L, 1);
    cached_repos[repo_name].source.key = "url";
    cached_repos[repo_name].source.value = url ? url : "";
    lua_getfield(L, -1, "dependencies");
    if (!lua_istable(L, -1)) {
      //std::cout << print_warning << repo_name << ": \tlua variable 'dependencies' is not a table.\n";
      lua_pop(L, 2);
      continue;
    }
    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
      const char* depname = lua_tostring(L, -2);
      if (depname && lua_istable(L, -1)) {
        lua_getfield(L, -1, "url");
        const char* dep_url = lua_tostring(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, -1, "version");
        const char* dep_version = lua_tostring(L, -1);
        lua_pop(L, 1);
        cached_repos[repo_name].dependencies[depname].url = dep_url ? dep_url : "";
        cached_repos[repo_name].dependencies[depname].version = dep_version ? dep_version : "";
      }
      lua_pop(L, 1);
    }
    lua_pop(L, 1);
    lua_pop(L, 1);
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
    std::cout << print_error << "lua variable 'build_systems' is not a table.\n";
    return;
  }
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    const char *key = lua_tostring(L, -2);
    if (lua_isfunction(L, -1) == 0) {
      std::cout << print_error << "build value is not a function\n";
      lua_pop(L, 1);
      continue;
    }
    cached_build_systems[key] = lua_gettop(L);
    lua_pop(L, 1);
  }
  lua_pop(L, 1);
}

bool config_build(const char *path) {
  lua_getglobal(L, "build_systems");
  if (!config_loaded || !lua_istable(L, -1)) {
    std::cout << print_warning << "lua variable 'build_systems' is not a table.\n";
    lua_pop(L, 1);
    return false;
  }
  std::cout << print_pkgit << "lua variable 'build_systems' used successfully.\n";
  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    const char *key = lua_tostring(L, -2);
    if (!lua_istable(L, -1)) {
      std::cout << print_error << "build value " << key << " is not a table\n";
      lua_pop(L, 1);
      continue;
    }
    for (auto const& dir_entry : std::filesystem::directory_iterator(std::filesystem::current_path().string())) {
      std::string string_key = key;
      if (dir_entry.path().filename() != string_key) { continue; }
      lua_getfield(L, -1, "build");
      if (lua_isfunction(L, -1) == 0) {
        std::cout << print_error << "no build function for " << key << "\n";
        lua_pop(L, 1);
        continue;
      }
      if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        std::cout << "'" << key << "' build failed: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        continue;
      }
    }
    lua_pop(L, 1);
  }
  lua_pop(L, 1);
  return true;
}