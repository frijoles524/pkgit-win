#include <iostream>
#include "lua_state.hh"
#include "setup_repo.hh"
#include "ensure_repo.hh"
#include "vars.hh"

void setup_repo() {
  ensure_repo();

  init_lua_state();
  lua_State *L = get_lua_state();

  lua_getglobal(L, "repos");

  if (!lua_istable(L, -1)) {
    std::cout << print_error << "lua variable 'repos' is not a table.\n";
  }

  lua_pushnil(L);

  while (lua_next(L, -2) != 0) {
    const char *key = lua_tostring(L, -2);
    const char *value = lua_tostring(L, -1);

    repos[key] = value;

    lua_pop(L, 1);
  }
}