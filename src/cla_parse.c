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

#include "cla_parse.h"

#include "add_repo.h"
#include "build.h"
#include "create_pkg.h"
#include "declare.h"
#include "search.h"
#include "help.h"
#include "install_pkg.h"
#include "list_pkgs.h"
#include "name_from_url.h"
#include "remove_pkg.h"
#include "resolve_deps.h"
#include "update_all.h"
#include "vars.h"

#define COMMAND(large, small, code)                                           \
	if (strcmp(argv[i], large) == 0 || strcmp(argv[i], small) == 0) code

#define NOT_ENOUGH_ARGS(arg, next)                                            \
	printf("%s Not enough arguments! Try: `pkgit %s [%s]`\n",                 \
		print_error, arg, next)

void cmd_add(char **argv, int i) {
	if (argv[i + 1]) {
		add_repo(argv[i + 1], name_from_url(argv[i + 1]));
	} else {
		NOT_ENOUGH_ARGS(argv[i], "url");
	}
}

void cmd_build(int argc, char **argv, int i, Pkg pkg) {
	if (argv[i + 1]) {
		for (int j = i + 1; j < argc; j++) {
			if (argv[j][0] == '-') continue;
			pkg = create_pkg(argv[j]);
			build(pkg);
		}
	} else {
		pkg = create_pkg(".");
		build(pkg);
	}
}

void cmd_install(int argc, char **argv, int i, Pkg pkg) {
	if (argv[i + 1]) {
		for (int j = i + 1; j < argc; j++) {
			if (argv[j][0] == '-') continue;
			pkg = create_pkg(argv[j]);
			install_pkg(pkg);
		}
	} else {
		NOT_ENOUGH_ARGS(argv[i], "url/pkg");
	}
}

void cmd_remove(int argc, char **argv, int i, Pkg pkg) {
	if (argv[i + 1]) {
		for (int j = i + 1; j < argc; j++) {
			if (argv[j][0] == '-') continue;
			pkg = create_pkg(argv[j]);
			remove_pkg(pkg);
		}
	} else {
		NOT_ENOUGH_ARGS(argv[i], "url/pkg");
	}
}

void mod_flags(char **argv, int i) {
	for (size_t j = 1; j < strlen(argv[i]); j++) {
		switch (argv[i][j]) {
			case 'q': is_verbose = false; break;
			case 'f': is_forced = true; break;
			default:  break;
		}
	}
}

void cmd_flags(int argc, char **argv, int i, Pkg pkg) {
	for (size_t j = 1; j < strlen(argv[i]); j++) {
		switch (argv[i][j]) {
			case 'a': cmd_add(argv, i); break;
			case 'b': cmd_build(argc, argv, i, pkg); break;
			case 'i': cmd_install(argc, argv, i, pkg); break;
			case 'r': cmd_remove(argc, argv, i, pkg); break;
			case 'u': update_all(); break;
			case 'd': declare(); break;
			case 'l': list_pkgs(); break;
			case 's': search(argv[i + 1]); break;
			case 'v': printf("%s\n", version); break;
			case 'h': help(); break;
			case 'c': resolve_deps(); break;
			default:  break;
		}
	}
}

void parse_flags(int argc, char **argv, Pkg pkg) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') continue;
    if (argv[i][1] == '-') {
      COMMAND("--quiet", "-q", { is_verbose = false; });
      COMMAND("--force", "-f", { is_forced = true; });
    } else {
      mod_flags(argv, i);
      cmd_flags(argc, argv, i, pkg);
    }
  }
}

void parse_cmds(int argc, char **argv, Pkg pkg) {
	for (int i = 1; i < argc; i++) {
		COMMAND("--add",      "a", { cmd_add(argv, i); });
		COMMAND("--build",    "b", { cmd_build(argc, argv, i, pkg); });
		COMMAND("--install",  "i", { cmd_install(argc, argv, i, pkg); });
		COMMAND("--remove",   "r", { cmd_remove(argc, argv, i, pkg); });
		COMMAND("--update",   "u", { update_all(); });
		COMMAND("--declare",  "d", { declare(); });
		COMMAND("--list",     "l", { list_pkgs(); });
		COMMAND("--search",   "s", { search(argv[i + 1]); });
		COMMAND("--version",  "v", { printf("%s\n", version); });
		COMMAND("--help",     "h", { help(); });
		COMMAND("--check",    "c", { resolve_deps(); });
	}
}

void cla_parse(int argc, char **argv) {
	if (!argv[1]) { help(); return; }
	Pkg pkg = {0};
	parse_flags(argc, argv, pkg);
	parse_cmds(argc, argv, pkg);
}