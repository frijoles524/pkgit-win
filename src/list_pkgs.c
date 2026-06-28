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

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "lua_state.h"
#include "vars.h"

void list_pkgs(void) {
  char* src_code = map_get(&cached_install_directories, "src");
  struct dirent* dirent_ptr;
  DIR* dir_ptr = opendir(src_code);

  if (dir_ptr == NULL) {
    fprintf(
      stderr,
      "%s could not open %s\n",
      print_pkgit, src_code
    );
    return;
  }

  while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
    if (
      strcmp(dirent_ptr->d_name, "..") == 0 ||
      strcmp(dirent_ptr->d_name, ".") == 0
    ) continue;
    printf("%s\n", dirent_ptr->d_name);
  }
}