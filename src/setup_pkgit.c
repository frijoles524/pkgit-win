#include <stdio.h>
#include <unistd.h>

#include "set_install_directories.h"
#include "setup_dirs.h"

void setup_pkgit() {
    set_install_directories();
    setup_dirs();
}