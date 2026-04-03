#include <cstring>

#include "fetch_git.hh"
#include "vars.hh"

int fetch_git(Pkg pkg) {
  std::string clone_cmds[] = {
    "git -c advice.detachedHead=false clone --depth=1 " + pkg.url +
      " " + pkg.src.c_str(),
    "git -c advice.detachedHead=false clone " + pkg.url +
      " " + pkg.src.c_str(),
    "git -c advice.detachedHead=false clone --branch " + pkg.ver +
      " --depth=1 " + pkg.url + " " + pkg.src.c_str(),
    "git -c advice.detachedHead=false clone --branch " + pkg.ver +
      " " + pkg.url + " " + pkg.src.c_str()
  };
  if (strcmp(pkg.ver.c_str(), "HEAD") == 0) {
    if (system(clone_cmds[0].c_str()) != 0) { system(clone_cmds[1].c_str()); }
  } else {
    if (system(clone_cmds[2].c_str()) != 0) { system(clone_cmds[3].c_str()); }
  }
  return 0;
}
