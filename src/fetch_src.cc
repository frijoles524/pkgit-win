#include <stdlib.h>
#include "fetch_git.cc"

void fetch_src(Pkg pkg) {
  if (fs::exists(pkg.src)) { fs::remove_all(pkg.src); }
  if (fetch_git(pkg) == 0) { return; } else { exit(EXIT_FAILURE); }
}
