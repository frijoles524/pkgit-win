#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "update_all.h"
#include "create_pkg.h"
#include "update_pkg.h"
#include "lua_state.h"
#include "vars.h"

void update_all() {
  init_lua_state();
  cache_repos();
  struct dirent* dirent_ptr;
  DIR* dir_ptr;
  if ((dir_ptr = opendir(get_install_dir("src"))) == NULL) {
    fprintf(stderr, "%s could not open %s\n", print_pkgit, get_install_dir("src"));
  }
  while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
    if (strcmp(dirent_ptr->d_name, "..") == 0 || strcmp(dirent_ptr->d_name, ".") == 0) continue;
    for (size_t i = 0; i < cached_repos_count; i++) {
      if (!strcmp(dirent_ptr->d_name, cached_repos[i].source_key) == 0) continue;
      Pkg pkg = create_pkg(cached_repos[i].source_value);
      update_pkg(pkg);
    }
  }
  closedir(dir_ptr);
}