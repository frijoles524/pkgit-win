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
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "update_all.h"
#include "create_pkg.h"
#include "update_pkg.h"
#include "lua_state.h"
#include "vars.h"

void update_all(void) {
	init_lua_state();
	cache_repos();
	struct dirent* dirent_ptr;
	DIR* dir_ptr;
	if ((dir_ptr = opendir(get_install_dir("src"))) == NULL) {
		fprintf(
			stderr, "%s could not open %s\n",
			print_pkgit, get_install_dir("src")
		);
	}
	while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
		if (
			strcmp(dirent_ptr->d_name, "..") == 0 ||
			strcmp(dirent_ptr->d_name, ".") == 0
		) continue;
		for (size_t i = 0; i < cached_repos_count; i++) {
			if (strcmp(dirent_ptr->d_name, cached_repos[i].source_key) != 0) continue;
			Pkg pkg = create_pkg(cached_repos[i].source_value);
			update_pkg(pkg);
		}
	}
	closedir(dir_ptr);
	on_update(get_lua_state());
}