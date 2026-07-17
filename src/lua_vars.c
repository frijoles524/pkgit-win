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

#include "pkgit_lua.h"

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

#define SETUP_INST_DIRS                                                        \
	X(bin)                                                                     \
	X(lib)                                                                     \
	X(include)                                                                 \
	X(src)

#define X(dir)                                                                 \
	do {                                                                       \
		lua_getfield(L, -1, #dir);                                             \
		if (!lua_isstring(L, -1)) {                                            \
			lua_isnt_type("install_directories." #dir, "string");              \
		} else {                                                               \
			str_copy_cstr_into(&inst_dirs.dir, lua_tostring(L, -1));           \
		}                                                                      \
		lua_pop(L, 1);                                                         \
	} while (0);

	SETUP_INST_DIRS
#undef X

	lua_pop(L, 1);
}

void init_prefix_directory(void) {
  lua_getglobal(L, "prefix");
  if (!lua_isstring(L, -1)) {
    lua_isnt_type("prefix", "string.");
    return;
  }
  inst_dirs.prefix = mstr(lua_tostring(L, -1));
  lua_pop(L, 1);
}
