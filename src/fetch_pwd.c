#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ftw.h>
#include <unistd.h>

#include "fetch_pwd.h"
#include "vars.h"

static const char *pwd_dst;

static int copy_entry(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    (void)ftwbuf;
    const char *rel = fpath;
    if (rel[0] == '.' && rel[1] == '/') rel += 2;
    if (rel[0] == '\0') return 0;

    char dst[MAX_PATH_LEN];
    snprintf(dst, sizeof(dst), "%s/%s", pwd_dst, rel);

    if (typeflag == FTW_D) {
        mkdir(dst, 0755);
    } else if (typeflag == FTW_F) {
        int src_fd = open(fpath, O_RDONLY);
        if (src_fd < 0) return -1;

        int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, sb->st_mode & 07777);
        if (dst_fd < 0) { close(src_fd); return -1; }

        char buf[8192];
        ssize_t n;
        while ((n = read(src_fd, buf, sizeof(buf))) > 0) {
            ssize_t written = 0;
            while (written < n) {
                ssize_t ret = write(dst_fd, buf + written, n - written);
                if (ret < 0) { close(src_fd); close(dst_fd); return -1; }
                written += ret;
            }
        }

        close(src_fd);
        close(dst_fd);
    }
    return 0;
}

void fetch_pwd(Pkg pkg) {
    pwd_dst = pkg.src;
    nftw(".", copy_entry, 64, FTW_PHYS);
}