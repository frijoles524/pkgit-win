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

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "search.h"

#include "globs.h"
#include "log.h"
#include "pkgit_lua.h"

void search(char *arg) {
	lua_getglobal(L, "repositories");
	if (!lua_istable(L, -1)) {
		lua_isnt_type("repositories", "table");
		lua_pop(L, 1);
		return;
	}
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		str key = mstr(lua_tostring(L, -2));
		if (!lua_istable(L, -1)) {
			lua_pop(L, 1);
			str_free(&key);
			continue;
		}
		if (key.len <= 1) str_println(&key);
		else if (strstr(key.data, arg)) str_println(&key);
		if (str_is_valid(&key)) str_free(&key);
		lua_pop(L, 1);
	}
}

void list_installed(void) {
	struct dirent* dirent_ptr;
	DIR* dir_ptr = opendir(inst_dirs.src.data);

	if (dir_ptr == NULL) {
		log_error("could not open %.*s", str_fmt(&inst_dirs.src));
		return;
	}

	while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
		if (
			strcmp(dirent_ptr->d_name, "..") == 0 ||
			strcmp(dirent_ptr->d_name, ".") == 0
		) continue;
		printf("%s\n", dirent_ptr->d_name);
	}
}
