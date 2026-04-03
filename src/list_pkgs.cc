#include <iostream>

#include "list_pkgs.hh"
#include "vars.hh"

void list_pkgs() {
  for (auto repo : repos) {
    std::cout << repo.first << std::endl;
  }
}
