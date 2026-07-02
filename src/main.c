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

// #include "cla_parse.h"
#include "pkg.h"
#include "globs.h"
#include "str.h"

int main(int argc, char **argv) {
	(void)argc;
	init_vars();
	package_t pkg = pkg_create(mstrslc(argv[1]));
	printf("NAME:\t");
	str_println(&pkg.name);
	printf("URL:\t");
	str_println(&pkg.url);
	printf("VER:\t");
	str_println(&pkg.version);
	printf("TRG:\t");
	str_println(&pkg.target);
	printf("SRC:\t");
	str_println(&pkg.src);
	pkg_free(&pkg);
	//cla_parse(argc, argv);
	free_vars();
	return 0;
}
