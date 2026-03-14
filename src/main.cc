#include <string>
#include <cstring>
#include <filesystem>

#include "create_pkg.cc"
#include "help.cc"
#include "setup_pkgit.cc"
#include "install_pkg.cc"

int main(int argc, char *argv[]) {
  setup_pkgit();
  Pkg pkg;

  if (argv[1]) {
    if (strcmp(argv[1], "build") == 0 || strcmp(argv[1], "b") == 0) {
      if (argv[2]) {
        build(argv[2]);
      } else {
        build(fs::current_path().string().c_str());
      }
    } else if (strcmp(argv[1], "install") == 0 || strcmp(argv[1], "i") == 0) {
      pkg = create_pkg(argv[2]);
      if (argv[2]) {
        install_pkg(pkg);
      } else {
        std::cout << print_error << "Not enough arguments! Try: `pkgit install [url]`";
      }
    } else if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "h") == 0) {
      help();
    } else if (strcmp(argv[1], "type") == 0) {
      link_install(fs::current_path().string());
    } else {
      help();
    }
  } else {
    help();
  }

  return 0;
}
