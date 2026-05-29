#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "install_pkg.h"
#include "add_repo.h"
#include "fetch_src.h"
#include "build.h"
#include "copy_install.h"
#include "link_install.h"
#include "lua_state.h"
#include "name_from_url.h"
#include "vars.h"

void install_pkg(Pkg pkg) {
    if (!pkg.is_local) {
        printf("%sfetching source...\n", print_pkgit);
        fetch_src(pkg);
        printf("%ssource fetched!\n", print_pkgit);
    }

    printf("%sbuilding...\n", print_pkgit);
    build(pkg);
    printf("%sbuild complete!\n", print_pkgit);

    printf("%sinstalling...\n", print_pkgit);
    if (is_auto_installed) {
      if (is_symlink_install) {
          link_install(pkg.src);
      } else {
          copy_install(pkg.src);
      }
    }
    printf("%s%s installed!\n", print_success, pkg.name);

    bool repo_exists = false;
    for (size_t i = 0; i < cached_repos_count; i++) {
        char *repo_name = name_from_url(cached_repos[i].source_value);
        if (strcmp(repo_name, pkg.name) == 0) {
            repo_exists = true;
        }
        free(repo_name);
    }

    if (!repo_exists) {
        printf("%sadding repository...\n", print_pkgit);
        if (pkg.url && strlen(pkg.url) > 0) {
            add_repo(pkg.url, pkg.name);
        }
        printf("%sdone!\n", print_pkgit);
    } else {
        printf("%srepo already exists, done!\n", print_pkgit);
    }
}