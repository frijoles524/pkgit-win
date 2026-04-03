#include <stdlib.h>
#include "fetch_src.hh"
#include "fetch_git.hh"

void fetch_src(Pkg pkg) {
  if (std::filesystem::exists(pkg.src)) { std::filesystem::remove_all(pkg.src); }

  if (pkg.url == "") { std::filesystem::create_directories(pkg.src); return; }
  else if (fetch_git(pkg) == 0) { return; } else { exit(EXIT_FAILURE); }
}
