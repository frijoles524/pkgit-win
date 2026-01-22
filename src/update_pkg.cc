#include "is_updated.cc"
#include "build_pkg.cc"
#include "link_install.cc"
#include "vars.cc"

void update_pkg(Pkg pkg) {
  if (is_updated(pkg.src)) {
    build_pkg(pkg.src);
    link_install(pkg.src);
  }
  else { std::cout << print_skipped << pkg.name << " is already up to date."; }
}
