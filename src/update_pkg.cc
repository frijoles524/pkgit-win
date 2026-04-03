#include <filesystem>
#include <iostream>

#include "update_pkg.hh"
#include "is_updated.hh"
#include "install_pkg.hh"

void update_pkg(Pkg pkg) {
  if (is_updated(pkg.src)) { std::cout << print_skipped << pkg.name << " is already up to date."; return; }
  install_pkg(pkg);
}
