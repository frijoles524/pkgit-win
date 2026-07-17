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

#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "is_updated.h"

#include "files.h"
#include "str.h"
#include "pkgit_lua.h"

bool is_updated(str *src) {
	bool result = false;
	if (str_is_valid(src) && src->len > 0 && chdir(src->data) != 0) return result;
	str bldit_pkgver = bldit_pkg_getver();
	str git_tag = cmd_out("git tag | tail -n 1");
	result = (strstr(git_tag.data, bldit_pkgver.data) != NULL && bldit_pkgver.len != 0);
	if (result) {
		str_free(&bldit_pkgver);
		str_free(&git_tag);
		return result;
	}
	str git_pull = cmd_out("git pull");
	result = (strstr(git_pull.data, "Already up to date.") != NULL);
	str_free(&git_tag);
	str_free(&git_pull);
	str_free(&bldit_pkgver);
	return result;
}
