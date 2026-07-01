#ifndef PKGIT_LOG_H
#define PKGIT_LOG_H

// XXX: im lazy.
#define PKGIT_PREFIX_PKGIT PKGIT_PREFIX
#define LOG_FUNCTIONS                                                          \
	X(error, ERROR)                                                            \
	X(warn, WARNING)                                                           \
	X(info, INFO)                                                              \
	X(success, SUCCESS)                                                        \
	X(pkgit, PKGIT)

#endif

#define X(n, N) void log_##n(const char *format, ...);
LOG_FUNCTIONS
#undef X
