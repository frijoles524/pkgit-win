#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

#include "cla_parse.hh"

#include "add_repo.hh"
#include "build.hh"
#include "create_pkg.hh"
#include "help.hh"
#include "install_pkg.hh"
#include "list_pkgs.hh"
#include "name_from_url.hh"
#include "remove_pkg.hh"
#include "update_all.hh"
#include "vars.hh"

#define COMMAND(large, small, code)                                            \
  if (strcmp(argv[i], large) == 0 || strcmp(argv[i], small) == 0)              \
  code
#define NOT_ENOUGH_ARGS(arg, next)                                             \
  std::cout << print_error << "Not enough arguments! Try: `pkgit " << arg      \
	    << " [" << next << "]`"

void cla_parse(int argc, char **argv) {
  Pkg pkg;

  if (!argv[1]) {
    help();
    return;
  }

  for (int i = 1; i < argc; i++) {
    COMMAND("--large", "-l", { is_symlink_install = true; });
    COMMAND("add", "a", {
      if (argv[i + 1]) {
        add_repo(argv[i + 1], name_from_url(argv[i + 1]));
      } else {
        NOT_ENOUGH_ARGS(argv[i], "url");
      }
    });
    COMMAND("build", "b", {
      if (argv[i + 1]) {
        if (argv[i + 2]) {
          pkg = create_pkg(argv[i + 1], argv[i + 2]);
          build(pkg);
        } else {
          pkg = create_pkg(argv[i + 1], "default");
          build(pkg);
        }
      } else {
        pkg = create_pkg(".", "default");
        build(pkg);
      }
    });
    COMMAND("install", "i", {
      if (argv[i + 1]) {
        if (argv[i + 2]) {
          pkg = create_pkg(argv[i + 1], argv[i + 2]);
          install_pkg(pkg);
        } else {
          pkg = create_pkg(argv[i + 1], "default");
          install_pkg(pkg);
        }
      } else {
        NOT_ENOUGH_ARGS(argv[i], "url/pkg");
      }
    });
    COMMAND("remove", "r", {
      pkg = create_pkg(argv[i + 1]);
      if (argv[i + 1]) {
        remove_pkg(pkg);
      } else {
        NOT_ENOUGH_ARGS(argv[i], "url/pkg");
      }
    });
    COMMAND("update", "u", { update_all(); });
    COMMAND("list", "l", { list_pkgs(); });
    COMMAND("--version", "-v", { std::cout << version << std::endl; });
    COMMAND("--help", "-h", { help(); });
  }
}
