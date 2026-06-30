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

#ifndef PKGIT_GLOBALS_H
#define PKGIT_GLOBALS_H

#define VERSION				"1.4.0_INDEV"
#define RED					"\x1b[0;31m"
#define GREEN				"\x1b[0;32m"
#define YELLOW				"\x1b[0;33m"
#define BLUE				"\x1b[0;34m"
#define MAGENTA				"\x1b[0;35m"
#define CYAN				"\x1b[0;36m"
#define GRAY				"\x1b[0;37m"
#define BRIGHT_RED			"\x1b[0;91m"
#define BRIGHT_GREEN		"\x1b[0;92m"
#define BRIGHT_YELLOW		"\x1b[0;93m"
#define BRIGHT_BLUE			"\x1b[0;94m"
#define BRIGHT_MAGENTA		"\x1b[0;95m"
#define BRIGHT_CYAN			"\x1b[0;96m"
#define BRIGHT_GRAY			"\x1b[0;97m"
#define BOLD_RED			"\x1b[1;31m"
#define BOLD_GREEN			"\x1b[1;32m"
#define BOLD_YELLOW			"\x1b[1;33m"
#define BOLD_BLUE			"\x1b[1;34m"
#define BOLD_MAGENTA		"\x1b[1;35m"
#define BOLD_CYAN			"\x1b[1;36m"
#define BOLD_GRAY			"\x1b[1;37m"
#define BOLD_WHITE			"\x1b[1;38m"
#define BOLD_BRIGHT_RED		"\x1b[1;91m"
#define BOLD_BRIGHT_GREEN	"\x1b[1;92m"
#define BOLD_BRIGHT_YELLOW	"\x1b[1;93m"
#define BOLD_BRIGHT_BLUE	"\x1b[1;94m"
#define BOLD_BRIGHT_MAGENTA	"\x1b[1;95m"
#define BOLD_BRIGHT_CYAN	"\x1b[1;96m"
#define BOLD_BRIGHT_GRAY	"\x1b[1;97m"
#define ITALIC				"\x1b[3m"
#define COLOR_RESET			"\x1b[0m"

#define PRINT_PKGIT BOLD_YELLOW "[" BOLD_MAGENTA "pkgit" BOLD_YELLOW "]" COLOR_RESET
#define PRINT_SUCCESS PRINT_PKGIT GREEN " [SUCCESS]" COLOR_RESET
#define PRINT_SKIPPED PRINT_PKGIT BLUE " [SKIPPED]" COLOR_RESET
#define PRINT_WARNING PRINT_PKGIT YELLOW " [WARNING]" COLOR_RESET
#define PRINT_ERROR PRINT_PKGIT RED " [ERROR]" COLOR_RESET

#include "str.h"

typedef struct {
	str_slc name;
	str_slc url;
	str_slc version;
	str_slc target;
	str_slc src;
	int is_local;
} package;

extern int is_verbose;
extern int is_forced;
extern int config_exists;

extern char root_config_file[20];

void init_vars(void);

#endif
