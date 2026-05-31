#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "build.h"
#include "lua_build.h"
#include "vars.h"

void build(Pkg pkg) {
    char original_dir[MAX_PATH_LEN];
    getcwd(original_dir, MAX_PATH_LEN);

    if (strcmp(pkg.src, original_dir) != 0) {
        chdir(pkg.src);
    }

    if (lua_build(pkg.name, pkg.target, pkg.src)) {
        return;
    }

    printf("%s no usable build system was found\n", print_error);
    exit(EXIT_FAILURE);
}