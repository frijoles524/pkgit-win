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
#include <unistd.h>

#include "build.h"

#include "is_updated.h"
#include "lua_build.h"
#include "vars.h"

void build(Pkg pkg) {
  char cwd[MAX_PATH_LEN];
  getcwd(cwd, MAX_PATH_LEN);
  if (strcmp(pkg.src, cwd) != 0 && !pkg.is_local) chdir(pkg.src);

  if (lua_build(pkg.name, pkg.target, pkg.is_local ? cwd : pkg.src)) return;
  printf("%s no usable build system was found\n", print_error);
  exit(EXIT_FAILURE);
}