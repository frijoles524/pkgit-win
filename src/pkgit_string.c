#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "pkgit_string.h"

void print_slice(str_s slice) {
	for (size_t i = 0; i < slice.len; i++) {
		putchar(slice.data[i]);
	}
}

str_s slice_from_cstr(char* cstr) {
	return (str_s) {
		.data = cstr,
		.len = strlen(cstr),
	};
}

int slice_eq(str_s a, str_s b) {
	if (a.len != b.len) return 0;
	for (size_t i = 0; i < a.len; i++) {
		if (a.data[i] != b.data[i]) return 0;
	}
	return 1;
}

int slice_starts_with(str_s slice, str_s prefix) {
	if (prefix.len > slice.len) return 0;
	for (size_t i = 0; i < prefix.len; i++) {
		if (slice.data[i] != prefix.data[i]) return 0;
	}
	return 1;
}

str_s slice_take(str_s slice, size_t n) {
	if (n > slice.len) n = slice.len;
	return (str_s) {
		.data = slice.data,
		.len = n
	};
}

str_s slice_drop(str_s slice, size_t n) {
	if (n > slice.len) n = slice.len;
	return (str_s) {
		.data = slice.data + n,
		.len = slice.len - n
	};
}

str_s slice_trim(str_s slice) {
	while (slice.len > 0 && isspace((unsigned char)slice.data[0])) {
		slice = slice_drop(slice, 1);
	}
	while (slice.len > 0 && isspace((unsigned char)slice.data[slice.len - 1])) {
		slice = slice_take(slice, slice.len - 1);
	}
	return slice;
}
