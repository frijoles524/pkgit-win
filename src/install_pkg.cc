#include "fetch_src.cc"
#include "build_pkg.cc"
#include "link_install.cc"

void install_pkg(Pkg pkg) {
  fetch_src(pkg);
  build_pkg(pkg.src);
  link_install(pkg.src);
}
