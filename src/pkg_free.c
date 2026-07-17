#include "pkg.h"
#include "globs.h"

void pkg_free(package_t *pkg) {
	str_free(&pkg->name);
	str_free(&pkg->url);
	str_free(&pkg->version);
	str_free(&pkg->target);
	str_free(&pkg->src);
}
