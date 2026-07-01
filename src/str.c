/*

pkgit - package it!

Copyright (C) 2026 dacctal
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <https://www.gnu.org/licenses/>.

*/

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globs.h"
#include "str.h"

// NOTE: only use this assertion in functions where you don't depend
// on any other str_ ones, to prevent checking twice
#ifdef PKGIT_DEBUG
#define assert_str_is_valid(str)                                               \
	do {                                                                       \
		if (!str_is_valid(str))                                                \
			panic("Cannot operate on str `%s`, which is invalid!", #str);      \
	} while (0)
#else
#define assert_str_is_valid(str)
#endif

// XXX: INTERNAL USE ONLY
#define _strslc(str)                                                           \
	(str_slc) {                                                                \
		str->data, str->len                                                    \
	}
#define _cstrslc(str)                                                          \
	(str_slc) {                                                                \
		str, strlen(str)                                                       \
	}
// XXX: since msvc/tcc and I presume cproc aren't smart enough to inline calls
// where a str calls a str_slc function and vice versa, we optimize things
// ourselves+reduce boilerplate
#define _STRINGS_EQUAL

str str_new(void) {
	return str_new_with_capacity(16);
}

str str_new_with_capacity(size_t cap) {
	str res = {0};
	res.cap = cap;
	res.data = calloc(res.cap, 1);
	check_alloc(res.data);
	return res;
}

str str_from_cstr(const char *s) {
	return str_from_str_slc(_cstrslc(s));
}

str mstr(const char *s) {
	return str_from_str_slc(_cstrslc(s));
}

str str_from_str_slc(const str_slc s) {
	str res = str_new_with_capacity(s.len + 1);
	str_copy_str_slc_into(&res, s);
	return res;
}

str str_format(const char *format, ...) {
	va_list args, args_copy;
	va_start(args, format);
	va_copy(args_copy, args);

	size_t len = vsnprintf(NULL, 0, format, args_copy);
	str res = str_new_with_capacity(len + 1);
	res.len = len;
	vsnprintf(res.data, res.cap, format, args);

	va_end(args);
	return res;
}

void str_format_into(str *s, const char *format, ...) {
	va_list args, args_copy;
	va_start(args, format);
	va_copy(args_copy, args);

	size_t len = vsnprintf(NULL, 0, format, args_copy);

	if (!str_is_valid(s)) {
		*s = str_new_with_capacity(len + 1);
	} else {
		if (s->cap < len + 1)
			str_reserve_exact(s, len + 1);
		str_clear(s);
	}
	s->len = len;
	vsnprintf(s->data, s->cap, format, args);

	va_end(args);
}

void str_reserve(str *s, size_t new_cap) {
	str_reserve_exact(s, new_cap + (new_cap >> 2));
}

void str_reserve_exact(str *s, size_t new_cap) {
	assert_str_is_valid(s);
	if (s->cap == new_cap)
		return;
	s->data = realloc(s->data, new_cap);
	s->cap = new_cap;
	check_alloc(s->data);
}

void str_clear(str *s) {
	assert_str_is_valid(s);
	memset(s->data, 0, s->cap);
	s->len = 0;
}

void str_free(str *s) {
	// let multiple frees be no-ops
	if (!s->data)
		return;
	free(s->data);
	s->data = NULL;
}

void str_copy_into(str *dest, const str *src) {
	str_ncopy_cstr_into(dest, src->data, src->len);
}

void str_copy_cstr_into(str *dest, const char *src) {
	str_ncopy_cstr_into(dest, src, strlen(src));
}

void str_copy_str_slc_into(str *dest, const str_slc src) {
	str_ncopy_cstr_into(dest, src.data, src.len);
}

void str_ncopy_into(str *dest, const str *src, size_t count) {
	str_ncopy_cstr_into(dest, src->data, count);
}

void str_ncopy_cstr_into(str *dest, const char *src, size_t count) {
	assert_str_is_valid(dest);
	assert(src);
	dest->len = count;
	if (dest->cap < count + 1)
		str_reserve_exact(dest, count + 1);
	memcpy(dest->data, src, count);
	dest->data[count] = 0;
}

void str_ncopy_str_slc_into(str *dest, const str_slc src, size_t count) {
	str_ncopy_cstr_into(dest, src.data, count);
}

str str_dupe(const str *src) {
	assert_str_is_valid(src);
	str res = str_new_with_capacity(src->cap);
	str_copy_into(&res, src);
	return res;
}

void str_append(str *src, const str *new) {
	str_append_str_slc(src, _strslc(new));
}

void str_append_char(str *src, char new) {
	assert_str_is_valid(src);
	// new char + nullterm
	if (src->len + 2 < src->cap) {
		// this will reserve 1.5*src->cap
		str_reserve(src, src->cap);
	}
	src->data[src->len++] = new;
	src->data[src->len] = 0;
}

void str_append_cstr(str *src, const char *new) {
	assert(new);
	str_append_str_slc(src, _cstrslc(new));
}

void str_append_str_slc(str *src, const str_slc new) {
	assert_str_is_valid(src);
	assert_str_is_valid(&new);
	size_t len_after_append = src->len + new.len;
	if (len_after_append + 1 > src->cap) {
		src->cap = len_after_append + 1;
		str_reserve(src, src->cap);
	}
	memcpy(src->data + src->len, new.data, new.len + 1);
	src->len = len_after_append;
	src->data[len_after_append] = 0;
}

char str_pop_last(str *src) {
	char c = str_last(src);
	src->len--;
	return c;
}

str str_concat(const str *lhs, const str *rhs) {
	str res = str_dupe(lhs);
	str_append(&res, rhs);
	return res;
}

str str_concat_cstr(const str *lhs, const char *rhs) {
	str res = str_dupe(lhs);
	str_append_cstr(&res, rhs);
	return res;
}

str str_concat_str_slc(const str *lhs, const str_slc rhs) {
	str res = str_dupe(lhs);
	str_append_str_slc(&res, rhs);
	return res;
}

str str_trim_left(const str *s) {
	str res = str_dupe(s);
	str_trim_left_in_place(&res);
	return res;
}

str str_trim_right(const str *s) {
	str res = str_dupe(s);
	str_trim_right_in_place(&res);
	return res;
}

str str_trim(const str *s) {
	str res = str_dupe(s);
	str_trim_in_place(&res);
	return res;
}

str str_to_upper(const str *s) {
	str res = str_dupe(s);
	str_to_upper_in_place(&res);
	return res;
}

str str_to_lower(const str *s) {
	str res = str_dupe(s);
	str_to_lower_in_place(&res);
	return res;
}

void str_trim_left_in_place(str *s) {
	assert_str_is_valid(s);
	size_t i;
	for (i = 0; i < s->len && isspace(s->data[i]); i++)
		continue;
	s->len -= i;
	memmove(s->data, s->data + i, s->len);
	s->data[s->len] = 0;
}

void str_trim_right_in_place(str *s) {
	assert_str_is_valid(s);
	if (s->len == 0)
		return;
	size_t end;
	for (end = s->len - 1; isspace(s->data[end]); end--)
		continue;
	end++;
	s->len = end;
	s->data[s->len] = 0;
}

void str_trim_in_place(str *s) {
	assert_str_is_valid(s);
	if (s->len == 0)
		return;
	size_t begin, end;
	for (begin = 0; begin < s->len && isspace(s->data[begin]); begin++)
		continue;
	for (end = s->len - 1; isspace(s->data[end]); end--)
		continue;
	end++;
	s->len = end - begin;
	memmove(s->data, s->data + begin, s->len);
	s->data[s->len] = 0;
}

void str_to_upper_in_place(str *s) {
	assert_str_is_valid(s);
	for (size_t i = 0; i < s->len; i++)
		s->data[i] = toupper(s->data[i]);
}

void str_to_lower_in_place(str *s) {
	assert_str_is_valid(s);
	for (size_t i = 0; i < s->len; i++)
		s->data[i] = tolower(s->data[i]);
}

bool str_equal(const str *lhs, const str *rhs) {
	return str_slc_equal(_strslc(lhs), _strslc(rhs));
}

bool str_equal_cstr(const str *lhs, const char *rhs) {
	assert(rhs);
	return str_slc_equal(_strslc(lhs), _cstrslc(rhs));
}

bool str_equal_str_slc(const str *lhs, const str_slc rhs) {
	return str_slc_equal(_strslc(lhs), rhs);
}

int str_compare(const str *lhs, const str *rhs) {
	return str_slc_compare(_strslc(lhs), _strslc(rhs));
}

int str_compare_cstr(const str *lhs, const char *rhs) {
	assert(rhs);
	return str_slc_compare(_strslc(lhs), _cstrslc(rhs));
}

int str_compare_str_slc(const str *lhs, const str_slc rhs) {
	return str_slc_compare(_strslc(lhs), rhs);
}

bool str_equal_case_insensitive(const str *lhs, const str *rhs) {
	return str_slc_equal_case_insensitive(_strslc(lhs), _strslc(rhs));
}

bool str_equal_cstr_case_insensitive(const str *lhs, const char *rhs) {
	assert(rhs);
	return str_slc_equal_case_insensitive(_strslc(lhs), _cstrslc(rhs));
}

bool str_equal_str_slc_case_insensitive(const str *lhs, const str_slc rhs) {
	return str_slc_equal_case_insensitive(_strslc(lhs), rhs);
}

int str_compare_case_insensitive(const str *lhs, const str *rhs) {
	return str_slc_compare_case_insensitive(_strslc(lhs), _strslc(rhs));
}

int str_compare_cstr_case_insensitive(const str *lhs, const char *rhs) {
	assert(rhs);
	return str_slc_compare_case_insensitive(_strslc(lhs), _cstrslc(rhs));
}

int str_compare_str_slc_case_insensitive(const str *lhs, const str_slc rhs) {
	return str_slc_compare_case_insensitive(_strslc(lhs), rhs);
}

str str_slice_to_str(const str *src, ptrdiff_t begin, ptrdiff_t end) {
	str_slc slc = str_slc_slice(_strslc(src), begin, end);
	return str_from_str_slc(slc);
}

str str_slice_from_cstr(const char *src, ptrdiff_t begin, ptrdiff_t end) {
	str_slc slc = str_slc_slice(_cstrslc(src), begin, end);
	return str_from_str_slc(slc);
}

str_slc str_slice(const str *src, ptrdiff_t begin, ptrdiff_t end) {
	return str_slc_slice(_strslc(src), begin, end);
}

str str_slice_from_str_slc(const str_slc src, ptrdiff_t begin, ptrdiff_t end) {
	str_slc slc = str_slc_slice(src, begin, end);
	return str_from_str_slc(slc);
}

size_t str_to_double_pro(const str *src, double *res) {
	return str_slc_to_double_pro(_strslc(src), res);
}

size_t str_to_int64_pro(const str *src, int64_t *res, int base) {
	return str_slc_to_int64_pro(_strslc(src), res, base);
}

bool str_to_double(const str *src, double *res) {
	return str_to_double_pro(src, res) == src->len;
}

bool str_to_int64(const str *src, int64_t *res) {
	return str_to_int64_pro(src, res, 10) == src->len;
}

char str_at(const str *s, size_t i) {
	assert_str_is_valid(s);
#ifndef PKGIT_DEBUG
	if (i >= s->len)
		panic("Out of bounds string access at index %zu (length: %zu)", i,
			  s->len);
#endif
	return s->data[i];
}

char str_first(const str *s) {
	assert_str_is_valid(s);
#ifndef PKGIT_DEBUG
	if (!s->len)
		panic("Attempted to get the first character of an empty string");
#endif
	return *s->data;
}

char str_last(const str *s) {
	assert_str_is_valid(s);
#ifndef PKGIT_DEBUG
	if (!s->len)
		panic("Attempted to get the last character of an empty string");
#endif
	return s->data[s->len - 1];
}

size_t str_find_char(const str *s, char c) {
	return str_slc_find_char(_strslc(s), c);
}

size_t str_find_char_right(const str *s, char c) {
	return str_slc_find_char_right(_strslc(s), c);
}

void str_print(const str *s) {
	printf("%.*s", str_fmt(s));
}

void str_println(const str *s) {
	printf("%.*s\n", str_fmt(s));
}

void str_fprint(const str *s, FILE *f) {
	fprintf(f, "%.*s", str_fmt(s));
}

void str_fprintln(const str *s, FILE *f) {
	fprintf(f, "%.*s\n", str_fmt(s));
}

str str_read_line_from_file(FILE *f) {
	size_t len = 0;
	char c;
	for (len = 0; (c = fgetc(f)) != EOF && c != '\n'; len++)
		continue;
	fseek(f, -len - 1, SEEK_SET);
	str res = str_new_with_capacity(len + 1);
	size_t bytes_read = fread(res.data, 1, len, f);
	assert(bytes_read == len);
	res.len = len;
	return res;
}

str str_read_chars_from_file(FILE *f, size_t count) {
	str res = str_new_with_capacity(count + 1);
	size_t bytes_read = fread(res.data, 1, count, f);
	assert(bytes_read == count);
	res.len = count;
	return res;
}

str str_read_entire_file(FILE *f) {
	fseek(f, 0, SEEK_END);
	size_t len = ftell(f);
	rewind(f);
	str res = str_new_with_capacity(len + 1);
	size_t bytes_read = fread(res.data, 1, len, f);
	assert(bytes_read == len);
	res.len = len;
	return res;
}

bool str_write_to_file(const str *s, FILE *f) {
	return str_slc_write_to_file(_strslc(s), f);
}

// === str_slc implementation ===

str_slc str_slc_from_str(const str *s) {
	return _strslc(s);
}

str_slc str_slc_from_cstr(const char *s) {
	return _cstrslc(s);
}

str_slc mstrslc(const char *s) {
	return (str_slc){s, strlen(s)};
}

str str_slc_concat(const str_slc lhs, const str_slc rhs) {
	str res = str_new();
	str_append_str_slc(&res, lhs);
	str_append_str_slc(&res, rhs);
	return res;
}

str str_slc_concat_cstr(const str_slc lhs, const char *rhs) {
	str res = str_new();
	str_append_str_slc(&res, lhs);
	str_append_cstr(&res, rhs);
	return res;
}

bool str_slc_equal(const str_slc lhs, const str_slc rhs) {
	assert_str_is_valid(&lhs);
	assert_str_is_valid(&rhs);
	return (lhs.len == rhs.len) && memcmp(lhs.data, rhs.data, lhs.len) == 0;
}

bool str_slc_equal_cstr(const str_slc lhs, const char *rhs) {
	assert(rhs);
	return str_slc_equal(lhs, _cstrslc(rhs));
}

bool str_slc_equal_case_insensitive(const str_slc lhs, const str_slc rhs) {
	assert_str_is_valid(&lhs);
	assert_str_is_valid(&rhs);
	if (lhs.len != rhs.len)
		return false;
	for (size_t i = 0; i < lhs.len; i++)
		if (tolower(lhs.data[i]) != tolower(rhs.data[i]))
			return false;
	return true;
}

bool str_slc_equal_cstr_case_insensitive(const str_slc lhs, const char *rhs) {
	return str_slc_equal_case_insensitive(lhs, _cstrslc(rhs));
}

int str_slc_compare(const str_slc lhs, const str_slc rhs) {
	assert_str_is_valid(&lhs);
	assert_str_is_valid(&rhs);
	if (lhs.len < rhs.len)
		return -1;
	else if (lhs.len > rhs.len)
		return 1;
	for (size_t i = 0; i < lhs.len; i++)
		if (lhs.data[i] != rhs.data[i])
			return (uint8_t)lhs.data[i] - (uint8_t)rhs.data[i];
	return 0;
}

int str_slc_compare_cstr(const str_slc lhs, const char *rhs) {
	return str_slc_compare(lhs, _cstrslc(rhs));
}

int str_slc_compare_case_insensitive(const str_slc lhs, const str_slc rhs) {
	assert_str_is_valid(&lhs);
	assert_str_is_valid(&rhs);
	if (lhs.len < rhs.len)
		return -1;
	else if (lhs.len > rhs.len)
		return 1;
	char cl, cr;
	for (size_t i = 0; i < lhs.len; i++)
		if ((cl = tolower(lhs.data[i])) != (cr = tolower(rhs.data[i])))
			return (uint8_t)cl - (uint8_t)cr;
	return 0;
}

int str_slc_compare_cstr_case_insensitive(const str_slc lhs, const char *rhs) {
	return str_slc_compare_case_insensitive(lhs, _cstrslc(rhs));
}

str_slc str_slc_slice(const str_slc src, ptrdiff_t begin, ptrdiff_t end) {
	assert_str_is_valid(&src);
	if (begin > end)
		return (str_slc){src.data, 0};
	if (end < 0)
		end = 0;
	else if (end > (ptrdiff_t)src.len)
		end = src.len;
	if (begin < 0)
		begin = 0;
	else if (begin > (ptrdiff_t)src.len)
		begin = src.len;
	str_slc res = {
		.data = src.data + begin,
		.len = end - begin,
	};
	return res;
}

// Equivalent to str_slice_from_str_slc
str str_slc_slice_to_str(const str_slc s, ptrdiff_t begin, ptrdiff_t end) {
	return str_slice_from_str_slc(s, begin, end);
}

str str_slc_to_lower(const str_slc s) {
	str res = str_from_str_slc(s);
	str_to_lower_in_place(&res);
	return res;
}

str str_slc_to_upper(const str_slc s) {
	str res = str_from_str_slc(s);
	str_to_upper_in_place(&res);
	return res;
}

str_slc str_slc_trim_left(const str_slc s) {
	assert_str_is_valid(&s);
	size_t i;
	for (i = 0; i < s.len && isspace(s.data[i]); i++)
		continue;
	return (str_slc){s.data + i, s.len - i};
}

str_slc str_slc_trim_right(const str_slc s) {
	if (s.len == 0)
		return s;
	size_t end;
	for (end = s.len - 1; isspace(s.data[end]); end--)
		continue;
	end++;
	return (str_slc){s.data, end};
}

str_slc str_slc_trim(const str_slc s) {
	if (s.len == 0)
		return s;
	size_t begin, end;
	for (begin = 0; begin < s.len && isspace(s.data[begin]); begin++)
		continue;
	for (end = s.len - 1; isspace(s.data[end]); end--)
		continue;
	end++;
	return (str_slc){s.data + begin, end - begin};
}

size_t str_slc_to_double_pro(const str_slc src, double *res) {
	assert_str_is_valid(&src);
	errno = 0;
	char *endptr = NULL;
	const char *nptr = src.data;
	double num = strtod(nptr, &endptr);
	size_t diff = endptr - nptr;
	*res = num;
	return diff;
}

size_t str_slc_to_int64_pro(const str_slc src, int64_t *res, int base) {
	assert_str_is_valid(&src);
	errno = 0;
	char *endptr = NULL;
	const char *nptr = src.data;
	int64_t num = strtoll(nptr, &endptr, base);
	size_t diff = endptr - nptr;
	*res = num;
	return diff;
}

bool str_slc_to_double(const str_slc src, double *res) {
	return str_slc_to_double_pro(src, res) == src.len;
}

bool str_slc_to_int64(const str_slc src, int64_t *res) {
	return str_slc_to_int64_pro(src, res, 10) == src.len;
}

bool str_slc_write_to_file(const str_slc s, FILE *f) {
	return s.len == fwrite(s.data, 1, s.len, f);
}

char str_slc_first(const str_slc s) {
	assert_str_is_valid(&s);
#ifndef PKGIT_DEBUG
	if (!s.len)
		panic("Attempted to get the first character of an empty string");
#endif
	return *s.data;
}

char str_slc_last(const str_slc s) {
	assert_str_is_valid(&s);
#ifndef PKGIT_DEBUG
	if (!s.len)
		panic("Attempted to get the last character of an empty string");
#endif
	return s.data[s.len - 1];
}

char str_slc_at(const str_slc s, size_t i) {
	assert_str_is_valid(&s);
#ifndef PKGIT_DEBUG
	if (i >= s.len)
		panic("Out of bounds string access at index %zu (length: %zu)", i,
			  s.len);
#endif
	return s.data[i];
}

void str_slc_print(const str_slc s) {
	printf("%.*s", str_fmt(&s));
}

void str_slc_println(const str_slc s) {
	printf("%.*s\n", str_fmt(&s));
}

void str_slc_fprint(const str_slc s, FILE *f) {
	fprintf(f, "%.*s", str_fmt(&s));
}

void str_slc_fprintln(const str_slc s, FILE *f) {
	fprintf(f, "%.*s\n", str_fmt(&s));
}

ptrdiff_t str_slc_find_char(const str_slc s, char c) {
	size_t i;
	for (i = 0; i < s.len; i++)
		if (s.data[i] == c)
			break;
	return (i == s.len) ? 0 : i;
}

ptrdiff_t str_slc_find_char_right(const str_slc s, char c) {
	if (s.len == 0)
		return 0;
	for (ptrdiff_t i = s.len - 1; i >= 0; i--) {
		if (s.data[i] == c)
			return (size_t)i;
	}
	return s.len;
}

str str_from_after_delim(str *arg, char delimiter) {
	size_t i = str_find_char_right(arg, delimiter);
	str res = str_slice_to_str(arg, i + 1, arg->len);
	return res;
}

str str_from_before_delim(str *arg, char delimiter) {
	size_t i = str_find_char(arg, delimiter);
	str res = str_slice_to_str(arg, i + 1, arg->len);
	return res;
}

str_slc str_slc_from_after_delim(str_slc arg, char delimiter) {
	size_t i = str_slc_find_char_right(arg, delimiter);
	str_slc res = str_slc_slice(arg, i + 1, arg.len);
	return res;
}

str_slc str_slc_from_before_delim(str_slc arg, char delimiter) {
	size_t i = str_slc_find_char(arg, delimiter);
	str_slc res = str_slc_slice(arg, i + 1, arg.len);
	return res;
}
