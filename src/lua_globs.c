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
#include <string.h>

#include "lua.h"
#include "pkgit_lua.h"

#include "files.h"
#include "globs.h"
#include "log.h"
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
			if (flags.verbose)
				log_warn("cannot load repository file: %s",
						 lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
	str_free(&lua_path);
	config_loaded = true;
}

// FIXME: ensure this is called ONCE for clarity
void init_bldit_state(void) {
	if (B != NULL)
		return;
	B = luaL_newstate();
	luaL_openlibs(B);
	if (luaL_loadfile(B, "bldit.lua") || lua_pcall(B, 0, 0, 0)) {
		if (flags.verbose)
			log_warn("cannot run bldit: %s", lua_tostring(B, -1));
		return;
	}
	lua_pushfstring(B, "%s", inst_dirs.prefix.data);
	lua_setglobal(B, "prefix");
	lua_pop(B, 1);
	bldit_loaded = true;
}

void free_lua_state(void) {
	if (L != NULL) {
		lua_close(L);
		L = NULL;
	}
	config_loaded = false;
}

void free_bldit_state(void) {
	if (B != NULL) {
		lua_close(B);
		B = NULL;
	}
	bldit_loaded = false;
}

lua_State *get_lua_state(void) {
	return L;
}

lua_State *get_bldit_state(void) {
	return B;
}

void lua_isnt_type(char *variable, char *type) {
	if (flags.verbose)
		log_error("init.lua: '%s' is not a %s.", variable, type);
}

void bldit_isnt_type(char *variable, char *type) {
	if (flags.verbose)
		log_error("bldit.lua: '%s' is not a %s.", variable, type);
}

bool lua_try_function(lua_State *L, char *lua_file, char *fname) {
	lua_getfield(L, -1, fname);
	if (!lua_isfunction(L, -1)) {
		if (strcmp(lua_file, "bldit.lua"))
			bldit_isnt_type(fname, "function");
		else
			lua_isnt_type(fname, "function");
		lua_pop(L, 1);
	} else if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
		if (flags.verbose)
			log_warn("%s: '%s' function borked: %s", lua_file, fname,
					 lua_tostring(L, -1));
		lua_pop(L, 1);
		return false;
	}
	if (!lua_isnumber(L, -1) || lua_tonumber(L, -1) != 0) {
		if (flags.verbose)
			log_warn("%s: '%s' failed: %s", lua_file, fname,
					 lua_tostring(L, -1));
		lua_pop(L, 1);
		return false;
	}
	lua_pop(L, 1);
	return true;
}

bool lua_try_table(lua_State *L, char *lua_file, char *tname) {
	lua_getfield(L, -1, tname);
	if (!lua_istable(L, -1)) {
		if (!strcmp(lua_file, "bldit.lua"))
			bldit_isnt_type(tname, "table");
		else
			lua_isnt_type(tname, "table");
		return false;
	}
	return true;
}

str bldit_getver(void) {
	init_bldit_state();
	if (!bldit_loaded) {
		return mstr("");
	}
	lua_getglobal(B, "bldit_version");
	if (!lua_isstring(B, -1)) {
		if (flags.verbose)
			log_warn("bldit.lua: 'bldit_version' is not a string.");
		lua_pop(B, 1);
		return mstr("");
	}
	str bldit_version = mstr(lua_tostring(B, -1));
	lua_pop(B, 1);
	return bldit_version;
}

str bldit_pkg_getver(void) {
	init_bldit_state();
	lua_getglobal(B, "package_version");
	if (!lua_isstring(B, -1)) {
		if (flags.verbose)
			log_warn("bldit.lua: 'package_version' is not a string.");
		lua_pop(B, 1);
		return mstr("");
	}
	str package_version = mstr(lua_tostring(B, -1));
	lua_pop(B, 1);
	return package_version;
}

bool is_bldit_usable(void) {
	str bldit_version = bldit_getver();
	if (bldit_version.len && str_equal_cstr(&bldit_version, VERSION))
		goto done;
	bool prev_pass = false;
	for (size_t i = 0; i < bldit_version.len; i++) {
		if (bldit_version.data[i] == '.')
			continue;
		if ((bldit_version.data[i] - '0') <= (VERSION[i] - '0')) {
			prev_pass = ((bldit_version.data[i] - '0') != (VERSION[i] - '0'));
			continue;
		} else {
			str_free(&bldit_version);
			return prev_pass;
		}
	}
done:
	str_free(&bldit_version);
	return true;
}
