#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cla_parse.h"

#include "add_repo.h"
#include "build.h"
#include "create_pkg.h"
#include "declare.h"
#include "find.h"
#include "help.h"
#include "install_pkg.h"
#include "list_pkgs.h"
#include "name_from_url.h"
#include "remove_pkg.h"
#include "update_all.h"
#include "vars.h"

#include "resolve_deps.h"

#define COMMAND(large, small, code) \
    if (strcmp(argv[i], large) == 0 || strcmp(argv[i], small) == 0) \
    code

#define NOT_ENOUGH_ARGS(arg, next) \
    printf("%sNot enough arguments! Try: `pkgit %s [%s]`\n", print_error, arg, next)

void cla_parse(int argc, char **argv) {
    Pkg pkg = {0};

    if (!argv[1]) {
        help();
        return;
    }

    for (int i = 1; i < argc; i++) {
        COMMAND("--link", "-l", { is_symlink_install = true; });
        COMMAND("add", "a", {
            if (argv[i + 1]) {
                add_repo(argv[i + 1], name_from_url(argv[i + 1]));
            } else {
                NOT_ENOUGH_ARGS(argv[i], "url");
            }
        });
        COMMAND("build", "b", {
            if (argv[i + 1]) {
                if (argv[i + 2]) {
                    pkg = create_pkg(argv[i + 1], argv[i + 2]);
                    build(pkg);
                } else {
                    pkg = create_pkg(argv[i + 1], "default");
                    build(pkg);
                }
            } else {
                pkg = create_pkg(".", "default");
                build(pkg);
            }
        });
        COMMAND("install", "i", {
            if (argv[i + 1]) {
                if (argv[i + 2]) {
                    pkg = create_pkg(argv[i + 1], argv[i + 2]);
                    install_pkg(pkg);
                } else {
                    pkg = create_pkg(argv[i + 1], "default");
                    install_pkg(pkg);
                }
            } else {
                NOT_ENOUGH_ARGS(argv[i], "url/pkg");
            }
        });
        COMMAND("remove", "r", {
            if (argv[i + 1]) {
                pkg = create_pkg(argv[i + 1], "default");
                remove_pkg(pkg);
            } else {
                NOT_ENOUGH_ARGS(argv[i], "url/pkg");
            }
        });
        COMMAND("update", "u", { update_all(); });
        COMMAND("declare", "d", { declare(); });
        COMMAND("list", "l", { list_pkgs(); });
        COMMAND("find", "f", { find(); });
        COMMAND("--version", "-v", { printf("%s\n", version); });
        COMMAND("--help", "-h", { help(); });
        COMMAND("--check", "-c", { resolve_deps(); return; });
    }
}