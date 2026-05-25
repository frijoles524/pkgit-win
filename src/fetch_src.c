#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ftw.h>
#include <unistd.h>

#include "fetch_src.h"
#include "fetch_git.h"
#include "vars.h"

static int remove_tree(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    (void)sb;
    (void)ftwbuf;
    if (typeflag == FTW_F || typeflag == FTW_SL) {
        unlink(fpath);
    } else if (typeflag == FTW_DP) {
        rmdir(fpath);
    }
    return 0;
}

void fetch_src(Pkg pkg) {
    printf("%starget source directory: %s\n", print_pkgit, pkg.src);
    if (file_exists(pkg.src)) {
        printf("%s%s already exists. deleting...\n", print_pkgit, pkg.src);
        nftw(pkg.src, remove_tree, 64, FTW_DEPTH | FTW_PHYS);
    }
    if (strcmp(pkg.url, "") == 0) {
        printf("%screating directory %s...\n", print_pkgit, pkg.src);
        mkdir_p(pkg.src);
        return;
    }
    if (fetch_git(pkg) == 0) {
        printf("%scloned into %s...\n", print_pkgit, pkg.src);
        return;
    }
    printf("%sno fetch methods worked.\n", print_error);
    exit(EXIT_FAILURE);
}