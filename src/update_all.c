#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "update_all.h"
#include "files.h"
#include "create_pkg.h"
#include "update_pkg.h"
#include "lua_state.h"
#include "set_install_directories.h"
#include "vars.h"

void update_all() {
  init_lua_state();
  cache_repos();
  struct dirent* dirent_ptr;
  DIR* dir_ptr;
  if ((dir_ptr = opendir(src)) == NULL) {
    fprintf(stderr, "%scould not open %s\n", print_pkgit, src);
  }
  while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
    if (strcmp(dirent_ptr->d_name, "..") == 0 || strcmp(dirent_ptr->d_name, ".") == 0) continue;
    struct stat stat_buf;
    FILE* file_ptr = fopen(dirent_ptr->d_name, "r");
    if (!file_ptr) { continue; }
    Pkg pkg = create_pkg(dirent_ptr->d_name, "default");
    update_pkg(pkg);
  }
  closedir(dir_ptr);
}