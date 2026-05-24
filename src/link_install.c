#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#include "link_install.h"
#include "vars.h"

static int is_executable(const char *path) {
    return (access(path, X_OK) == 0);
}

static int is_dir(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
    return 0;
}

static const char* get_extension(const char *path) {
    const char *dot = strrchr(path, '.');
    return dot ? dot : "";
}

void link_install(const char *build_dir) {
    DIR *dir = opendir(build_dir);
    if (!dir) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char src_path[MAX_PATH_LEN];
        char link_path[MAX_PATH_LEN];
        snprintf(src_path, sizeof(src_path), "%s/%s", build_dir, entry->d_name);

        const char *ext = get_extension(entry->d_name);

        if (strncmp(ext, ".so", 3) == 0) {
            snprintf(link_path, sizeof(link_path), "%s/%s", lib, entry->d_name);
            if (!file_exists(link_path)) {
                symlink(src_path, link_path);
                if (is_verbose) {
                    printf("%slinked library: %s\n", print_pkgit, src_path);
                }
            }
        } else if (is_executable(src_path) && !is_dir(src_path)) {
            if (strcmp(entry->d_name, "bldit") != 0 &&
                strcmp(entry->d_name, "build.sh") != 0 &&
                strcmp(entry->d_name, "compile.sh") != 0) {
                snprintf(link_path, sizeof(link_path), "%s/%s", bin, entry->d_name);
                if (!file_exists(link_path)) {
                    symlink(src_path, link_path);
                    if (is_verbose) {
                        printf("%slinked executable: %s\n", print_pkgit, src_path);
                    }
                }
            }
        } else if (strcmp(ext, ".h") == 0) {
            snprintf(link_path, sizeof(link_path), "%s/%s", inc, entry->d_name);
            if (!file_exists(link_path)) {
                symlink(src_path, link_path);
                if (is_verbose) {
                    printf("%slinked include: %s\n", print_pkgit, src_path);
                }
            }
        }
    }

    closedir(dir);
}