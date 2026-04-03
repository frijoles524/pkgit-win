#include <iostream>
#include <string>
#include <filesystem>

#include "create_pkg.hh"
#include "name_from_url.hh"
#include "vars.hh"

Pkg create_pkg(std::string arg) {
  Pkg pkg;
  bool is_in_repos = false;

  for (auto repo : repos) {
    if (arg == repo.first) { is_in_repos = true; }
  }

  if (arg.rfind("http", 0) == 0) {
    pkg.url = arg;
    pkg.name = name_from_url(arg);
  } else if (arg == ".") {
    pkg.url = "";
    pkg.name = name_from_url(std::filesystem::current_path().string());
  } else if (is_in_repos) {
    pkg.url = repos[arg];
    pkg.name = arg;
  } else {
    std::cout << print_error << "'" << arg << "'" << " is not a valid package";
    exit(1);
  }

  pkg.ver = "HEAD";

  pkg.src = pkgblds + "/" + pkg.name + "/" + pkg.ver;

  return pkg;
}
