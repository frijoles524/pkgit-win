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

#include <stdlib.h>

#include "files.h"
#include "globs.h"
#include "log.h"
#include "lua_globs.h"
#include "str.h"

lua_State *L = NULL;
lua_State *B = NULL;
bool config_loaded = false;
bool bldit_loaded = false;

void push_lua_path(lua_State *L, const char *new_path) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "path");
	const char *current_path = lua_tostring(L, -1);
	if (!current_path)
		current_path = "";
	lua_pop(L, 1);
	lua_pushfstring(L, "%s;%s", current_path, new_path);
	lua_setfield(L, -2, "path");
	lua_pop(L, 1);
}

void init_lua_state(void) {
	if (L != NULL)
		return;
	L = luaL_newstate();
	luaL_openlibs(L);
	str lua_path = str_format("%.*s/?.lua", str_fmt(&cfg.dir));
	push_lua_path(L, lua_path.data);
	if (luaL_loadfile(L, cfg.name.data) || lua_pcall(L, 0, 0, 0)) {
		log_error("cannot run configuration script: %s", lua_tostring(L, -1));
		log_pkgit("to generate a configuration file, head into the");
		log_pkgit(
			"root directory of the pkgit source and run `make defconfig`");
		exit(EXIT_FAILURE);
	}
	if (file_exists(cfg.repos.data)) {
		if (luaL_loadfile(L, cfg.repos.data) || lua_pcall(L, 0, 0, 0)) {
			log_warn("cannot load repository file: %s", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
	str_free(&lua_path);
	config_loaded = true;
}

void init_bldit(void) {
	if (B != NULL)
		return;
	B = luaL_newstate();
	luaL_openlibs(B);
	if (luaL_loadfile(B, "bldit.lua") || lua_pcall(B, 0, 0, 0)) {
		if (flags.verbose)
			log_warn("cannot run bldit: %s", lua_tostring(B, -1));
		return;
	}
	bldit_loaded = true;
}

void free_lua_state(void) {
	if (L != NULL) {
		lua_close(L);
		L = NULL;
	}
	config_loaded = false;
}

lua_State *get_lua_state(void) {
	return L;
}

void lua_isnt_type(char *variable, char *type) {
	if (flags.verbose)
		log_error("init.lua: '%s' is not a %s.", variable, type);
}
