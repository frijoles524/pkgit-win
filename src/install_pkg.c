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

#include "add_repo.h"
#include "build.h"
#include "fetch_src.h"
#include "files.h"
#include "install_pkg.h"
#include "lua_state.h"
#include "name_from_url.h"
#include "vars.h"

void install_pkg(Pkg pkg) {
  if (is_directory(pkg.src)) {
    if (!is_forced) {
      if (is_verbose) printf(
        "%s %s is already installed.\n",
        print_skipped, pkg.name
      );
      return;
    } else {
      if (is_verbose) printf(
        "%s %s is already installed.\n",
        print_warning, pkg.name
      );
    }
  }
  char cwd[MAX_PATH_LEN];
  getcwd(cwd, MAX_PATH_LEN);
  if (strcmp(pkg.src, cwd) != 0) chdir(pkg.src);

	if (pkg.is_local) {
    cpdir(cwd, pkg.src);
	} else {
    printf(
      "%s fetching %s%s%s\n",
      print_pkgit, green, pkg.name, color_reset
    );
    fetch_src(pkg);
    if (is_verbose)
      printf(
        "%s fetched %s%s%s\n",
        print_pkgit, green, pkg.name, color_reset
      );
  }

  printf(
    "%s building %s%s%s\n",
    print_pkgit, green, pkg.name, color_reset
  );
  build(pkg);
  if (is_verbose)
    printf(
      "%s built %s%s%s\n",
      print_pkgit, green, pkg.name, color_reset
    );

  printf(
    "%s installing %s%s%s\n",
    print_pkgit, green, pkg.name, color_reset
  );
  bool install_success = false;
  if (!install_success && repo_install(pkg.name, pkg.target))
    install_success = true;
  if (!install_success && bldit_install(pkg.target))
    install_success = true;
  if (!install_success && config_install(pkg.src, pkg.target))
    install_success = true;
  if (!install_success) {
    printf(
      "%s no install function availible for package: %s\n",
      print_error, pkg.name
    );
    return;
  }
  printf(
    "%s installed %s%s%s\n",
    print_success, green, pkg.name, color_reset
  );

  bool repo_exists = false;
  for (size_t i = 0; i < cached_repos_count; i++) {
    char *repo_name = name_from_url(cached_repos[i].source_key);
    if (strcmp(repo_name, pkg.name) == 0) { repo_exists = true; }
    free(repo_name);
  }

  if (!repo_exists) {
    printf(
      "%s adding %s%s%s\n",
      print_pkgit, green, pkg.name, color_reset
    );
    if (pkg.url && strlen(pkg.url) > 0) {
      add_repo(pkg.url, pkg.name);
    }
    printf(
      "%s added %s%s%s\n",
      print_pkgit, green, pkg.name, color_reset
    );
  } else {
    if (is_verbose)
      printf("%s repo already exists, done\n", print_pkgit);
  }
}