#ifndef LUA_STATE
#define LUA_STATE

extern "C" {
#include <luajit-2.1/lua.h>
#include <luajit-2.1/lauxlib.h>
#include <luajit-2.1/lualib.h>
}

void init_lua_state();
void free_lua_state();
lua_State* get_lua_state();

void cache_install_directories();
void cache_repos();
void cache_build_systems();

#endif