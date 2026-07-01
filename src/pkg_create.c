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
#include <string.h>

#include "pkg_create.h"

#include "files.h"
#include "globs.h"
#include "str.h"

static str get_destdir(str_slc cwd, str_slc arg) {
	str destdir;
	if (str_slc_first(arg) == '.') {
		str_format_into(
			&destdir, "%s/%s",
			src, str_slc_from_after_delim(cwd, '/')
		);
	} else {
		str_format_into(
			&destdir, "%s/%s",
			src, arg
		);
	}
	return destdir;
}

static str get_pkgsrc(package pkg) {
	str src_dir;
	if (pkg.is_local == 1) {
		str_format_into(
			&src_dir, "%s/%s",
			src, pkg.name
		);
	} else {
		str_format_into(
			&src_dir, "%s/%s/%s",
			src, pkg.name, pkg.version
		);
	}
	return src_dir;
}

package pkg_create(str_slc *arg) {
	package pkg;
	pkg.version = (str) {
		.data = "HEAD",
		.len = 4,
		.cap = 4,
	};
	pkg.is_local = false;
	char cwd[MAX_PATH_LEN];
	getcwd(cwd, MAX_PATH_LEN);
	str_slc cwd_slc = mstrslc(cwd);
	str cwd_str = mstr(cwd);
	str_slc new_arg;
	new_arg.data = arg->data;
	new_arg.len = arg->len;
	if (!new_arg.data) exit(EXIT_FAILURE);
	str dest_dir = get_destdir(cwd_slc, new_arg);
	bool is_installed_locally = is_directory(dest_dir.data);

	str new_arg_str = str_from_str_slc(new_arg);
	pkg.version = str_from_after_delim(&new_arg_str, '@');
	str_println(&pkg.version);
	pkg.target = str_from_after_delim(&new_arg_str, ',');
	str_println(&pkg.target);

	bool is_in_repos = false;
	//for (size_t i = 0; i < cached_repos_count; i++) {
	//	if (strcmp(new_arg, cached_repos[i].source_key) == 0) {
	//		is_in_repos = true;
	//		break;
	//	}
	//}

	if (strncmp(new_arg.data, "http", 4) == 0 || strncmp(new_arg.data, "ssh", 3) == 0) {
		pkg.url = new_arg_str;
		pkg.name = str_from_after_delim(&new_arg_str, '/');
	} else if (strcmp(new_arg.data, ".") == 0) {
		pkg.url = str_from_cstr("");
		pkg.name = str_from_after_delim(&cwd_str, '/');
		pkg.is_local = true;
	} else if (is_in_repos) {
	//for (size_t i = 0; i < cached_repos_count; i++) {
	//	if (strcmp(new_arg, cached_repos[i].source_key) == 0) {
	//		pkg.url = strdup(cached_repos[i].source_value);
	//		break;
	//	}
	//}
	pkg.name = new_arg_str;
	} else if (is_installed_locally) {
		pkg.url = str_from_cstr("");
		pkg.name = str_from_after_delim(&dest_dir, '/');
		pkg.is_local = true;
	} else {
		printf("%s '%.*s' is not a valid package\n", PRINT_ERROR, str_fmt(&new_arg));
		exit(EXIT_FAILURE);
	}
	pkg.src = get_pkgsrc(pkg);
	return pkg;
}
