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
#define _XOPEN_SOURCE 700

#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "files.h"
#include "globs.h"
#include "log.h"
#include "pkgit_lua.h"

bool target_uninstall(lua_State *L, char *lua_file, str *target) {
	if (!lua_try_table(L, lua_file, target->data)) {
		lua_pop(L, 1);
		return false;
	}
	if (!lua_try_function(L, lua_file, "uninstall")) {
		lua_pop(L, 1);
		return false;
	}
	lua_pop(L, 1);
	return true;
}

bool repo_uninstall(package_t *pkg) {
	if (!config_loaded || !lua_istable(L, -1)) {
		lua_isnt_type("repositories", "table");
		lua_pop(L, 1);
		return false;
	}
	if (!lua_try_table(L, "init.lua", pkg->name.data)) {
		lua_isnt_type(pkg->name.data, "table");
		lua_pop(L, 2);
		return false;
	}
	if (!lua_try_table(L, "init.lua", "targets"))
		return false;
	bool target_success = target_uninstall(L, "init.lua", &pkg->target);
	lua_pop(L, 3);
	return target_success;
}

bool bldit_uninstall(package_t *pkg) {
	init_bldit_state();
	if (!bldit_loaded) {
		lua_close(B);
		return false;
	}
	if (!is_bldit_usable()) {
		log_error("bldit version is newer than the installed pkgit version");
		log_error("consider updating pkgit");
		if (!flags.force)
			return false;
	}
	lua_pushfstring(B, "%s", inst_dirs.prefix.data);
	lua_setglobal(B, "prefix");
	lua_pop(B, 1);
	if (!lua_try_table(B, "bldit.lua", "targets"))
		return false;
	bool target_success = target_uninstall(B, "bldit.lua", &pkg->target);
	lua_pop(B, 2);
	lua_close(B);
	return target_success;
}

bool config_uninstall(package_t *pkg) {
	lua_getglobal(L, "build_systems");
	lua_pushnil(L);
	bool target_success = false;
	while (lua_next(L, -2) != 0) {
		const char *key = lua_tostring(L, -2);
		if (!lua_istable(L, -1)) {
			lua_pop(L, 1);
			continue;
		}
		str file_path = str_format("%.*s/%s", str_fmt(&pkg->src), key);
		if (access(file_path.data, F_OK) != 0) {
			lua_pop(L, 1);
			str_free(&file_path);
			continue;
		}
		str_free(&file_path);
		if (!lua_try_table(L, "init.lua", "targets")) {
			lua_pop(L, 2);
			continue;
		}
		target_success = target_uninstall(L, "init.lua", &pkg->target);
		lua_pop(L, 2);
		if (target_success)
			break;
	}
	lua_pop(L, 1);
	return target_success;
}

static int remove_installed(const char *src_path, const struct stat *sb,
							int typeflag, struct FTW *ftwbuf) {
	(void)sb;
	(void)ftwbuf;

	if (typeflag == FTW_F) {
		const char *filename = src_path + ftwbuf->base;
		const char *ext = strrchr(filename, '.');
		if (!ext)
			ext = "";

		if (strncmp(ext, ".so", 3) == 0) {
			char dest[MAX_PATH_LEN];
			snprintf(dest, sizeof(dest), "%s/%s", inst_dirs.lib.data, filename);
			if (file_exists(dest))
				remove(dest);
		} else if (access(src_path, X_OK) == 0) {
			if (strcmp(ext, ".sample") != 0 && strcmp(filename, "bldit") != 0 &&
				strcmp(filename, "build.sh") != 0 &&
				strcmp(filename, "compile.sh") != 0) {
				char dest[MAX_PATH_LEN];
				snprintf(dest, sizeof(dest), "%s/%s", inst_dirs.bin.data,
						 filename);
				if (file_exists(dest))
					remove(dest);
			}
		} else if (strcmp(ext, ".h") == 0) {
			char dest[MAX_PATH_LEN];
			snprintf(dest, sizeof(dest), "%s/%s", inst_dirs.include.data,
					 filename);
			if (file_exists(dest))
				remove(dest);
		}
	}
	return 0;
}

void pkg_remove(package_t *pkg) {
	if (!is_directory(pkg->src.data)) {
		log_error("%.*s is not installed!", str_fmt(&pkg->name));
		return;
	}
	chdir(pkg->src.data);

	bool uninstall_available = false;
	if (!uninstall_available)
		if (repo_uninstall(pkg))
			uninstall_available = true;
	if (!uninstall_available)
		if (bldit_uninstall(pkg))
			uninstall_available = true;
	if (!uninstall_available)
		if (config_uninstall(pkg))
			uninstall_available = true;

	if (!uninstall_available) {
		log_error("no uninstall function availible for package: %.*s",
				  str_fmt(&pkg->name));
		return;
	}

	// TODO: refactor this
	nftw(pkg->src.data, remove_installed, 64, FTW_PHYS);
	const char *last_slash = strrchr(pkg->src.data, '/');
	char target[MAX_PATH_LEN];
	if (last_slash && last_slash != pkg->src.data) {
		size_t parent_len = last_slash - pkg->src.data;
		snprintf(target, sizeof(target), "%.*s", (int)parent_len,
				 pkg->src.data);
	} else {
		snprintf(target, sizeof(target), "%s", pkg->src.data);
	}
	remove_tree(pkg->src.data);
	log_success("removed %.*s", str_fmt(&pkg->name));
}
