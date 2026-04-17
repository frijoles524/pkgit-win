#include <string>
#include <cstring>
#include <filesystem>
#include <iostream>

#include "cla_parse.hh"

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

void cla_parse(int argc, char** argv) {
  Pkg pkg;

  if (!argv[1]) { help(); return; }

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--link") == 0 || strcmp(argv[i], "-l") == 0) {
      is_symlink_install = true;
    }

    if (strcmp(argv[i], "add") == 0 || strcmp(argv[i], "a") == 0) {
      for (int j = i+1; i < argc; j++) {
        if (argv[j]) {
          add_repo(argv[j], name_from_url(argv[j]));
          return;
        } else {
          std::cout << print_error << "Not enough arguments! Try: `pkgit add [url]`";
          return;
        }
      }

    } else if (strcmp(argv[i], "build") == 0 || strcmp(argv[i], "b") == 0) {
      for (int j = i+1; i < argc; j++) {
        if (argv[j]) {
          build(argv[j]);
          return;
        } else {
          build(std::filesystem::current_path().string().c_str());
          return;
        }
      }

    } else if (strcmp(argv[i], "install") == 0 || strcmp(argv[i], "i") == 0) {
      for (int j = i+1; i < argc; j++) {
        pkg = create_pkg(argv[j]);
        if (argv[j]) {
          install_pkg(pkg);
          return;
        } else {
          std::cout << print_error << "Not enough arguments! Try: `pkgit install [url/pkg]`";
          return;
        }
      }

    } else if (strcmp(argv[i], "remove") == 0 || strcmp(argv[i], "r") == 0) {
      for (int j = i+1; i < argc; j++) {
        pkg = create_pkg(argv[j]);
        if (argv[j]) {
          remove_pkg(pkg);
          return;
        } else {
          std::cout << print_error << "Not enough arguments! Try: `pkgit remove [url/pkg]`";
          return;
        }
      }

    } else if (strcmp(argv[i], "update") == 0 || strcmp(argv[i], "u") == 0) {
      update_all();
      return;

    } else if (strcmp(argv[i], "list") == 0 || strcmp(argv[i], "l") == 0) {
      list_pkgs();
      return;

    } else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
      std::cout << version << std::endl;
      return;

    } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      help();
      return;

    } else {
      help();
      return;
    }
  }
}
