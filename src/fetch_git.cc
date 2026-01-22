#include <cstring>

#include "vars.cc"

int fetch_git(Pkg pkg) {
  std::string clone_cmds[] = {
      "git -c advice.detachedHead=false clone --depth 1 " + pkg.url + " " +
          pkg.src.c_str(),
      "git -c advice.detachedHead=false clone --branch " + pkg.ver +
          " --depth 1 " + pkg.url + " " + pkg.src.c_str()};
  if (strcmp(pkg.ver.c_str(), "HEAD") == 0) {
    if (WEXITSTATUS(system(clone_cmds[0].c_str())) == 0x10) {
      return 0;
    } else {
      return 1;
    }
  } else {
    if (WEXITSTATUS(system(clone_cmds[1].c_str())) == 0x10) {
      return 0;
    } else {
      return 1;
    }
  }
}
