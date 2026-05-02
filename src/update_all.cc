#include "update_all.hh"
#include "create_pkg.hh"
#include "lua_state.hh"
#include "update_pkg.hh"
#include "vars.hh"

void update_all() {
  for (auto repo : cached_repos) {
    Pkg pkg = create_pkg(repo.second.source.value);
    update_pkg(pkg);
  }
}
