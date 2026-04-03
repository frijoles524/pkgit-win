#include <iostream>
#include <fstream>

#include "ensure_repo.hh"
#include "vars.hh"

void ensure_repo() {
  if (!std::filesystem::exists(repo_file)) {
    std::cout << "repo file does not exist" << std::endl;
    std::ofstream file;
    file.open(repo_file);
    
    if (!file.is_open()) {
      std::cout << print_error << "repo file not created" << std::endl;
      return;
    }

    file << "repos = {}" << std::endl;
    file << "repos[\"pkgit\"] = https://git.symlinx.net/pkgit" << std::endl;

    std::cout << repo_file << std::endl;
    file.close();
  }
}
