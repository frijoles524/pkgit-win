#include <stdio.h>
#include <string.h>

#include "update_pkg.h"
#include "is_updated.h"
#include "install_pkg.h"

void update_pkg(Pkg pkg) {
    if (is_updated(pkg.src)) {
        printf("%s%s is already up to date.\n", print_skipped, pkg.name);
        return;
    }
    install_pkg(pkg);
}