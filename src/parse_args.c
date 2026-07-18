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

#include "parse_args.h"

#include "build.h"
#include "globs.h"
#include "log.h"
#include "str.h"
#include "check.h"
#include "help.h"
#include "pkg.h"
#include "pkgit_lua.h"
#include "add_repo.h"

#define COMMAND(large, small)                                                  \
	(!strcmp(argv[i], large) || !strcmp(argv[i], small))

#define NOT_ENOUGH_ARGS(arg, next)                                             \
	log_error("Not enough arguments! Try: pkgit %s [%s]", (arg), (next))

void cmd_add(char **argv, int i) {
	if (argv[i + 1]) {
		str arg = mstr(argv[i + 1]);
		package_t pkg = pkg_create(&arg);
		add_repo(&pkg);
		pkg_free(&pkg);
		if (str_is_valid(&arg)) str_free(&arg);
	} else {
		NOT_ENOUGH_ARGS(argv[i], "url");
	}
}

void cmd_build(int argc, char **argv, int i) {
	if (argv[i + 1]) {
		for (int j = i + 1; j < argc; j++) {
			if (argv[j][0] == '-')
				continue;
			str arg = mstr(argv[j]);
			package_t pkg = pkg_create(&arg);
			build(&pkg);
			pkg_free(&pkg);
			if (str_is_valid(&arg)) str_free(&arg);
		}
	} else {
		str arg = mstr(".");
		package_t pkg = pkg_create(&arg);
		build(&pkg);
		pkg_free(&pkg);
		if (str_is_valid(&arg)) str_free(&arg);
	}
}

void cmd_install(int argc, char **argv, int i) {
	if (argv[i + 1]) {
		for (int j = i + 1; j < argc; j++) {
			if (argv[j][0] == '-') continue;
			str arg = mstr(argv[j]);
			package_t pkg = pkg_create(&arg);
			if (flags.force) pkg_remove(&pkg);
			pkg_install(&pkg);
			pkg_free(&pkg);
			if (str_is_valid(&arg)) str_free(&arg);
		}
	} else {
		NOT_ENOUGH_ARGS(argv[i], "url/pkg");
	}
}

void cmd_update(int argc, char **argv, int i) {
	if (argv[i + 1]) {
		for (int j = i + 1; j < argc; j++) {
			if (argv[j][0] == '-') continue;
			str arg = mstr(argv[j]);
			package_t pkg = pkg_create(&arg);
			pkg_update(&pkg);
			pkg_free(&pkg);
			if (str_is_valid(&arg)) str_free(&arg);
		}
	} else {
		all_update();
	}
}

void cmd_remove(int argc, char **argv, int i) {
	if (argv[i + 1]) {
		for (int j = i + 1; j < argc; j++) {
			if (argv[j][0] == '-') continue;
			str arg = mstr(argv[j]);
			package_t pkg = pkg_create(&arg);
			pkg_remove(&pkg);
			pkg_free(&pkg);
			if (str_is_valid(&arg)) str_free(&arg);
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
		case 'a':	cmd_add(argv, i);			break;
		case 'b':	cmd_build(argc, argv, i);	break;
		case 'c':	check();					break;
		case 'd':	declare();					break;
		case 'i':	cmd_install(argc, argv, i);	break;
		case 'r':	cmd_remove(argc, argv, i);	break;
		case 'u':	cmd_update(argc, argv, i);	break;
		case 'l':	panic("not implemented");	printf("list\n");		break;
		case 's':	panic("not implemented");	printf("search\n");		break;
		case 'v':	printf("%s\n", VERSION);	break;
		case 'h':	help();						break;
		default:	break;
		}
	}
}

void parse_flags(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] != '-') continue;
		if (argv[i][1] == '-') {
			if (COMMAND("--quiet", "-q")) flags.verbose = false;
			if (COMMAND("--force", "-f")) flags.force = true;
		} else {
			flags_mod(argv, i);
			flags_cmd(argc, argv, i);
		}
	}
	return;
}

void parse_cmds(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		if (COMMAND("--add", "a"))		{ cmd_add(argv, i); }
		if (COMMAND("--build", "b"))	{ cmd_build(argc, argv, i); }
		if (COMMAND("--install", "i"))	{ cmd_install(argc, argv, i); }
		if (COMMAND("--remove", "r"))	{ cmd_remove(argc, argv, i); }
		if (COMMAND("--update", "u"))	{ cmd_update(argc, argv, i); }
		if (COMMAND("--declare", "d"))	{ declare(); }
		if (COMMAND("--list", "l"))		{ panic("not implemented"); }
		if (COMMAND("--search", "s"))	{ panic("not implemented"); }
		if (COMMAND("--version", "v"))	{ printf(VERSION "\n"); }
		if (COMMAND("--help", "h"))		{ help(); }
		if (COMMAND("--check", "c"))	{ check(); }
	}
}

void parse_args(int argc, char **argv) {
	flags.force = false;
	flags.verbose = true;
	if (argc == 1) {
		help();
		return;
	}
	parse_flags(argc, argv);
	parse_cmds(argc, argv);
	return;
}
