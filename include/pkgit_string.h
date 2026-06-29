#ifndef PKGIT_STRING_H
#define PKGIT_STRING_H
#include <stddef.h>

typedef struct {
	char* data;
	size_t len;
} str_s;

typedef struct {
	char* data;
	size_t cap;
	size_t len;
} str;

#define format_string(s) (int)((s).len), (s).data
// Usage: printf("%.*s", format_string(ss));

void print_slice(str_s s);
str_s slice_from_cstr(char* cstr);
int slice_eq(str_s a, str_s b);
int slice_starts_with(str_s slice, str_s prefix);
str_s slice_take(str_s slice, size_t n);
str_s slice_drop(str_s slice, size_t n);
str_s slice_trim(str_s slice);

#endif
