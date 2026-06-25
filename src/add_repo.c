
/*

  pkgit - package it!

  Copyright (C) 2026 dacctal
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "add_repo.h"
#include "vars.h"

void add_repo(const char *repo, const char *repo_name) {
    bool is_previous_repos = false;
    char rfile_line[1024];
    char rfile_contents[8192] = {0};

    if (file_exists(repo_file)) {
        FILE *rfile = fopen(repo_file, "r");
        if (rfile) {
            while (fgets(rfile_line, sizeof(rfile_line), rfile)) {
                strcat(rfile_contents, rfile_line);
            }
            fclose(rfile);
            is_previous_repos = true;
        }
    }

    char *previous_repos = is_previous_repos ? rfile_contents : "";

    FILE *wfile = fopen(repo_file, "w");
    if (wfile) {
        fprintf(wfile, "%srepositories[\"%s\"] = { url = \"%s\" }\n", previous_repos, repo_name, repo);
        fclose(wfile);
    }
}