#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "set_install_directories.h"
#include "setup_dirs.h"
#include "vars.h"

void setup_pkgit() {
    set_install_directories();
    setup_dirs();
}