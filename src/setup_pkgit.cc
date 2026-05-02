#include <iostream>

#include "set_install_directories.hh"
#include "setup_pkgit.hh"
#include "setup_dirs.hh"
#include "vars.hh"

void setup_pkgit() {
  set_install_directories();
  setup_dirs();
}
