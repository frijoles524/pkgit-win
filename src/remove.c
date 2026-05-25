#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "files.h"
#include "vars.h"

//void remove_pkg(Pkg pkg) {
//  char* src_code = pkg.src;
//  struct dirent* dirent_ptr;
//  DIR* dir_ptr;
//
//  if ((dir_ptr = opendir(src_code)) == NULL) {
//    fprintf(stderr, "%scould not open %s\n", print_pkgit, src_code);
//  }
//
//  while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
//    struct stat stat_buf;
//    FILE* file_ptr = fopen(dirent_ptr->d_name, "r");
//    if (!file_ptr) { continue; }
//    if (strcmp(get_filename_ext(dirent_ptr->d_name), ".so") == 0) {
//    remove(strcat(map_get(&cached_install_directories, "lib"), strcat("/", dirent_ptr->d_name)));
//    } else if (!access(dirent_ptr->d_name, X_OK) && stat_buf.st_mode != S_IFDIR) {
//    remove(strcat(map_get(&cached_install_directories, "bin"), strcat("/", dirent_ptr->d_name)));
//    } else if (strcmp(get_filename_ext(dirent_ptr->d_name), ".h") == 0) {
//    remove(strcat(map_get(&cached_install_directories, "include"), strcat("/", dirent_ptr->d_name)));
//    }
//    }
//  }
//}
