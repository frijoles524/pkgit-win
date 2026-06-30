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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"

void slc_print(str_slc slc) {
	for (size_t i = 0; i < slc.len; i++) {
		putchar(slc.data[i]);
	}
}

str_slc slc_from_cstr(char* cstr) {
	return (str_slc) {
		.data = cstr,
		.len = strlen(cstr),
	};
}

int slc_eq(str_slc a, str_slc b) {
	if (a.len != b.len) return 0;
	for (size_t i = 0; i < a.len; i++) {
		if (a.data[i] != b.data[i]) return 0;
	}
	return 1;
}

int slc_starts_with(str_slc slc, str_slc prefix) {
	if (prefix.len > slc.len) return 0;
	for (size_t i = 0; i < prefix.len; i++) {
		if (slc.data[i] != prefix.data[i]) return 0;
	}
	return 1;
}

str_slc slc_take(str_slc slc, size_t n) {
	if (n > slc.len) n = slc.len;
	return (str_slc) {
		.data = slc.data,
		.len = n
	};
}

str_slc slc_drop(str_slc slc, size_t n) {
	if (n > slc.len) n = slc.len;
	return (str_slc) {
		.data = slc.data + n,
		.len = slc.len - n
	};
}

str_slc slc_trim(str_slc slc) {
	while (slc.len > 0 && isspace((unsigned char)slc.data[0])) {
		slc = slc_drop(slc, 1);
	}
	while (slc.len > 0 && isspace((unsigned char)slc.data[slc.len - 1])) {
		slc = slc_take(slc, slc.len - 1);
	}
	return slc;
}

str_slc slc_split(str_slc *slc, char delimiter) {
	size_t i = 0;
	while(i < slc->len && slc->data[i] != delimiter) {
		i++;
	}

	str_slc result = slc_take(*slc, i);
	if (i < slc->len) {
		*slc = slc_drop(*slc, i + i);
	} else {
		*slc = slc_drop(*slc, i);
	}
	return result;
}

void slc_cat(str_slc first, str_slc second, str_slc *new_slc) {
	if (new_slc->len < first.len + second.len) {
		printf("fatal: slc_cat length of new slice is too small\n");
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < first.len; i++) {
		new_slc->data[i] = first.data[i];
	}
	for (size_t i = first.len; i < new_slc->len; i++) {
		new_slc->data[i] = second.data[i-first.len];
	}
}
