#include <stdio.h>

#include "update_pkg.h"
#include "is_updated.h"
#include "install_pkg.h"

void update_pkg(Pkg pkg) {
    if (is_updated(pkg.src)) {
        printf("%s %s%s%s is already up to date.\n", print_skipped, green, pkg.name, color_reset);
        return;
    }
    printf("%s Updating package: %s%s%s\n", print_pkgit, green, pkg.name, color_reset);
    install_pkg(pkg);
}