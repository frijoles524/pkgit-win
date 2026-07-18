/*
pkgit - package it!
  Copyright (C) 2026 dacctal
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <dirent.h>
#include <string.h>

#include "files.h"
#include "globs.h"
#include "is_updated.h"
#include "log.h"
#include "pkgit_lua.h"

lua_State *U = NULL;

void init_update_state(void) {
	if (U != NULL)
		return;
	U = luaL_newstate();
	luaL_openlibs(U);
	str lua_path = str_format("%.*s/?.lua", str_fmt(&cfg.dir));
	push_lua_path(U, lua_path.data);
	if (luaL_loadfile(U, cfg.name.data) || lua_pcall(U, 0, 0, 0)) {
		log_error("cannot run configuration script: %s", lua_tostring(U, -1));
		log_pkgit("to generate a configuration file, head into the");
		log_pkgit(
			"root directory of the pkgit source and run `make defconfig`");
		exit(EXIT_FAILURE);
	}
	if (file_exists(cfg.repos.data)) {
		if (luaL_loadfile(U, cfg.repos.data) || lua_pcall(U, 0, 0, 0)) {
			if (flags.verbose) log_warn("cannot load repository file: %s", lua_tostring(U, -1));
			lua_pop(U, 1);
		}
	}
	str_free(&lua_path);
	config_loaded = true;
}

bool on_pkg_update(lua_State *L, package_t *pkg) {
	lua_getglobal(L, "repositories");
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		return false;
	}
	if (!lua_try_table(L, "init.lua", pkg->name.data)) {
		lua_pop(L, 2);
		return false;
	}
	if (!lua_try_table(L, "init.lua", "targets")) {
		lua_pop(L, 3);
		return false;
	}
	if (!lua_try_table(L, "init.lua", pkg->target.data)) {
		lua_pop(L, 4);
		return false;
	}
	if (!lua_try_function(L, "init.lua", "on_update")) {
		lua_pop(L, 4);
		return false;
	}
	lua_pop(L, 4);
	return true;
}

void pkg_update(package_t *pkg) {
	if (is_updated(&pkg->src)) {
		if (flags.verbose) log_info(
			"%.*s is already up to date.",
			str_fmt(&pkg->name)
		);
		return;
	}
	flags.force = true;
	str_copy_cstr_into(&pkg->version, "HEAD");
	pkg_install(pkg);
	if (!on_pkg_update(L, pkg)) log_warn(
		"init.lua: 'repositories.%.*s.%.*s.on_update' function failed",
		str_fmt(&pkg->name), str_fmt(&pkg->target)
	);
	pkg_free(pkg);
}

bool on_all_update(lua_State *L) {
	char* fname = "on_update";
	char* lua_file = "init.lua";
	lua_getglobal(L, fname);
	if (!lua_isfunction(L, -1)) {
		if (flags.verbose) lua_isnt_type(fname, "function");
		lua_pop(L, 1);
	} else if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
		if (flags.verbose) log_warn(
			"%s: '%s' function borked: %s",
			lua_file, fname, lua_tostring(L, -1)
		);
		lua_pop(L, 1);
		return false;
	}
	if (!lua_isnumber(L, -1) || lua_tonumber(L, -1) != 0) {
		if (flags.verbose) log_warn(
			"%s: '%s' failed: %s",
			lua_file, fname, lua_tostring(L, -1)
		);
		lua_pop(L, 1);
		return false;
	}
	lua_pop(L, 1);
	return true;
}

void all_update(void) {
	struct dirent* dirent_ptr;
	DIR* dir_ptr;
	if ((dir_ptr = opendir(inst_dirs.src.data)) == NULL) {
		log_error("could not open %s", str_fmt(&inst_dirs.src));
	}
	while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
		if (
			strcmp(dirent_ptr->d_name, "..") == 0 ||
			strcmp(dirent_ptr->d_name, ".") == 0
		) continue;
		str pkg_name = mstr(dirent_ptr->d_name);
		if (!pkg_exists(&pkg_name)) {
			str_free(&pkg_name);
			continue;
		}
		package_t pkg = pkg_create(&pkg_name);
		pkg_update(&pkg);
		pkg_free(&pkg);
		if (str_is_valid(&pkg_name)) str_free(&pkg_name);
	}
	closedir(dir_ptr);
	if (!on_all_update(L))
		log_warn("init.lua: 'on_update' function failed");
}

void declare(void) {
	init_update_state();
	lua_getglobal(U, "repositories");
	if (!lua_istable(U, -1)) {
		lua_isnt_type("repositories", "table");
		lua_pop(U, 1);
		return;
	}
	lua_pushnil(U);
	while (lua_next(U, -2) != 0) {
		str key = mstr(lua_tostring(U, -2));
		if (!lua_istable(U, -1)) {
			lua_pop(U, 1);
			str_free(&key);
			continue;
		}
		package_t pkg = pkg_create(&key);
		pkg_install(&pkg);
		pkg_free(&pkg);
		if (str_is_valid(&key)) str_free(&key);
		lua_pop(U, 1);
	}
	if (!on_all_update(L))
		log_warn("init.lua: 'on_update' function failed");
	lua_pop(U, 1);
}
