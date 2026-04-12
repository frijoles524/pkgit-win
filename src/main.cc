#include <string>
#include <cstring>
#include <filesystem>
#include <iostream>

#include "cla_parse.hh"
#include "help.hh"
#include "setup_pkgit.hh"

int main(int argc, char *argv[]) {
  setup_pkgit();

  if (!argv[1]) { help(); return 0; }

  cla_parse(argc, argv);

  return 0;
}
