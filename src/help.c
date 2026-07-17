/*

  pkgit - package it!

  Copyright (C) 2026 dacctal
  This program is free software: you can REDistribute it and/or modify
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

#include "help.h"
#include "globs.h"

void help(void) {
	typedef struct {
		const char *short_flag;
		const char *long_flag;
		const char *arg;
		const char *desc;
	} flag_t;

	static const flag_t cmd_flags[] = {
		{"-a", "--add", "[url]", "add a repo"},
		{"-b", "--build", "[path]", "build a package"},
		{"-d", "--declare", "", "install all packages"},
		{"-s", "--search", "[pkgs]", "find a package from your repos"},
		{"-i", "--install", "[pkgs, urls]", "install a package/repo"},
		{"-r", "--remove", "[pkgs]", "remove an installed package"},
		{"-l", "--list", "", "list all installed packages"},
		{"-u", "--update", "", "update all installed packages"},
		{"-h", "--help", "", "display this help message"},
		{"-v", "--version", "", "display version number"},
		{"-c", "--check", "", "run package checks"},
	};

	static const flag_t mod_flags[] = {
		{"-q", "--quiet", "", "add a repo"},
		{"-f", "--force", "", "build a package"},
	};

	if (flags.verbose) {
		printf(BOLD_MAGENTA "      ,        \n");
		printf(BOLD_MAGENTA "     / \\       \n");
		printf(BOLD_MAGENTA "_.--'   '--._  \n");
		printf(BOLD_MAGENTA "`'--,   ,--'`  " COLOR_RESET "pkgit " ITALIC GRAY
							"- package it!" COLOR_RESET "\n");
		printf(BOLD_YELLOW " _-  " BOLD_MAGENTA "\\ /" BOLD_YELLOW
						   "  -_   " MAGENTA VERSION COLOR_RESET "\n");
		printf(BOLD_YELLOW "'-_   " BOLD_MAGENTA "'" BOLD_YELLOW "   _-'  \n");
		printf(BOLD_YELLOW "   `'-.-'`     " COLOR_RESET "\n");
	} else {
		printf(BOLD "pkgit " COLOR_RESET ITALIC GRAY "- package it!" COLOR_RESET
					"\n");
		printf("version " MAGENTA VERSION COLOR_RESET "\n");
	}
	printf("\n");

	printf(RED "subcommand flags" COLOR_RESET ":\n");
	for (size_t i = 0; i < LENGTH(cmd_flags); i++) {
		printf(COLOR_RESET "  " GREEN "%-2s" COLOR_RESET ", " YELLOW
						   "%-12s" BLUE "%-16s" GRAY "# %s\n" COLOR_RESET,
			   cmd_flags[i].short_flag, cmd_flags[i].long_flag,
			   cmd_flags[i].arg, cmd_flags[i].desc);
	}

	printf("\n" RED "modifier flags" COLOR_RESET ":\n");
	for (size_t i = 0; i < LENGTH(mod_flags); i++) {
		printf(COLOR_RESET "  " GREEN "%-2s" COLOR_RESET ", " YELLOW
						   "%-12s" BLUE "%-16s" GRAY "# %s\n" COLOR_RESET,
			   mod_flags[i].short_flag, mod_flags[i].long_flag,
			   mod_flags[i].arg, mod_flags[i].desc);
	}
}
