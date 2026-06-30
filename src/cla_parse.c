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

#include "globs.h"
#include "str.h"

//#include "declare.h"
//#include "deps_resolve.h"
//#include "help.h"
//#include "name_from_url.h"
//#include "pkg_build.h"
//#include "pkg_create.h"
//#include "pkg_search.h"
//#include "pkg_install.h"
//#include "pkg_list.h"
//#include "pkg_remove.h"
//#include "pkgit_globals.h"
//#include "repo_add.h"
//#include "update.h"

#define COMMAND(large, small, code)									\
	if (slc_eq(slc_from_cstr(argv[i]), slc_from_cstr(large))		\
	|| slc_eq(slc_from_cstr(argv[i]), slc_from_cstr(small))) code

#define NOT_ENOUGH_ARGS(arg, next)									\
	printf("%.*s Not enough arguments! Try: `pkgit %s [%s]`\n",		\
	PRINT_ERROR, arg, next)

void cmd_add(char **argv, int i) {
	if (argv[i + 1]) {
		printf("add repo %s", argv[i + 1]);
		//repo_add(argv[i + 1], name_from_url(argv[i + 1]));
	} else {
//		NOT_ENOUGH_ARGS(argv[i], "url");
	}
}

void cmd_build(int argc, char **argv, int i, package pkg) {
	if (argv[i + 1]) {
		for (int j = i + 1; j < argc; j++) {
			if (argv[j][0] == '-') continue;
			printf("build pkg %s", argv[j]);
			//pkg = pkg_create(argv[j]);
			//pkg_build(pkg);
		}
	} else {
		printf("build pkg .");
		//pkg = pkg_create(".");
		//pkg_build(pkg);
	}
}

void cmd_install(int argc, char **argv, int i, package pkg) {
	if (argv[i + 1]) {
		for (int j = i + 1; j < argc; j++) {
			if (argv[j][0] == '-') continue;
			printf("install pkg %s", argv[j]);
			//pkg = pkg_create(argv[j]);
			//pkg_install(pkg);
		}
	} else {
		//NOT_ENOUGH_ARGS(argv[i], "url/pkg");
	}
}

void cmd_remove(int argc, char **argv, int i, package pkg) {
	if (argv[i + 1]) {
		for (int j = i + 1; j < argc; j++) {
			if (argv[j][0] == '-') continue;
			printf("remove pkg %s", argv[j]);
			//pkg = pkg_create(argv[j]);
			//pkg_remove(pkg);
		}
	} else {
		//NOT_ENOUGH_ARGS(argv[i], "url/pkg");
	}
}

void flags_mod(char **argv, int i) {
	for (size_t j = 1; j < strlen(argv[i]); j++) {
		switch (argv[i][j]) {
			case 'q': is_verbose = 0; break;
			case 'f': is_forced = 1; break;
			default:  break;
		}
	}
}

void flags_cmd(int argc, char **argv, int i, package pkg) {
	for (size_t j = 1; j < strlen(argv[i]); j++) {
		switch (argv[i][j]) {
			case 'a': cmd_add(argv, i); break;
			case 'b': cmd_build(argc, argv, i, pkg); break;
			case 'c': /*deps_resolve();*/ printf("deps_resolve\n"); break;
			case 'd': /*declare();*/ printf("declare\n"); break;
			case 'i': cmd_install(argc, argv, i, pkg); break;
			case 'r': cmd_remove(argc, argv, i, pkg); break;
			case 'u': /*update();*/ printf("update\n"); break;
			case 'l': /*pkgs_list();*/ printf("list\n"); break;
			case 's': /*search(argv[i + 1]);*/ printf("search\n"); break;
			case 'v': printf("%s\n", VERSION); break;
			case 'h': /*help();*/ printf("help\n"); break;
			default:  break;
		}
	}
}

void flags_parse(int argc, char **argv, package pkg) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') continue;
    if (argv[i][1] == '-') {
      COMMAND("--quiet", "-q", { is_verbose = 0; });
      COMMAND("--force", "-f", { is_forced = 1; });
    } else {
      flags_mod(argv, i);
      flags_cmd(argc, argv, i, pkg);
    }
  }
}

void cmds_parse(int argc, char **argv, package pkg) {
	for (int i = 1; i < argc; i++) {
		COMMAND("--add",      "a", { cmd_add(argv, i); });
		COMMAND("--build",    "b", { cmd_build(argc, argv, i, pkg); });
		COMMAND("--install",  "i", { cmd_install(argc, argv, i, pkg); });
		COMMAND("--remove",   "r", { cmd_remove(argc, argv, i, pkg); });
		COMMAND("--update",   "u", { /*update();*/ });
		COMMAND("--declare",  "d", { /*declare();*/ });
		COMMAND("--list",     "l", { /*pkgs_list();*/ });
		COMMAND("--search",   "s", { /*search(argv[i + 1]);*/ });
		COMMAND("--version",  "v", { printf("%s\n", VERSION); });
		COMMAND("--help",     "h", { /*help();*/ });
		COMMAND("--check",    "c", { /*deps_resolve();*/ });
	}
}

void cla_parse(int argc, char **argv) {
	if (!argv[1]) { /*help();*/ return; }
	package pkg = {0};
	flags_parse(argc, argv, pkg);
	cmds_parse(argc, argv, pkg);
}
