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
#include <stdio.h>

#include "globs.h"

#include "files.h"
#include "lua_vars.h"
#include "str.h"

int is_verbose = 1;
int is_forced = 0;

int is_root_config = 0;
str home_config_dir; 
str home_config_file; 
str home_repos_file; 
str root_config_dir;
str root_config_file;
str root_repos_file;
str config_dir;
str config_file;
str repos_file;

str prefix_dir;

str bin; 
str lib; 
str include; 
str src; 

void init_vars(void) {
	home_config_dir = str_format("%s/.config/pkgit", getenv("HOME"));
	home_config_file = str_format("%s/init.lua", home_config_dir.data);
	home_repos_file = str_format("%s/repos.lua", home_config_dir.data);
	root_config_dir = str_from_cstr("/etc/pkgit");
	root_config_file = str_format("%s/init.lua", root_config_dir.data);
	root_repos_file = str_format("%s/repos.lua", root_config_dir.data);
	str_copy_into(&config_dir, &home_config_dir);
	if (file_exists(root_config_file.data)) str_copy_into(&config_dir, &root_config_dir);
	config_file = str_format("%s/init.lua", config_dir.data);
	repos_file = str_format("%s/repos.lua", config_dir.data);
	init_install_directories();
}

void free_vars(void) {
	str_free(&home_config_dir);
	str_free(&home_config_file);
	str_free(&home_repos_file);
	str_free(&root_config_dir);
	str_free(&root_config_file);
	str_free(&root_repos_file);
	str_free(&config_dir);
	str_free(&config_file);
	str_free(&repos_file);
}

//	old init_vars()
//
//	is_root_config = file_exists(root_config);
//	if (is_root_config) snprintf(config_dir, MAX_PATH_LEN, "/etc/pkgit");
//	else snprintf(config_dir, MAX_PATH_LEN, "%s/.config/pkgit", home_dir);
//
//	snprintf(config_file, MAX_PATH_LEN, "%s/init.lua", config_dir);
//	snprintf(repo_file, MAX_PATH_LEN, "%s/repos.lua", config_dir);
//
//	config_exists = file_exists(root_config) || file_exists(home_config());
//
//	snprintf(bin, MAX_PATH_LEN, "%s/.local/bin", home_dir);
//	snprintf(lib, MAX_PATH_LEN, "%s/.local/lib", home_dir);
//	snprintf(inc, MAX_PATH_LEN, "%s/.local/include", home_dir);
//	snprintf(src, MAX_PATH_LEN, "%s/.local/share/pkgit", home_dir);
//
//	install_directories[0] = config_dir;
//	install_directories[1] = strdup(get_install_dir("bin"));
//	install_directories[2] = strdup(get_install_dir("lib"));
//	install_directories[3] = strdup(get_install_dir("inc"));
//	install_directories[4] = strdup(get_install_dir("src"));
