#include <iostream>

extern "C" {
#include <luajit-2.1/lua.h>
#include <luajit-2.1/lauxlib.h>
#include <luajit-2.1/lualib.h>
}

#include "set_install_directories.hh"
#include "vars.hh"

void set_install_directories() {
  lua_State *L = lua_open();
  luaL_openlibs(L);

  if (luaL_loadfile(L, config_file.c_str()) || lua_pcall(L, 0, 0, 0)){
    std::cout << print_error << "cannot run configuration script: " << lua_tostring(L, -1) << "\n";
    return;
  }

  lua_getglobal(L, "install_directories");

  if (!lua_istable(L, -1)) {
    std::cout << print_error << "lua variable 'install_directories' is not a table.\n";
  }

  lua_pushnil(L);

  while (lua_next(L, -2) != 0) {
    const char *key = lua_tostring(L, -2);
    const char *value = lua_tostring(L, -1);

    install_directories[key] = value;

    lua_pop(L, 1);
  }
}
