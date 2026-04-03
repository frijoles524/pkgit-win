#include <string>
#include <cstring>
#include <filesystem>
#include <iostream>

#include "add_repo.hh"
#include "build.hh"
#include "create_pkg.hh"
#include "help.hh"
#include "install_pkg.hh"
#include "list_pkgs.hh"
#include "name_from_url.hh"
#include "remove_pkg.hh"
#include "setup_pkgit.hh"
#include "update_all.hh"
#include "vars.hh"

int main(int argc, char *argv[]) {
  setup_pkgit();
  Pkg pkg;

  if (!argv[1]) { help(); return 0; }

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--link") == 0 || strcmp(argv[i], "-l") == 0) {
      is_symlink_install = true;
    }
    if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
      is_verbose = true;
    }
  }

  if (strcmp(argv[1], "add") == 0 || strcmp(argv[1], "a") == 0) {
    if (argv[2]) {
      add_repo(argv[2], name_from_url(argv[2]));
    } else {
      std::cout << print_error << "Not enough arguments! Try: `pkgit add [url]`";
    }

  } else if (strcmp(argv[1], "build") == 0 || strcmp(argv[1], "b") == 0) {
    if (argv[2]) {
      build(argv[2]);
    } else {
      build(std::filesystem::current_path().string().c_str());
    }

  } else if (strcmp(argv[1], "install") == 0 || strcmp(argv[1], "i") == 0) {
    pkg = create_pkg(argv[2]);
    if (argv[2]) {
      install_pkg(pkg);
    } else {
      std::cout << print_error << "Not enough arguments! Try: `pkgit install [url/pkg]`";
    }

  } else if (strcmp(argv[1], "remove") == 0 || strcmp(argv[1], "r") == 0) {
    pkg = create_pkg(argv[2]);
    if (argv[2]) {
      remove_pkg(pkg);
    } else {
      std::cout << print_error << "Not enough arguments! Try: `pkgit remove [url/pkg]`";
    }

  } else if (strcmp(argv[1], "update") == 0 || strcmp(argv[1], "u") == 0) {
    update_all();

  } else if (strcmp(argv[1], "list") == 0 || strcmp(argv[1], "l") == 0) {
    list_pkgs();

  } else if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "h") == 0) {
    help();

  } else {
    help();
  }

  return 0;
}
