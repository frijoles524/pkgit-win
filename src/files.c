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

#include "files.h"

#include "str.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>

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

FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);

str cmd_out(const char *cmd) {
    FILE *pipe = popen(cmd, "r");
    if (!pipe) return mstr("");

    char buffer[128];
    size_t total_size = 0;
    size_t capacity = 256;
    char *result = malloc(capacity);
    if (!result) {
        pclose(pipe);
        return mstr("");
    }
    result[0] = '\0';

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        size_t len = strlen(buffer);
        // Ensure enough space for the new chunk + 1 byte for '\0'
        if (total_size + len + 1 > capacity) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (!new_result) {
                free(result);
                pclose(pipe);
		        return mstr("");
            }
            result = new_result;
        }
 
        snprintf(result + total_size, capacity - total_size, "%s", buffer);
        total_size += len;
    }

    pclose(pipe);
    str str_result = mstr(result);
    free(result);
    return str_result;
}

const char *get_filename_ext(const char *filename) {
	const char *dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return dot + 1;
}

static int write_all(int fd, const void *buf, size_t size) {
	while (size) {
		ssize_t written = write(fd, buf, size);
		if (written < 0) {
			return -1;
		}

		size -= written;
		buf = (const char *)buf + written;
	}

	return 0;
}

static int copy_file_content(int src_fd, int dst_fd) {
	char buf[4096];
	ssize_t readed;
	while ((readed = read(src_fd, buf, sizeof(buf))) > 0) {
		if (write_all(dst_fd, buf, readed) < 0) {
			perror("write_all");
			return -1;
		}
	}

	if (readed < 0) {
		perror("read");
		return -1;
	}

	return 0;
}

static int copy_at(int src_dir_fd, const char *src_path, int dst_dir_fd, const char *dst_path);

static int copy_dir_content(int src_fd, int dst_fd) {
	int ret = -1;
	// duplicate the descriptor to prevent closedir from closing original one
	src_fd = dup(src_fd);
	if (src_fd < 0) {
		perror("dup");
		goto cleanup0;
	}

	DIR *src_dir = fdopendir(src_fd);
	if (!src_dir) {
		perror("fdopendir");
		close(src_fd);
		goto cleanup0;
	}

	for (struct dirent *ent; (errno = 0, ent = readdir(src_dir));) {
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
			continue;
		}

		if (copy_at(src_fd, ent->d_name, dst_fd, ent->d_name) < 0) {
			goto cleanup1;
		}
	}

	if (errno != 0) {
		perror("readdir");
		goto cleanup1;
	}

	ret = 0;

	cleanup1:
		closedir(src_dir);
	cleanup0:
		return ret;
}

static int copy_at(int src_dir_fd, const char *src_path, int dst_dir_fd, const char *dst_path) {
	int ret = -1;
	int src_fd = openat(src_dir_fd, src_path, O_NOFOLLOW | O_RDONLY);
	if (src_fd < 0) {
		perror("openat");
		goto cleanup0;
	}

	struct stat stat;
	if (fstat(src_fd, &stat) < 0) {
		perror("fstat");
		goto cleanup1;
	}

	int dst_oflag = O_NOFOLLOW;
	if (S_ISDIR(stat.st_mode)) {
		if (mkdirat(dst_dir_fd, dst_path, stat.st_mode) < 0) {
			perror("mkdirat");
			goto cleanup1;
		}

		dst_oflag |= O_RDONLY | O_DIRECTORY;
	} else {
		dst_oflag |= O_WRONLY | O_CREAT | O_EXCL;
	}

	int dst_fd = openat(dst_dir_fd, dst_path, dst_oflag, stat.st_mode);
	if (dst_fd < 0) {
		perror("openat");
		goto cleanup1;
	}

	switch (stat.st_mode & S_IFMT) {
		case S_IFDIR:
			ret = copy_dir_content(src_fd, dst_fd);
			break;
		case S_IFREG:
			ret = copy_file_content(src_fd, dst_fd);
			break;
		default:
			fprintf(stderr, "copy_at: unsupported file inode type\n");
	}
	goto cleanup2;

	cleanup2:
		close(dst_fd);
	cleanup1:
		close(src_fd);
	cleanup0:
		return ret;
}

void cpdir(const char *src_path, const char *dst_path) {
	copy_at(AT_FDCWD, src_path, AT_FDCWD, dst_path);
}
