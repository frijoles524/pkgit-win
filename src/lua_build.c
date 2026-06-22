#include <stdio.h>
#include <stdbool.h>

#include "lua_build.h"

#include "is_updated.h"
#include "lua_state.h"
#include "vars.h"

bool lua_build(const char *repository, const char *target, const char *path) {
  if (is_verbose) printf(
    "%s attempting init.lua: 'repositories.%s.build'\n",
    print_pkgit, repository
  );
  if (repo_build(repository, target)) { return true; }
  if (is_verbose) printf(
    "%s failed init.lua: 'repositories.%s.build'\n",
    print_warning, repository
  );

  if (is_verbose) printf(
    "%s attempting bldit.lua\n",
    print_pkgit
  );
  if (bldit(target)) { return true; }
  if (is_verbose) printf(
    "%s failed bldit.lua\n",
    print_warning
  );

  if (is_verbose) printf(
    "%s attempting init.lua: 'build_systems'\n",
    print_pkgit
  );
  if (config_build(path, target)) { return true; }
  if (is_verbose) printf(
    "%s failed init.lua: 'build_systems'\n",
    print_warning
  );

  return false;
}