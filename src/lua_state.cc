#include <iostream>
#include <unordered_map>
#include "lua_state.hh"
#include "vars.hh"

static lua_State *L = nullptr;
static bool config_loaded = false;

std::unordered_map<std::string, std::string> cached_install_directories;
std::unordered_map<std::string, std::string> cached_repos;
std::unordered_map<std::string, int> cached_build_systems;

void init_lua_state() {
  if (L != nullptr) return;
  L = luaL_newstate();
  luaL_openlibs(L);

  if (luaL_loadfile(L, config_file.c_str()) || lua_pcall(L, 0, 0, 0)) {
    std::cout << print_error << "cannot run configuration script: " << lua_tostring(L, -1) << "\n";
    return;
  }

  if (luaL_loadfile(L, repo_file.c_str()) || lua_pcall(L, 0, 0, 0)) {
    std::cout << print_error << "cannot run repository script: " << lua_tostring(L, -1) << "\n";
    return;
  }

  config_loaded = true;
}

void free_lua_state() {
  if (L != nullptr) {
    lua_close(L);
    L = nullptr;
  }
  config_loaded = false;
}

lua_State* get_lua_state() {
  return L;
}

void cache_install_directories() {
  if (!config_loaded || !lua_istable(L, -1)) {
    lua_getglobal(L, "install_directories");
  }

  if (!lua_istable(L, -1)) {
    std::cout << print_error << "lua variable 'install_directories' is not a table.\n";
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

void cache_repos() {
  if (!config_loaded) {
    lua_getglobal(L, "repos");
  } else if (!lua_istable(L, -1)) {
    lua_getglobal(L, "repos");
  }

  if (!lua_istable(L, -1)) {
    std::cout << print_error << "lua variable 'repos' is not a table.\n";
    return;
  }

  lua_pushnil(L);
  while (lua_next(L, -2) != 0) {
    const char *key = lua_tostring(L, -2);
    const char *value = lua_tostring(L, -1);
    cached_repos[key] = value;
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
