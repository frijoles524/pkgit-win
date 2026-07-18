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

#include <stdlib.h>

#include "str.h"

#define VERSION "1.2.0_INDEV"
#define RED "\x1b[0;31m"
#define GREEN "\x1b[0;32m"
#define YELLOW "\x1b[0;33m"
#define BLUE "\x1b[0;34m"
#define MAGENTA "\x1b[0;35m"
#define CYAN "\x1b[0;36m"
#define GRAY "\x1b[0;37m"
#define BRIGHT_RED "\x1b[0;91m"
#define BRIGHT_GREEN "\x1b[0;92m"
#define BRIGHT_YELLOW "\x1b[0;93m"
#define BRIGHT_BLUE "\x1b[0;94m"
#define BRIGHT_MAGENTA "\x1b[0;95m"
#define BRIGHT_CYAN "\x1b[0;96m"
#define BRIGHT_GRAY "\x1b[0;97m"
#define BOLD "\x1b[1m"
#define BOLD_RED "\x1b[1;31m"
#define BOLD_GREEN "\x1b[1;32m"
#define BOLD_YELLOW "\x1b[1;33m"
#define BOLD_BLUE "\x1b[1;34m"
#define BOLD_MAGENTA "\x1b[1;35m"
#define BOLD_CYAN "\x1b[1;36m"
#define BOLD_GRAY "\x1b[1;37m"
#define BOLD_WHITE "\x1b[1;38m"
#define BOLD_BRIGHT_RED "\x1b[1;91m"
#define BOLD_BRIGHT_GREEN "\x1b[1;92m"
#define BOLD_BRIGHT_YELLOW "\x1b[1;93m"
#define BOLD_BRIGHT_BLUE "\x1b[1;94m"
#define BOLD_BRIGHT_MAGENTA "\x1b[1;95m"
#define BOLD_BRIGHT_CYAN "\x1b[1;96m"
#define BOLD_BRIGHT_GRAY "\x1b[1;97m"
#define ITALIC "\x1b[3m"
#define COLOR_RESET "\x1b[0m"

#define LENGTH(lst) (sizeof(lst) / sizeof(*(lst)))

#define eprintf(...) fprintf(stderr, __VA_ARGS__);

#define check_alloc(ptr)                                                       \
	do {                                                                       \
		if (ptr == NULL)                                                       \
			panic("allocation of `%s` failed", #ptr);                          \
	} while (0)

#define panic(...)                                                            \
	do {                                                                       \
		char *tmp = strrchr(__FILE__, '/');                                    \
		eprintf(BOLD RED "panic:" COLOR_RESET " line %d, func \"%s\" in file " \
						 "\"%s\": ",                                           \
				__LINE__, __func__, tmp ? tmp + 1 : __FILE__);                 \
		eprintf(__VA_ARGS__);                                                  \
		eprintf("\n");                                                         \
		fflush(stderr);                                                        \
		abort();                                                               \
	} while (0)

#ifdef PKGIT_DEBUG
#define assert(expr)                                                           \
	do {                                                                       \
		if (!(expr))                                                           \
			panic("Assertion `%s` failed", #expr);                             \
	} while (0)
#else
#define assert(expr) (void)(expr)
#endif

#ifdef unreachable
#undef unreachable
#endif
#define unreachable panic("reached unreachable code")

#define PKGIT_PREFIX                                                           \
	BOLD_YELLOW "[" BOLD_MAGENTA "pkgit" BOLD_YELLOW "] " COLOR_RESET
#define PKGIT_PREFIX_SUCCESS PKGIT_PREFIX GREEN "[SUCCESS] " COLOR_RESET
#define PKGIT_PREFIX_INFO PKGIT_PREFIX BLUE "[INFO] " COLOR_RESET
#define PKGIT_PREFIX_WARNING PKGIT_PREFIX YELLOW "[WARNING] " COLOR_RESET
#define PKGIT_PREFIX_ERROR PKGIT_PREFIX RED "[ERROR] " COLOR_RESET

#define MAX_REPOS 1000
#define MAX_DIRS 100
#define MAX_PATH_LEN 1024

typedef struct {
	bool verbose, force;
} cli_flags_t;

typedef struct {
	str dir, name, content, repos;
	bool is_root_config;
} config_t;

typedef struct {
	str prefix, bin, lib, include, src;
} install_dirs_t;

extern cli_flags_t flags;
extern config_t cfg;
extern install_dirs_t inst_dirs;
extern str new_arg_str;

void init_vars(void);
void free_vars(void);

#endif
