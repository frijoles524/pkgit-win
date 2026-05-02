#ifndef LUA_BUILD
#define LUA_BUILD

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

bool lua_build(const char* repository, const char* target, const char* path);

#endif
