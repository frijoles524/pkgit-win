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
#include "log.h"
#include "str.h"
// #include "declare.h"
// #include "deps_resolve.h"
#include "help.h"
// #include "name_from_url.h"
// #include "pkg_build.h"
#include "pkg/create.h"
// #include "pkg_search.h"
// #include "pkg_install.h"
// #include "pkg_list.h"
// #include "pkg_remove.h"
// #include "pkgit_globals.h"
// #include "repo_add.h"
// #include "update.h"

#define COMMAND(arg, large, small, code)                                       \
	if (strcmp(arg, large) == 0 || strcmp(arg, small) == 0)                    \
	code

#define NOT_ENOUGH_ARGS(arg, next)                                             \
	log_error("Not enough arguments! Try: pkgit %s [%s]", (arg), (next))

void cmd_add(char **argv, int i) {
	if (argv[i + 1]) {
		printf("add repo %s\n", argv[i + 1]);
		// repo_add(argv[i + 1], name_from_url(argv[i + 1]));
	} else {
		NOT_ENOUGH_ARGS(argv[i], "url");
	}
}

void cmd_build(int argc, char **argv, int i) {
	if (argv[i + 1]) {
		for (int j = i + 1; j < argc; j++) {
			if (argv[j][0] == '-') continue;
			printf("build pkg %s\n", argv[j]);
			// str_slc arg = mstrslc(argv[j]);
			// package_t pkg = pkg_create(arg);
			//  pkg_build(pkg);
		}
	} else {
		printf("build pkg .\n");
		// str_slc arg = mstrslc(".");
		// package_t pkg = pkg_create(arg);
		//  pkg_build(pkg);
	}
}

void cmd_install(int argc, char **argv, int i) {
	if (argv[i + 1]) {
		for (int j = i + 1; j < argc; j++) {
			if (argv[j][0] == '-') continue;
			printf("install pkg %s\n", argv[j]);
			// str_slc arg = mstrslc(argv[j]);
			// package_t pkg = pkg_create(arg);
			//  pkg_install(pkg);
		}
	} else {
		NOT_ENOUGH_ARGS(argv[i], "url/pkg");
	}
}

void cmd_remove(int argc, char **argv, int i) {
	if (argv[i + 1]) {
		for (int j = i + 1; j < argc; j++) {
			if (argv[j][0] == '-') continue;
			printf("remove pkg %s\n", argv[j]);
			// str_slc arg = mstrslc(argv[j]);
			// package_t pkg = pkg_create(arg);
			//  pkg_remove(pkg);
		}
	} else {
		NOT_ENOUGH_ARGS(argv[i], "url/pkg");
	}
}

void flags_mod(char **argv, int i) {
	for (size_t j = 1; j < strlen(argv[i]); j++) {
		switch (argv[i][j]) {
		case 'q':
			flags.verbose = false;
			break;
		case 'f':
			flags.force = true;
			break;
		default:
			break;
		}
	}
}

void flags_cmd(int argc, char **argv, int i) {
	for (size_t j = 1; j < strlen(argv[i]); j++) {
		switch (argv[i][j]) {
		case 'a':
			cmd_add(argv, i);
			break;
		case 'b':
			cmd_build(argc, argv, i);
			break;
		case 'c': /*deps_resolve();*/
			printf("deps_resolve\n");
			break;
		case 'd': /*declare();*/
			printf("declare\n");
			break;
		case 'i':
			cmd_install(argc, argv, i);
			break;
		case 'r':
			cmd_remove(argc, argv, i);
			break;
		case 'u': /*update();*/
			printf("update\n");
			break;
		case 'l': /*pkgs_list();*/
			printf("list\n");
			break;
		case 's': /*search(argv[i + 1]);*/
			printf("search\n");
			break;
		case 'v':
			printf("%s\n", VERSION);
			break;
		case 'h':
			help();
			break;
		default:
			break;
		}
	}
}

void flags_parse(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] != '-') continue;
		if (argv[i][1] == '-') {
			COMMAND(argv[i], "--quiet", "-q", { flags.verbose = false; });
			COMMAND(argv[i], "--force", "-f", { flags.force = true; });
		} else {
			flags_mod(argv, i);
			flags_cmd(argc, argv, i);
		}
	}
}

void cmds_parse(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		COMMAND(argv[i], "--add", "a", { cmd_add(argv, i); });
		COMMAND(argv[i], "--build", "b", { cmd_build(argc, argv, i); });
		COMMAND(argv[i], "--install", "i", { cmd_install(argc, argv, i); });
		COMMAND(argv[i], "--remove", "r", { cmd_remove(argc, argv, i); });
		COMMAND(argv[i], "--update", "u", {/*update();*/});
		COMMAND(argv[i], "--declare", "d", {/*declare();*/});
		COMMAND(argv[i], "--list", "l", {/*pkgs_list();*/});
		COMMAND(argv[i], "--search", "s", {/*search(argv[i + 1]);*/});
		COMMAND(argv[i], "--version", "v", { printf("%s\n", VERSION); });
		COMMAND(argv[i], "--help", "h", { help(); });
		COMMAND(argv[i], "--check", "c", {/*deps_resolve();*/});
	}
}

void cla_parse(int argc, char **argv) {
	if (!argv[1]) {
		help();
		return;
	}
	flags_parse(argc, argv);
	cmds_parse(argc, argv);
}
