#include <iostream>

#include "list_pkgs.hh"
#include "lua_state.hh"
#include "vars.hh"

void list_pkgs() {
  for (auto repo : cached_repos) {
    std::cout << repo.first << std::endl;
  }
}
