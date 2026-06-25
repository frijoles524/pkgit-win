
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
#include <stdbool.h>

#include "lua_build.h"

#include "is_updated.h"
#include "lua_state.h"
#include "vars.h"

bool lua_build(const char *repository, const char *target, const char *path) {
  if (is_verbose) printf(
    "%s attempting init.lua: 'repositories.%s.build'\n",
    print_pkgit, repository
  );
  if (repo_build(repository, target)) { return true; }
  if (is_verbose) printf(
    "%s failed init.lua: 'repositories.%s.build'\n",
    print_warning, repository
  );

  if (is_verbose) printf(
    "%s attempting bldit.lua\n",
    print_pkgit
  );
  if (bldit(target)) { return true; }
  if (is_verbose) printf(
    "%s failed bldit.lua\n",
    print_warning
  );

  if (is_verbose) printf(
    "%s attempting init.lua: 'build_systems'\n",
    print_pkgit
  );
  if (config_build(path, target)) { return true; }
  if (is_verbose) printf(
    "%s failed init.lua: 'build_systems'\n",
    print_warning
  );

  return false;
}