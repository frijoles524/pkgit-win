#include <string>
#include <cstring>
#include <filesystem>
#include <iostream>

#include "cla_parse.hh"
#include "setup_pkgit.hh"

int main(int argc, char *argv[]) {
  setup_pkgit();
  cla_parse(argc, argv);
  return 0;
}
