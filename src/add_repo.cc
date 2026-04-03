#include <fstream>
#include <string>

#include "add_repo.hh"
#include "vars.hh"

void add_repo(std::string repo, std::string repo_name) {
  std::ifstream rfile(repo_file);
  std::string rfile_line;
  std::string rfile_contents;
  while (getline(rfile, rfile_line)) {
    rfile_contents += rfile_line + "\n";
  }
  rfile.close();

  std::ofstream wfile;
  wfile.open(repo_file);
  wfile << rfile_contents << "repos[\"" << repo_name << "\"] = \"" << repo << "\"" << std::endl;
  wfile.close();
}
