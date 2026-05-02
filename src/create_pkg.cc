#include <iostream>
#include <string>
#include <filesystem>

#include "create_pkg.hh"
#include "lua_state.hh"
#include "name_from_url.hh"
#include "vars.hh"

Pkg create_pkg(std::string arg, const char* target) {
  Pkg pkg;
  pkg.target = target;
  pkg.ver = "HEAD";
  bool is_in_repos = false;
  bool is_local = false;

  init_lua_state();
  cache_repos();
  for (auto repo : cached_repos) {
    if (arg == repo.first) { is_in_repos = true; }
  }

  if (arg.rfind("http", 0) == 0) {
    pkg.url = arg;
    pkg.name = name_from_url(arg);
  } else if (std::filesystem::exists(arg) || arg == ".") {
    pkg.url = "";
    if (std::filesystem::exists(arg)) {
      pkg.src = arg;
    } else {
      pkg.src = std::filesystem::current_path().string();
    }
    pkg.name = name_from_url(arg);
    is_local = true;
  } else if (is_in_repos) {
    pkg.url = cached_repos[arg].source.value;
    pkg.name = arg;
  } else {
    std::cout << print_error << "'" << arg << "'" << " is not a valid package" << std::endl;
    exit(1);
  }

  cache_install_directories();
  if (!is_local) {
    pkg.src = install_directories["src"] + "/" + pkg.name + "/" + pkg.ver;
  }

  return pkg;
}
