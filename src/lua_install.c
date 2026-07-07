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

#include "add_repo.h"
#include "build.h"
#include "fetch.h"
#include "files.h"
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

bool target_install(lua_State *L, char* lua_file, str *target) {
	if (!lua_try_table(L, lua_file, target->data)) return false;
	lua_try_function(L, lua_file, "pre_install");
	if (!lua_try_function(L, lua_file, "install")) return false;
	lua_try_function(L, lua_file, "post_install");
	lua_pop(L, 1);
	return true;
}

bool repo_install(package_t *pkg) {
	lua_getglobal(L, "repositories");
	if (!config_loaded || !lua_istable(L, -1)) {
		lua_isnt_type("repositories", "table");
		lua_pop(L, 1);
		return false;
	}
	if (!lua_try_table(L, "init.lua", pkg->name.data)) {
		lua_pop(L, 1);
		return false;
	}
	if (!lua_try_table(L, "init.lua", "targets")) return false;
	bool target_success = target_install(L, "init.lua", &pkg->target);
	lua_pop(L, 3);
	return target_success;
}

bool bldit_install(package_t *pkg) {
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
	bool target_success = target_install(B, "bldit.lua", &pkg->target);
	lua_pop(B, 1);
	lua_close(B);
	return target_success;
}

bool config_install(package_t *pkg) {
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
			continue;
		}
		str_free(&file_path);
		lua_try_table(L, "init.lua", "targets");
		target_success = target_install(L, "init.lua", &pkg->target);
		lua_pop(L, 2);
	}
	lua_pop(L, 1);
	return target_success;
}

void pkg_install(package_t *pkg) {
	if (is_directory(pkg->src.data)) {
		if (!flags.force) {
			log_info("%.*s is already installed.", str_fmt(&pkg->name));
			return;
		} else {
			log_warn("%.*s is already installed.", str_fmt(&pkg->name));
		}
	}
	char cwd[MAX_PATH_LEN];
	getcwd(cwd, MAX_PATH_LEN);
	if (str_equal_cstr(&pkg->src, cwd)) chdir(pkg->src.data);

	//if (pkg->is_local) {
	//	cpdir(cwd, pkg->src);
	//} else {
		log_pkgit("fetching " GREEN "%.*s" COLOR_RESET , str_fmt(&pkg->name));
		if (!fetch(pkg)) return;
		log_pkgit("fetched " GREEN "%.*s" COLOR_RESET , str_fmt(&pkg->name));
	//}

	log_pkgit("building " GREEN "%.*s" COLOR_RESET , str_fmt(&pkg->name));
	if (!build(pkg)) return;
	log_pkgit("built " GREEN "%.*s" COLOR_RESET , str_fmt(&pkg->name));

	bool install_success = false;
	if (!install_success && repo_install(pkg))
	install_success = true;
	if (!install_success && bldit_install(pkg))
	install_success = true;
	if (!install_success && config_install(pkg))
	install_success = true;
	if (!install_success) {
		log_error("no install function availible for package: %.*s", str_fmt(&pkg->name));
		return;
	}
	log_success("installed " GREEN "%.*s" COLOR_RESET , str_fmt(&pkg->name));

	bool repo_exists = false;
	//for (size_t i = 0; i < cached_repos_count; i++) {
	//char *repo_name = name_from_url(cached_repos[i].source_key);
	//if (strcmp(repo_name, pkg.name) == 0) { repo_exists = true; }
	//free(repo_name);
	//}

	if (!repo_exists) {
		log_pkgit("adding " GREEN "%.*s" COLOR_RESET , &pkg->name);
		if (pkg->url.len > 0) {
			add_repo(pkg);
			log_pkgit("added " GREEN "%.*s" COLOR_RESET , &pkg->name);
		}
	} else {
		log_info("repo already exists, done");
	}
}
