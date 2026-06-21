#include <stdio.h>

#include "cla_parse.h"
#include "lua_state.h"
#include "setup_pkgit.h"
#include "vars.h"

int main(int argc, char *argv[]) {
  init_vars();
  init_lua_state();
  setup_pkgit();
  cache_repos();
  cla_parse(argc, argv);
  free_lua_state();
  return 0;
}