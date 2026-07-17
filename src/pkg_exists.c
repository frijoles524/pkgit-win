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
#include "str.h"

bool pkg_exists(str *name) {
	lua_getglobal(L, "repositories");
	if (!config_loaded || !lua_istable(L, -1)) {
		lua_pop(L, 1);
		return false;
	}
	if (!lua_try_table(L, "init.lua", name->data)) {
		lua_pop(L, 2);
		return false;
	}
	lua_pop(L, 2);
	return true;
}

str pkg_get_url(str *name) {
	lua_getglobal(L, "repositories");
	if (!config_loaded || !lua_istable(L, -1)) {
		lua_pop(L, 1);
		return mstr("");
	}
	if (!lua_try_table(L, "init.lua", name->data)) {
		lua_pop(L, 2);
		return mstr("");
	}
	lua_getfield(L, -1, "url");
	if (lua_isstring(L, -1)) {
		return mstr(lua_tostring(L, -1));
	}
	lua_pop(L, 3);
	return mstr("");
}
