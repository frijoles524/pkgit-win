#ifndef LUA_STATE_H
#define LUA_STATE_H

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "vars.h"

extern Map cached_install_directories;
extern Repo cached_repos[MAX_REPOS];
extern size_t cached_repos_count;
extern Map cached_build_systems;

void push_lua_path(lua_State *L, const char *new_path);
void init_lua_state(void);
void free_lua_state(void);
lua_State* get_lua_state(void);

void cache_install_directories(void);
bool repo_build(const char *repository);
bool bldit(const char *target);
bool config_build(const char *path);
void cache_repos(void);
void cache_build_systems(void);

#endif