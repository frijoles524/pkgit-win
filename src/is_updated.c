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
#include <stdbool.h>
#include <unistd.h>

#include "is_updated.h"

#include "cmd_out.h"
#include "lua_state.h"

bool is_updated(const char *src) {
  bool result = false;
  if (src && strlen(src) > 0 && chdir(src) != 0) return result;
  char *bldit_pkgver = bldit_pkg_getver();
  char *git_tag = cmd_out("git tag | tail -n 1");
  result = (strstr(git_tag, bldit_pkgver) != NULL);
  char *git_pull = cmd_out("git pull");
  result = (strstr(git_pull, "Already up to date.") != NULL);
  free(git_tag);
  free(git_pull);
  return result;
}

bool is_bldit_usable(void) {
  char* bldit_version = bldit_getver();
  if (!bldit_version) return false;
  if (strcmp(bldit_version, version) == 0) return true;
  bool prev_pass = false;
  for (size_t i = 0; i < strlen(bldit_version); i++) {
    if (bldit_version[i] == '.') continue;
    if ((bldit_version[i] - '0') <= (version[i] - '0')) {
      prev_pass = ((bldit_version[i] - '0') != (version[i] - '0'));
      continue;
    } else return prev_pass;
  }
  return true;
}