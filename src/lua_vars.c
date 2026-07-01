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

#include "lua_vars.h"

#include "lua_globs.h"
#include "globs.h"

void init_install_directories(void) {
	init_lua_state();
	if (!lua_istable(L, -1)) {
		lua_getglobal(L, "install_directories");
	}
	if (!lua_istable(L, -1)) {
		lua_isnt_type("install_directories", "table");
		return;
	}
	//lua_pop(L, 1);

	lua_getfield(L, -1, "bin");
	if (!lua_isstring(L, -1)) {
		lua_isnt_type("install_directories.bin", "string");
	} else {
		str_copy_cstr_into(&bin, lua_tostring(L, -1));
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "lib");
	if (!lua_isstring(L, -1)) {
		lua_isnt_type("install_directories.lib", "string");
	} else {
		str_copy_cstr_into(&lib, lua_tostring(L, -1));
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "include");
	if (!lua_isstring(L, -1)) {
		lua_isnt_type("install_directories.include", "string");
	} else {
		str_copy_cstr_into(&include, lua_tostring(L, -1));
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "src");
	if (!lua_isstring(L, -1)) {
		lua_isnt_type("install_directories.src", "string");
	} else {
		str_copy_cstr_into(&src, lua_tostring(L, -1));
	}
	lua_pop(L, 1);
}
