#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include "vars.h"

bool is_symlink_install = false;
bool is_verbose = true;
bool is_auto_installed = true;
bool config_exists = false;

char home_dir[MAX_PATH_LEN] = {0};

char root_config[MAX_PATH_LEN] = "/etc/pkgit/init.lua";
bool is_root_config = false;
char config_dir[MAX_PATH_LEN] = {0};
char config_file[MAX_PATH_LEN] = {0};
char repo_file[MAX_PATH_LEN] = {0};

char bin[MAX_PATH_LEN] = {0};
char lib[MAX_PATH_LEN] = {0};
char inc[MAX_PATH_LEN] = {0};
char src[MAX_PATH_LEN] = {0};

char *install_directories[5] = {NULL};

Map cached_install_directories = {0};
Repo cached_repos[MAX_REPOS];
size_t cached_repos_count = 0;
Map cached_build_systems = {0};

char home_config_buf[MAX_PATH_LEN] = {0};
char* home_config() {
    snprintf(home_config_buf, MAX_PATH_LEN, "%s/.config/pkgit/init.lua", home_dir);
    return home_config_buf;
}

const char* get_install_dir(const char *key) {
    for (size_t i = 0; i < cached_install_directories.size; i++) {
        if (strcmp(cached_install_directories.items[i].key, key) == 0) {
            return cached_install_directories.items[i].value;
        }
    }
    return "";
}

const char *version = "0.1.2";

const char *red = "\e[0;31m";
const char *green = "\e[0;32m";
const char *yellow = "\e[0;33m";
const char *blue = "\e[0;34m";
const char *magenta = "\e[0;35m";
const char *cyan = "\e[0;36m";
const char *gray = "\e[0;37m";
const char *bright_red = "\e[0;91m";
const char *bright_green = "\e[0;92m";
const char *bright_yellow = "\e[0;93m";
const char *bright_blue = "\e[0;94m";
const char *bright_magenta = "\e[0;95m";
const char *bright_cyan = "\e[0;96m";
const char *bright_gray = "\e[0;97m";
const char *bold_red = "\e[1;31m";
const char *bold_green = "\e[1;32m";
const char *bold_yellow = "\e[1;33m";
const char *bold_blue = "\e[1;34m";
const char *bold_magenta = "\e[1;35m";
const char *bold_cyan = "\e[1;36m";
const char *bold_gray = "\e[1;37m";
const char *bold_white = "\e[1;38m";
const char *bold_bright_red = "\e[1;91m";
const char *bold_bright_green = "\e[1;92m";
const char *bold_bright_yellow = "\e[1;93m";
const char *bold_bright_blue = "\e[1;94m";
const char *bold_bright_magenta = "\e[1;95m";
const char *bold_bright_cyan = "\e[1;96m";
const char *bold_bright_gray = "\e[1;97m";
const char *italic = "\e[3m";
const char *color_reset = "\e[0m";

const char *print_pkgit;
const char *print_success;
const char *print_skipped;
const char *print_warning;
const char *print_error;

int mkdir_p(const char *path) {
    char tmp[MAX_PATH_LEN];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if (tmp[len - 1] == '/') {
        tmp[len - 1] = 0;
    }

    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            if (mkdir(tmp, 0755) != 0 && errno != EEXIST) {
                return -1;
            }
            *p = '/';
        }
    }

    if (mkdir(tmp, 0755) != 0 && errno != EEXIST) {
        return -1;
    }

    return 0;
}

bool file_exists(const char *path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

bool is_directory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0) {
        return false;
    }
    return S_ISDIR(statbuf.st_mode);
}

void init_vars() {
    char *home = getenv("HOME");
    if (home) {
        snprintf(home_dir, MAX_PATH_LEN, "%s", home);
    } else {
        snprintf(home_dir, MAX_PATH_LEN, "/root");
    }

    is_root_config = file_exists(root_config);

    if (is_root_config) {
        snprintf(config_dir, MAX_PATH_LEN, "/etc/pkgit");
    } else {
        snprintf(config_dir, MAX_PATH_LEN, "%s/.config/pkgit", home_dir);
    }

    snprintf(config_file, MAX_PATH_LEN, "%s/init.lua", config_dir);
    snprintf(repo_file, MAX_PATH_LEN, "%s/repos.lua", config_dir);

    config_exists = file_exists(root_config) || file_exists(home_config());

    snprintf(bin, MAX_PATH_LEN, "%s/.local/bin", home_dir);
    snprintf(lib, MAX_PATH_LEN, "%s/.local/lib", home_dir);
    snprintf(inc, MAX_PATH_LEN, "%s/.local/include", home_dir);
    snprintf(src, MAX_PATH_LEN, "%s/.local/share/pkgit", home_dir);

    install_directories[0] = config_dir;
    install_directories[1] = strdup(get_install_dir("bin"));
    install_directories[2] = strdup(get_install_dir("lib"));
    install_directories[3] = strdup(get_install_dir("inc"));
    install_directories[4] = strdup(get_install_dir("src"));

    static char print_pkgit_buf[256];
    snprintf(print_pkgit_buf, sizeof(print_pkgit_buf), "%s[%s%s%s] %s",
             bold_yellow, bold_magenta, "pkgit", bold_yellow, color_reset);
    print_pkgit = print_pkgit_buf;

    static char print_success_buf[256];
    snprintf(print_success_buf, sizeof(print_success_buf), "%s%s[SUCCESS] %s",
             print_pkgit, green, color_reset);
    print_success = print_success_buf;

    static char print_skipped_buf[256];
    snprintf(print_skipped_buf, sizeof(print_skipped_buf), "%s%s[SKIP] %s",
             print_pkgit, blue, color_reset);
    print_skipped = print_skipped_buf;

    static char print_warning_buf[256];
    snprintf(print_warning_buf, sizeof(print_warning_buf), "%s%s[WARN] %s",
             print_pkgit, yellow, color_reset);
    print_warning = print_warning_buf;

    static char print_error_buf[256];
    snprintf(print_error_buf, sizeof(print_error_buf), "%s%s[ERROR] %s",
             print_pkgit, red, color_reset);
    print_error = print_error_buf;
}
