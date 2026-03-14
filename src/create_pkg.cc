#include <string>
#include <filesystem>

#include "vars.cc"
#include "name_from_url.cc"

Pkg create_pkg(std::string arg) {
  Pkg pkg;

  if (arg.rfind("http", 0) == 0) {
    pkg.url = arg;
    pkg.name = name_from_url(arg);
  } else if (arg == ".") {
    pkg.url = "";
    pkg.name = name_from_url(fs::current_path().string());
  }

  pkg.ver = "HEAD";

  pkg.src = pkgblds + "/" + pkg.name + "/" + pkg.ver;

  return pkg;
}
