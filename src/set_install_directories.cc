#include <iostream>
#include "lua_state.hh"
#include "set_install_directories.hh"
#include "vars.hh"

void set_install_directories() {
  init_lua_state();
  lua_State *L = get_lua_state();

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