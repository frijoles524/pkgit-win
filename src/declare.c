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
#include <stdlib.h>
#include <string.h>

#include "declare.h"
#include "create_pkg.h"
#include "update_pkg.h"
#include "lua_state.h"

void declare(void) {
  init_lua_state();
  cache_repos();
  for (size_t i = 0; i < cached_repos_count; i++) {
    Pkg pkg = create_pkg(cached_repos[i].source_value);
    update_pkg(pkg);
  }
}
