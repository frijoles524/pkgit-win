#include <iostream>
#include <filesystem>
#include <map>
#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lualib.h>

#include "lua_build.hh"
#include "vars.hh"

std::map<std::string, int> build_files;

bool lua_build(const char *path) {
  lua_State *L = lua_open();
  luaL_openlibs(L);

  if (luaL_loadfile(L, config_file.c_str()) || lua_pcall(L, 0, 0, 0)){
    std::cout << print_error << "cannot run configuration script: " << lua_tostring(L, -1) << "\n";
  }

  lua_getglobal(L, "build_systems");

  if (!lua_istable(L, -1)) {
    std::cout << print_error << "lua variable 'build_systems' is not a table.\n";
  }

  lua_pushnil(L);

  bool build_found = false;
  while (lua_next(L, -2) != 0) {
    const char *key = lua_tostring(L, -2);
    int value = lua_type(L, -1);

    if (lua_isfunction(L, -1) == 0) {
      std::cout << print_error << "build value is not a function\n";
      lua_pop(L, 1);
      continue;
    }

    build_files[key] = value;

    for (auto const& dir_entry : std::filesystem::directory_iterator(std::filesystem::current_path().string())) {
      std::string string_key = key;
      if (dir_entry.path().filename() != string_key) { continue; }
      build_found = true;
      lua_pushvalue(L, -1);
      lua_pushstring(L, std::filesystem::current_path().string().c_str());
      std::cout << "calling lua build function according to key filename '"<< key << "'...\n";
      if (lua_pcall(L, 1, 0, 0) != 0) {
        std::cout << print_error << "lua build function failed to run\n";
        break;
      }
      if (build_found) { break; }
    }
    lua_pop(L, 1);
  }
  return build_found;
}
