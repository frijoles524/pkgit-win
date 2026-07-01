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

#ifndef PKGIT_LUA_GLOBALS_H
#define PKGIT_LUA_GLOBALS_H

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdbool.h>

extern lua_State *L;
extern lua_State *B;
extern bool config_loaded;

void push_lua_path(lua_State *L, const char *new_path);
void init_lua_state(void);
void init_bldit(void);
void free_lua_state(void);

lua_State *get_lua_state(void);

void lua_isnt_type(char* variable, char* type);

#endif
