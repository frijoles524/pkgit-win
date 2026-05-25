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
//      remove(strcat(install_directories[], dirent_ptr->d_name));
//    } else if (!access(dir_entry.path().c_str(), X_OK) && !is_directory(dir_entry.path())) {
//      std::filesystem::remove(install_directories["bin"]+"/"+dir_entry.path().filename().string());
//    } else if (dir_entry.path().extension() == ".h") {
//      std::filesystem::remove(install_directories["include"]+"/"+dir_entry.path().filename().string());
//    }
//  }
//}
