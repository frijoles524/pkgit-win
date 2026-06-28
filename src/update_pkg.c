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
#include <string.h>

#include "update_pkg.h"

#include "install_pkg.h"
#include "is_updated.h"
#include "lua_state.h"

void update_pkg(Pkg pkg) {
	if (is_updated(pkg.src)) {
		printf(
			"%s %s%s%s is already up to date.\n",
			print_skipped, green, pkg.name, color_reset
		);
		return;
	}
	printf(
		"%s Updating package: %s%s%s\n",
		print_pkgit, green, pkg.name, color_reset
	);
	install_pkg(pkg);
}