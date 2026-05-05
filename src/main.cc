#include "cla_parse.hh"
#include "lua_state.hh"
#include "setup_pkgit.hh"

int main(int argc, char* argv[]) {
  setup_pkgit();
  cla_parse(argc, argv);
  free_lua_state();
  return 0;
}
