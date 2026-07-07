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

#ifndef PKGIT_LUA_H
#define PKGIT_LUA_H

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdbool.h>

#include "pkg.h"

extern lua_State *L;
extern lua_State *B;
extern bool config_loaded;
extern bool bldit_loaded;

// init
void push_lua_path(lua_State *L, const char *new_path);
void init_lua_state(void);
void init_bldit_state(void);
void free_lua_state(void);
void free_bldit_state(void);
lua_State *get_lua_state(void);
lua_State *get_bldit_state(void);

// gobal vars
void init_install_directories(void);
void init_prefix_directory(void);

// helpers
void lua_isnt_type(char* variable, char* type);
void bldit_isnt_type(char* variable, char* type);
bool lua_try_function(lua_State *L, char *lua_file, char *fname);
bool lua_try_table(lua_State *L, char *lua_file, char *tname);

// install
void install_dependencies(lua_State *L);
void pkg_install(package_t *pkg);

#endif
