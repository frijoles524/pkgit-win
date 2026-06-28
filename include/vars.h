/*

  pkgit - package it!

  Copyright (C) 2026 dacctal
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef VARS_H
#define VARS_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_REPOS 1000
#define MAX_DIRS 100
#define MAX_PATH_LEN 1024

typedef struct {
    char *key;
    char *value;
} KeyValue;

typedef struct {
    char *key;
    char *value;
} MapItem;

typedef struct {
    MapItem *items;
    size_t size;
    size_t capacity;
} Map;

typedef struct {
    char *url;
    char *version;
} Dependency;

typedef struct {
    char *source_key;
    char *source_value;
    char *version;
    Dependency *dependencies;
    size_t dep_count;
    int build_ref;
    int pre_install_ref;
    int install_ref;
    int post_install_ref;
} Repo;

typedef struct {
    char *url;
    char *name;
    const char *target;
    char *ver;
    char src[MAX_PATH_LEN];
    bool is_local;
} Pkg;

extern bool is_symlink_install;
extern bool is_verbose;
extern bool is_auto_installed;
extern bool is_forced;
extern bool config_exists;

extern char home_dir[MAX_PATH_LEN];
extern char prefix_dir[MAX_PATH_LEN];

extern char root_config[MAX_PATH_LEN];
extern bool is_root_config;
extern char config_dir[MAX_PATH_LEN];
extern char config_file[MAX_PATH_LEN];
extern char repo_file[MAX_PATH_LEN];

extern char bin[MAX_PATH_LEN];
extern char lib[MAX_PATH_LEN];
extern char inc[MAX_PATH_LEN];
extern char src[MAX_PATH_LEN];

extern char *install_directories[5];

extern const char *version;

extern const char *red;
extern const char *green;
extern const char *yellow;
extern const char *blue;
extern const char *magenta;
extern const char *cyan;
extern const char *gray;
extern const char *bright_red;
extern const char *bright_green;
extern const char *bright_yellow;
extern const char *bright_blue;
extern const char *bright_magenta;
extern const char *bright_cyan;
extern const char *bright_gray;
extern const char *bold_red;
extern const char *bold_green;
extern const char *bold_yellow;
extern const char *bold_blue;
extern const char *bold_magenta;
extern const char *bold_cyan;
extern const char *bold_gray;
extern const char *bold_white;
extern const char *bold_bright_red;
extern const char *bold_bright_green;
extern const char *bold_bright_yellow;
extern const char *bold_bright_blue;
extern const char *bold_bright_magenta;
extern const char *bold_bright_cyan;
extern const char *bold_bright_gray;
extern const char *italic;
extern const char *color_reset;

extern const char *print_pkgit;
extern const char *print_success;
extern const char *print_skipped;
extern const char *print_warning;
extern const char *print_error;

void init_vars();
const char* get_install_dir(const char *key);
bool file_exists(const char *path);
bool is_directory(const char *path);
int mkdir_p(const char *path);

void map_init(Map *map);
void map_put(Map *map, char *key, char *value);
char* map_get(Map *map, const char *key);

#endif