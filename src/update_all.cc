#include "update_all.hh"
#include "create_pkg.hh"
#include "update_pkg.hh"
#include "vars.hh"

void update_all() {
  for (auto repo : repos) {
    Pkg pkg = create_pkg(repo.second);
    update_pkg(pkg);
  }
}
