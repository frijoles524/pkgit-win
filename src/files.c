
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

#include <dirent.h>
#include <errno.h>
#ifdef __APPLE__
#define _DARWIN_C_SOURCE
#endif
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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

#define ERROR_NOT_A_SYMLINK (-2)

static int copy_symlink(int src_dir_fd, const char *src_path, int dst_dir_fd, const char *dst_path) {
  char symlink_target[PATH_MAX];
  ssize_t symlink_target_len = readlinkat(src_dir_fd, src_path, symlink_target, sizeof(symlink_target) - 1);
  if (symlink_target_len >= 0) {
    symlink_target[symlink_target_len] = '\0';

    if (symlinkat(symlink_target, dst_dir_fd, dst_path) < 0) {
      perror("symlinkat");
      return -1;
    }

    return 0;
  } else if (errno == EINVAL) {
    return ERROR_NOT_A_SYMLINK;
  } else {
    perror("readlinkat");
    return -1;
  }
}

static int copy_at(int src_dir_fd, const char *src_path, int dst_dir_fd, const char *dst_path) {
  int ret = copy_symlink(src_dir_fd, src_path, dst_dir_fd, dst_path);
  if (ret != ERROR_NOT_A_SYMLINK) {
    goto cleanup0;
  }

  ret = -1;
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
