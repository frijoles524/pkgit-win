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

#include "globs.h"

#include "files.h"
#include "log.h"
#include "lua_vars.h"
#include "str.h"

install_dirs_t inst_dirs = {0};
cli_flags_t flags = {0};
config_t cfg = {0};

void init_vars(void) {
	if (file_exists("/etc/pkgit/repos.lua")) {
		cfg.dir = mstr("/etc/pkgit");
		cfg.name = mstr("/etc/pkgit/repos.lua");

	} else {
		const char *tmp = getenv("XDG_CONFIG_HOME");
		if (tmp) {
			cfg.dir = str_format("%s/pkgit", tmp);
		} else {
			cfg.dir = str_format("%s/.config/pkgit", getenv("HOME"));
		}
		cfg.name = str_concat_cstr(&cfg.dir, "/init.lua");
	}
	// TODO: read into cfg.content

	cfg.repos = str_format("%.*s/repos.lua", str_fmt(&cfg.dir));
	init_install_directories();
}

void free_vars(void) {
	str_free(&cfg.dir);
	str_free(&cfg.name);
	str_free(&cfg.content);
	str_free(&cfg.repos);
	str_free(&inst_dirs.prefix);
	str_free(&inst_dirs.bin);
	str_free(&inst_dirs.lib);
	str_free(&inst_dirs.include);
	str_free(&inst_dirs.src);
}
