#ifndef PKGIT_GLOBALS_H
#define PKGIT_GLOBALS_H
#include "pkgit_string.h"

typedef struct {
	str_slc name;
	str_slc url;
	str_slc version;
	str_slc target;
	str_slc src;
	int is_local;
} package;

#define VERSION				slc_from_cstr("1.4.0_INDEV")
#define RED					slc_from_cstr("\x1b[0;31m")
#define GREEN				slc_from_cstr("\x1b[0;32m")
#define YELLOW				slc_from_cstr("\x1b[0;33m")
#define BLUE				slc_from_cstr("\x1b[0;34m")
#define MAGENTA				slc_from_cstr("\x1b[0;35m")
#define CYAN				slc_from_cstr("\x1b[0;36m")
#define GRAY				slc_from_cstr("\x1b[0;37m")
#define BRIGHT_RED			slc_from_cstr("\x1b[0;91m")
#define BRIGHT_GREEN		slc_from_cstr("\x1b[0;92m")
#define BRIGHT_YELLOW		slc_from_cstr("\x1b[0;93m")
#define BRIGHT_BLUE			slc_from_cstr("\x1b[0;94m")
#define BRIGHT_MAGENTA		slc_from_cstr("\x1b[0;95m")
#define BRIGHT_CYAN			slc_from_cstr("\x1b[0;96m")
#define BRIGHT_GRAY			slc_from_cstr("\x1b[0;97m")
#define BOLD_RED			slc_from_cstr("\x1b[1;31m")
#define BOLD_GREEN			slc_from_cstr("\x1b[1;32m")
#define BOLD_YELLOW			slc_from_cstr("\x1b[1;33m")
#define BOLD_BLUE			slc_from_cstr("\x1b[1;34m")
#define BOLD_MAGENTA		slc_from_cstr("\x1b[1;35m")
#define BOLD_CYAN			slc_from_cstr("\x1b[1;36m")
#define BOLD_GRAY			slc_from_cstr("\x1b[1;37m")
#define BOLD_WHITE			slc_from_cstr("\x1b[1;38m")
#define BOLD_BRIGHT_RED		slc_from_cstr("\x1b[1;91m")
#define BOLD_BRIGHT_GREEN	slc_from_cstr("\x1b[1;92m")
#define BOLD_BRIGHT_YELLOW	slc_from_cstr("\x1b[1;93m")
#define BOLD_BRIGHT_BLUE	slc_from_cstr("\x1b[1;94m")
#define BOLD_BRIGHT_MAGENTA	slc_from_cstr("\x1b[1;95m")
#define BOLD_BRIGHT_CYAN	slc_from_cstr("\x1b[1;96m")
#define BOLD_BRIGHT_GRAY	slc_from_cstr("\x1b[1;97m")
#define ITALIC				slc_from_cstr("\x1b[3m")
#define COLOR_RESET			slc_from_cstr("\x1b[0m")

#define STR_BUFFER slc_from_cstr("")

#define PRINT_PKGIT slc_cat(							\
	slc_cat(											\
		slc_cat(BOLD_YELLOW, slc_from_cstr("[")),		\
		slc_cat(BOLD_MAGENTA, slc_from_cstr("pkgit")),	\
		&STR_BUFFER										\
	), slc_cat(											\
		slc_cat(BOLD_MAGENTA, slc_from_cstr("]")),		\
		COLOR_RESET, &STR_BUFFER						\
	), &STR_BUFFER										\
)
#define PRINT_SUCCESS slc_cat(							\
	slc_cat(PRINT_PKGIT, GREEN),						\
	slc_cat(slc_from_cstr(" [SUCCESS]"), COLOR_RESET)	\
)
#define PRINT_SKIPPED slc_cat(							\
	slc_cat(PRINT_PKGIT, BLUE),							\
	slc_cat(slc_from_cstr(" [SKIPPED]"), COLOR_RESET)	\
)
#define PRINT_WARNING slc_cat(							\
	slc_cat(PRINT_PKGIT, YELLOW),						\
	slc_cat(slc_from_cstr(" [WARNING]"), COLOR_RESET)	\
)
#define PRINT_ERROR slc_cat(							\
	slc_cat(PRINT_PKGIT, RED),							\
	slc_cat(slc_from_cstr(" [ERROR]"), COLOR_RESET)		\
)

extern int is_verbose;
extern int is_forced;

#endif
