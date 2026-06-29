#include <stdio.h> 

#include "pkgit_string.h" 

int main(int argc, char** argv) {
	str_s slice = slice_trim(slice_from_cstr("  hello world  "));
	printf("[%.*s]\n", format_string(slice));
	return 0;
}
