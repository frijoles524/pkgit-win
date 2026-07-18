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
#include <unistd.h>

#include "build.h"

#include "globs.h"
#include "log.h"
#include "lua.h"
#include "pkg.h"
#include "pkgit_lua.h"

bool target_build(lua_State *L, char *lua_file, str *target) {
	if (!lua_try_table(L, lua_file, target->data))
		return false;
	if (lua_try_table(L, lua_file, "dependencies")) {
		lua_pushnil(L);
		install_dependencies(L);
	}
	lua_pop(L, 1);
	if (!lua_try_function(L, lua_file, "build"))
		return false;
	lua_pop(L, 1);
	return true;
}

bool repo_build(package_t *pkg) {
	lua_getglobal(L, "repositories");
	if (!lua_try_table(L, "init.lua", pkg->name.data)) {
		lua_pop(L, 2);
		return false;
	}
	if (lua_try_table(L, "init.lua", "dependencies")) {
		lua_pushnil(L);
		install_dependencies(L);
	}
	lua_pop(L, 1);
	if (!lua_try_table(L, "init.lua", "targets")) {
		lua_pop(L, 3);
		return false;
	}
	bool target_success = target_build(L, "init.lua", &pkg->target);
	lua_pop(L, 4);
	return target_success;
}

bool bldit(package_t *pkg) {
	init_bldit_state();
	if (!bldit_loaded)
		return false;
	lua_getglobal(B, "dependencies");
	if (!lua_istable(B, -1)) {
		bldit_isnt_type("dependencies", "table");
	} else {
		lua_pushnil(B);
		install_dependencies(B);
		lua_pop(B, 1);
	}
	lua_getglobal(B, "targets");
	if (!lua_istable(B, -1)) {
		bldit_isnt_type("targets", "table");
		lua_pop(B, 3);
		lua_close(B);
		return false;
	}
	bool target_success = target_build(B, "bldit.lua", &pkg->target);
	lua_pop(B, 1);
	// lua_pop(B, 2);
	//  lua_close(B);
	return target_success;
}

bool config_build(package_t *pkg) {
	lua_getglobal(L, "build_systems");
	if (!lua_istable(L, -1)) {
		lua_isnt_type("build_systems", "table");
		lua_pop(L, 1);
		return false;
	}
	bool target_success = false;
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		str key = mstr(lua_tostring(L, -2));
		if (!lua_istable(L, -1)) {
			lua_pop(L, 1);
			str_free(&key);
			continue;
		}
		str file_path = str_format("%.*s/%s", str_fmt(&pkg->src), key.data);
		if (access(file_path.data, F_OK) != 0) {
			lua_pop(L, 1);
			str_free(&file_path);
			str_free(&key);
			continue;
		}
		str_free(&file_path);
		str_free(&key);
		if (!lua_try_table(L, "init.lua", "targets"))
			continue;
		target_success = target_build(L, "init.lua", &pkg->target);
		if (target_success)
			break;
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
	return target_success;
}

bool build_loop(package_t *pkg) {
	if (flags.verbose)
		log_info("attempting init.lua: 'repositories.%.*s.build'",
				 str_fmt(&pkg->name));
	if (repo_build(pkg)) {
		return true;
	}
	if (flags.verbose)
		log_warn("failed init.lua: 'repositories.%.*s.build'",
				 str_fmt(&pkg->name));

	if (flags.verbose)
		log_info("attempting bldit.lua");
	if (bldit(pkg)) {
		return true;
	}
	if (flags.verbose)
		log_warn("failed bldit.lua");

	if (flags.verbose)
		log_info("attempting init.lua: 'build_systems'");
	if (config_build(pkg)) {
		return true;
	}
	if (flags.verbose)
		log_warn("failed init.lua: 'build_systems'");
	return false;
}

bool build(package_t *pkg) {
	char cwd[MAX_PATH_LEN];
	getcwd(cwd, MAX_PATH_LEN);
	if (!str_equal_cstr(&pkg->src, cwd) && !pkg->is_local)
		chdir(pkg->src.data);

	if (build_loop(pkg))
		return true;
	log_error("no usable build system was found for %.*s", str_fmt(&pkg->name));
	return false;
}
