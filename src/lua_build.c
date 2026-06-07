#include <stdio.h>
#include <stdbool.h>

#include "lua_build.h"
#include "lua_state.h"
#include "vars.h"

bool lua_build(const char *repository, const char *target, const char *path) {
    if (is_verbose) printf(
      "%sattempting to use build function specified in 'repositories.%s'...\n",
      print_pkgit, repository
    );

    if (repo_build(repository)) { return true; }

    if (is_verbose) printf("%sattempting to use build function specified in 'bldit.lua'...\n", print_pkgit);
    if (bldit(target)) { return true; }

    if (is_verbose) printf("%sattempting to use build functions specified in 'build_systems'...\n", print_pkgit);
    if (config_build(path)) { return true; }

    return false;
}