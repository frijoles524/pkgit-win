#include <fstream>
#include <string>

#include "add_repo.hh"
#include "vars.hh"

void add_repo(std::string repo, std::string repo_name) {
  bool is_previous_repos = false;
  std::string rfile_line;
  std::string rfile_contents;
  if (std::filesystem::exists(repo_file)) {
    std::ifstream rfile(repo_file);
    while (getline(rfile, rfile_line)) {
      rfile_contents += rfile_line + "\n";
    }
    rfile.close();
    is_previous_repos = true;
  }

  std::string previous_repos = is_previous_repos ? rfile_contents : "";

  std::ofstream wfile;
  wfile.open(repo_file);
  wfile << previous_repos << "repos[\"" << repo_name << "\"] = \"" << repo << "\"" << std::endl;
  wfile.close();
}
