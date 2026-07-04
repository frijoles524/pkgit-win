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


#include <unistd.h>

#include "pkgit_lua.h"

#include "globs.h"
#include "log.h"
#include "pkg.h"

void install_dependencies(lua_State *L) {
  while (lua_next(L, -2) != 0) {
    const char *depname = lua_tostring(L, -2);
    if (depname && lua_istable(L, -1)) {
      lua_getfield(L, -1, "url");
      str_slc dep_url = mstrslc(lua_tostring(L, -1));
      lua_pop(L, 1);
      lua_getfield(L, -1, "version");
      str_slc dep_version = mstrslc(lua_tostring(L, -1));
      lua_pop(L, 1);
      package_t pkg = pkg_create(dep_url);
      pkg.version = str_from_str_slc(dep_version);
      const int top = lua_gettop(L);
      char cwd[MAX_PATH_LEN];
      if (getcwd(cwd, sizeof(cwd)) != NULL) {
        //install_pkg(pkg);
        chdir(cwd);
      }
      lua_settop(L, top);
      pkg_free(&pkg);
    }
    lua_pop(L, 1);
  }
}

bool target_loop_install(lua_State *L, char* lua_file, str *target) {
	if (!lua_try_table(L, lua_file, target->data)) return false;
	lua_try_function(L, lua_file, "pre_install");
	lua_try_function(L, lua_file, "install");
	lua_try_function(L, lua_file, "post_install");
	lua_pop(L, 1);
	return true;
}

bool repo_install(const char *repository, const char* target) {
	lua_getglobal(L, "repositories");
	if (!config_loaded || !lua_istable(L, -1)) {
		lua_isnt_type("repositories", "table");
		lua_pop(L, 1);
		return false;
	}
	str str_repo = mstr(repository);
	if (!lua_try_table(L, "init.lua", str_repo.data)) {
		lua_pop(L, 1);
		return false;
	}
	str_free(&str_repo);
	if (!lua_try_table(L, "init.lua", "targets")) return false;
	str str_target = mstr(target);
	bool target_loop_success = target_loop_install(L, "init.lua", &str_target);
	str_free(&str_target);
	lua_pop(L, 3);
	return target_loop_success;
}

bool bldit_install(const char *target) {
	init_bldit_state();
	//if (!is_bldit_usable()) {
	//	log_error("bldit version is newer than the installed pkgit version");
	//	log_error("consider updating pkgit");
	//	if (!flags.force) return false;
	//}
	lua_pushfstring(B, "%s", inst_dirs.prefix.data);
	lua_setglobal(B, "prefix");
	lua_pop(B, 1);
	lua_try_table(L, "bldit.lua", "targets");
	str str_target = mstr(target);
	bool target_loop_success = target_loop_install(B, "bldit.lua", &str_target);
	str_free(&str_target);
	lua_pop(B, 1);
	lua_close(B);
	return target_loop_success;
}

bool config_install(const char *path, const char *target) {
	lua_getglobal(L, "build_systems");
	lua_pushnil(L);
	bool target_loop_success = false;
	while (lua_next(L, -2) != 0) {
		const char *key = lua_tostring(L, -2);
		if (!lua_istable(L, -1)) {
			lua_pop(L, 1);
			continue;
		}
		char file_path[MAX_PATH_LEN];
		snprintf(file_path, sizeof(file_path), "%s/%s", path, key);
		if (access(file_path, F_OK) != 0) {
			lua_pop(L, 1);
			continue;
		}
		lua_try_table(L, "init.lua", "targets");
		str str_target = mstr(target);
		target_loop_success = target_loop_install(L, "init.lua", &str_target);
		str_free(&str_target);
		lua_pop(L, 2);
	}
	lua_pop(L, 1);
	return target_loop_success;
}
