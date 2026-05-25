#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "setup_dirs.h"
#include "vars.h"

void setup_dirs() {
  mkdir_p(config_dir);
  for (int i = 0; i < 5; i++) {
    if (install_directories[i] && strlen(install_directories[i]) > 0) {
      if (!file_exists(install_directories[i])) {
        mkdir_p(install_directories[i]);
      }
    }
  }
}