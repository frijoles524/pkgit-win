
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

#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "fetch_git.h"
#include "fetch_src.h"
#include "vars.h"

static int remove_tree(
  const char *fpath, const struct stat *sb,
  int typeflag, struct FTW *ftwbuf
) {
  (void)sb;
  (void)ftwbuf;
  if (typeflag == FTW_F || typeflag == FTW_SL) {
    unlink(fpath);
  } else if (typeflag == FTW_DP) {
    rmdir(fpath);
  }
  return 0;
}

void fetch_src(Pkg pkg) {
  if (is_verbose)
    printf("%s target source directory: %s\n", print_pkgit, pkg.src);
  if (file_exists(pkg.src)) {
    if (is_verbose)
      printf("%s %s already exists. deleting...\n", print_pkgit, pkg.src);
    nftw(pkg.src, remove_tree, 64, FTW_DEPTH | FTW_PHYS);
  }
  if (strcmp(pkg.url, "") == 0) {
    if (is_verbose)
      printf("%s creating directory %s...\n", print_pkgit, pkg.src);
    mkdir_p(pkg.src);
    return;
  }
  if (fetch_git(pkg) == 0) {
    if (is_verbose)
      printf("%s cloned into %s\n", print_pkgit, pkg.src);
    return;
  }
  printf("%s no fetch methods worked.\n", print_error);
  exit(EXIT_FAILURE);
}