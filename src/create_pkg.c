#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "create_pkg.h"
#include "lua_state.h"
#include "name_from_url.h"
#include "vars.h"

Pkg create_pkg(const char *arg, const char *target) {
    Pkg pkg = {0};
    pkg.target = target;
    pkg.ver = "HEAD";
    pkg.is_local = false;

    init_lua_state();
    cache_repos();

    bool is_in_repos = false;
    for (size_t i = 0; i < cached_repos_count; i++) {
        if (strcmp(arg, cached_repos[i].source_key) == 0) {
            is_in_repos = true;
            break;
        }
    }

    if (strncmp(arg, "http", 4) == 0) {
        pkg.url = strdup(arg);
        pkg.name = name_from_url(arg);
    } else if (strcmp(arg, ".") == 0) {
        pkg.url = "";
        getcwd(pkg.src, MAX_PATH_LEN);
        pkg.name = name_from_url(pkg.src);
        pkg.is_local = true;
    } else if (is_in_repos) {
        for (size_t i = 0; i < cached_repos_count; i++) {
            if (strcmp(arg, cached_repos[i].source_key) == 0) {
                pkg.url = strdup(cached_repos[i].source_value);
                break;
            }
        }
        pkg.name = strdup(arg);
    } else {
        printf("%s'%s' is not a valid package\n", print_error, arg);
        exit(1);
    }

    cache_install_directories();

    if (!pkg.is_local) {
        char src_dir[MAX_PATH_LEN];
        snprintf(src_dir, sizeof(src_dir), "%s/%s/%s", get_install_dir("src"), pkg.name, pkg.ver);
        snprintf(pkg.src, MAX_PATH_LEN, "%s", src_dir);
    }

    return pkg;
}