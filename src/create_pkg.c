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

#include "create_pkg.h"
#include "lua_state.h"
#include "name_from_url.h"
#include "vars.h"

bool is_in_repos(char *arg) {
  for (size_t i = 0; i < cached_repos_count; i++)
    if (strcmp(arg, cached_repos[i].source_key) == 0) return true;
  return false;
}

Pkg pkg_from_repo(char* arg) {
  Pkg pkg = {0};
  for (size_t i = 0; i < cached_repos_count; i++) {
    if (strcmp(arg, cached_repos[i].source_key) == 0) {
      pkg.url = strdup(cached_repos[i].source_value);
      break;
    }
  }
  pkg.name = arg;
  return pkg;
}

char* get_destdir(char* cwd, char* arg) {
  char dest_dir[MAX_PATH_LEN];
  if (arg[0] == '.') {
	  snprintf(dest_dir, sizeof(dest_dir), "%s/%s",
	    get_install_dir("src"), name_from_url(cwd));
  } else {
	  snprintf(dest_dir, sizeof(dest_dir), "%s/%s",
	    get_install_dir("src"), arg);
  }
  return strdup(dest_dir);
}

void rmdotgit(Pkg pkg) {
  if (strlen(pkg.name) > 4 &&
  strncmp(pkg.name + strlen(pkg.name) - 4, ".git", 4) == 0)
    pkg.name[strlen(pkg.name) - 4] = '\0';
}

char* get_pkgsrc(Pkg pkg) {
  char src_dir[MAX_PATH_LEN];
  if (pkg.is_local) {
	  snprintf(src_dir, sizeof(src_dir), "%s/%s",
	    get_install_dir("src"), pkg.name);
  } else {
	  snprintf(src_dir, sizeof(src_dir), "%s/%s/%s",
	    get_install_dir("src"), pkg.name, pkg.ver);
  }
  return strdup(src_dir);
}

Pkg create_pkg(const char *arg) {
  Pkg pkg = {0};
  pkg.ver = "HEAD";
  pkg.is_local = false;
	char cwd[MAX_PATH_LEN];
	getcwd(cwd, MAX_PATH_LEN);
  char *new_arg = strdup(arg);
  if (!new_arg) exit(EXIT_FAILURE);
  char* dest_dir = get_destdir(cwd, new_arg);
  bool is_installed_locally = false;
  if (is_directory(dest_dir)) is_installed_locally = true;

  char *verptr = strchr(new_arg, '@');
  char *trgptr = strchr(new_arg, ',');
  for (size_t i = 0; i < cached_repos_count; i++) {
    if (strcmp(cached_repos[i].source_key, new_arg) != 0) continue;
    pkg.ver = cached_repos[i].version;
  }
  if (verptr) {
    pkg.ver = verptr + 1;
    *verptr = '\0';
  }
  if (trgptr) {
    pkg.target = trgptr + 1;
    *trgptr = '\0';
  } else {
    pkg.target = is_verbose ? "default" : "quiet";
  }

  bool is_in_repos = false;
  for (size_t i = 0; i < cached_repos_count; i++) {
    if (strcmp(new_arg, cached_repos[i].source_key) == 0) {
      is_in_repos = true;
      break;
    }
  }

  if (strncmp(new_arg, "http", 4) == 0) {
    pkg.url = strdup(new_arg);
    pkg.name = name_from_url(new_arg);
  } else if (strcmp(new_arg, ".") == 0) {
    pkg.url = "";
    pkg.name = name_from_url(cwd);
    pkg.is_local = true;
  } else if (is_in_repos) {
    for (size_t i = 0; i < cached_repos_count; i++) {
      if (strcmp(new_arg, cached_repos[i].source_key) == 0) {
        pkg.url = strdup(cached_repos[i].source_value);
        break;
      }
    }
    pkg.name = new_arg;
  } else if (is_installed_locally) {
    pkg.url = "";
    pkg.name = name_from_url(dest_dir);
    pkg.is_local = true;
  } else {
    printf("%s '%s' is not a valid package\n", print_error, new_arg);
    exit(EXIT_FAILURE);
  }
  rmdotgit(pkg);
  snprintf(pkg.src, MAX_PATH_LEN, "%s", get_pkgsrc(pkg));
  return pkg;
}