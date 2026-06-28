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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua_state.h"
#include "set_install_directories.h"
#include "vars.h"

void set_install_directories(void) {
	lua_State *L = get_lua_state();
	cache_prefix_directory();
	lua_getglobal(L, "install_directories");
	if (!lua_istable(L, -1)) {
		printf(
			"%slua variable 'install_directories' is not a table.\n",
			print_error
		);
	}
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		const char *key = lua_tostring(L, -2);
		const char *value = lua_tostring(L, -1);
		if (key && value) {
			map_put(&cached_install_directories, strdup(key), strdup(value));
		}
		lua_pop(L, 1);
	}
}

void map_init(Map *map) {
	map->items = NULL;
	map->size = 0;
	map->capacity = 0;
}

void map_put(Map *map, char *key, char *value) {
	for (size_t i = 0; i < map->size; i++) {
		if (strcmp(map->items[i].key, key) == 0) {
			free(map->items[i].value);
			map->items[i].value = value;
			free(key);
			return;
		}
	}
	if (map->size >= map->capacity) {
		size_t new_capacity = map->capacity == 0 ? 8 : map->capacity * 2;
		MapItem *new_items = realloc(map->items, new_capacity * sizeof(MapItem));
		if (!new_items) return;
		map->items = new_items;
		map->capacity = new_capacity;
	}
	map->items[map->size].key = key;
	map->items[map->size].value = value;
	map->size++;
}

char *map_get(Map *map, const char *key) {
	for (size_t i = 0; i < map->size; i++) {
		if (strcmp(map->items[i].key, key) != 0) continue;
		return map->items[i].value;
	}
	return NULL;
}