#include "fetch_src.cc"
#include "build.cc"
#include "link_install.cc"

void install_pkg(Pkg pkg) {
  fetch_src(pkg);
  //build_pkg(pkg.src);
  build(pkg.src.c_str());
  link_install(pkg.src);
}
