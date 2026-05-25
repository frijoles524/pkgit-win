#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "lua_state.h"
#include "vars.h"

void find() {
  char* src_code = map_get(&cached_install_directories, "src");
  struct dirent* dirent_ptr;
  DIR* dir_ptr = opendir(src_code);

  if (dir_ptr == NULL) {
    fprintf(stderr, "%scould not open %s\n", print_pkgit, src_code);
    return;
  }

  while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
    if (strcmp(dirent_ptr->d_name, "..") == 0 || strcmp(dirent_ptr->d_name, ".") == 0) { continue; }
    printf("%s\n", dirent_ptr->d_name);
  }
}
