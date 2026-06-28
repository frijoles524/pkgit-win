/*

  pkgit - package it!

  Copyright (C) 2026 dacctal
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

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

void cache_repos(void);
void cache_build_systems(void);
void cache_install_directories(void);
void cache_prefix_directory(void);
bool on_update(lua_State *L);
bool target_loop_on_update(lua_State *L, const char* lua_file, const char *target);
bool repo_build(const char *repository, const char *target);
bool repo_install(const char *repository, const char *target);
bool repo_uninstall(const char *repository, const char *target);
bool bldit(const char *target);
bool bldit_install(const char *target);
bool bldit_uninstall(const char *target);
bool config_build(const char *path, const char *target);
bool config_install(const char *path, const char *target);
bool config_uninstall(const char *path, const char *target);

char* bldit_getver(void);
char* bldit_pkg_getver(void);

#endif