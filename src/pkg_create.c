/*

  pkgit - package_t it!

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

#include <string.h>
#include <unistd.h>

#include "pkg.h"

#include "files.h"
#include "globs.h"
#include "log.h"
#include "str.h"

static str get_destdir(str_slc cwd, str_slc arg) {
	str_slc name = str_slc_from_after_delim(arg, '/');
	if (str_slc_first(arg) == '.')
		return str_format("%.*s/%.*s", str_fmt(&inst_dirs.src), str_fmt(&cwd));
	else
		return str_format("%.*s/%.*s", str_fmt(&inst_dirs.src), str_fmt(&name));
}

static str get_pkgsrc(package_t pkg) {
	if (pkg.is_local == 1)
		return str_format("%.*s/%.*s", str_fmt(&inst_dirs.src), pkg.name);
	else
		return str_format("%.*s/%.*s/%.*s", str_fmt(&inst_dirs.src),
			str_fmt(&pkg.name), str_fmt(&pkg.version));
}

static void assign_pkg_version(package_t *pkg, str *new_arg_str) {
	if (str_find_char(new_arg_str, '@')) {
		str tmp_arg = str_from_after_delim(new_arg_str, '@');
		if (str_find_char(new_arg_str, ',')) {
			str tmp_trg = str_from_after_delim(&tmp_arg, ',');
			pkg->version = str_from_after_delim(new_arg_str, '@');
			pkg->version.len -= tmp_trg.len;
			str_free(&tmp_trg);
		} else {
			pkg->version = str_from_after_delim(new_arg_str, '@');
		}
		if (str_find_char(&pkg->version, ',')) pkg->version.len--;
		str_free(&tmp_arg);
		pkg->name.len -= pkg->version.len + 1;
	} else pkg->version = mstr("HEAD");
}

static void assign_pkg_target(package_t *pkg, str *new_arg_str) {
	if (str_find_char(new_arg_str, ',')) {
		str tmp_arg = str_from_after_delim(new_arg_str, ',');
		if (str_find_char(new_arg_str, '@')) {
			str tmp_ver = str_from_after_delim(&tmp_arg, '@');
			pkg->target = str_from_after_delim(new_arg_str, ',');
			pkg->target.len -= tmp_ver.len;
			str_free(&tmp_ver);
		} else {
			pkg->target = str_from_after_delim(new_arg_str, ',');
		}
		if (str_find_char(&pkg->target, '@')) pkg->target.len--;
		str_free(&tmp_arg);
		pkg->name.len -= pkg->target.len + 1;
	} else pkg->target = mstr("default");
}

package_t pkg_create(str_slc arg) {
	package_t pkg = { .is_local = false, };

	char cwd[MAX_PATH_LEN];
	getcwd(cwd, MAX_PATH_LEN);

	str_slc cwd_slc = mstrslc(cwd);
	str cwd_str = mstr(cwd);
	str dest_dir = get_destdir(cwd_slc, arg);
	str new_arg_str = str_from_str_slc(arg);

	bool is_installed_locally = is_directory(dest_dir.data);

	bool is_in_repos = false;
	// for (size_t i = 0; i < cached_repos_count; i++) {
	//	if (strcmp(new_arg, cached_repos[i].source_key) == 0) {
	//		is_in_repos = true;
	//		break;
	//	}
	// }

	assign_pkg_version(&pkg, &new_arg_str);
	assign_pkg_target(&pkg, &new_arg_str);
	if (str_find_char(&new_arg_str, '@')) {
		str tmp = str_from_after_delim(&new_arg_str, '@');
		new_arg_str.len -= tmp.len + 1;
		str_free(&tmp);
	}
	if (str_find_char(&new_arg_str, ',')) {
		str tmp = str_from_after_delim(&new_arg_str, ',');
		new_arg_str.len -= tmp.len + 1;
		str_free(&tmp);
	}
	if (strncmp(arg.data, "http", 4) == 0 || strncmp(arg.data, "ssh", 3) == 0) {
		pkg.url = new_arg_str;
		pkg.name = str_from_after_delim(&new_arg_str, '/');
	} else if (str_slc_equal_cstr(arg, ".")) {
		pkg.url = mstr("");
		pkg.name = str_from_after_delim(&cwd_str, '/');
		pkg.is_local = true;
	} else if (is_in_repos) {
		// for (size_t i = 0; i < cached_repos_count; i++) {
		//	if (strcmp(new_arg, cached_repos[i].source_key) == 0) {
		//		pkg.url = strdup(cached_repos[i].source_value);
		//		break;
		//	}
		// }
		pkg.name = new_arg_str;
	} else if (is_installed_locally) {
		pkg.url = mstr("");
		pkg.name = str_from_after_delim(&dest_dir, '/');
		pkg.is_local = true;
	} else {
		log_error("'%.*s' is not a valid package", str_fmt(&arg));
		exit(EXIT_FAILURE);
	}
	if (str_find_char(&new_arg_str, ',')
		&& str_find_char(&new_arg_str, '@')
	) pkg.name.len--;
	pkg.src = get_pkgsrc(pkg);
	str_free(&cwd_str);
	str_free(&dest_dir);
	return pkg;
}
