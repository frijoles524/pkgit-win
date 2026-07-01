
#include <stdarg.h>
#include <stdio.h>

#include "globs.h"
#include "log.h"

#define X(n, N)                                                                \
	void log_##n(const char *format, ...) {                                    \
		va_list args;                                                          \
		va_start(args, format);                                                \
		eprintf(PKGIT_PREFIX_##N);                                             \
		vfprintf(stderr, format, args);                                        \
		eprintf("\n");                                                         \
		va_end(args);                                                          \
	}

LOG_FUNCTIONS
#undef X
