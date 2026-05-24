#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "copy_install.h"
#include "vars.h"

static int is_executable(const char *path) { return (access(path, X_OK) == 0); }

static int is_dir(const char *path) {
  struct stat st;
  if (stat(path, &st) == 0) {
    return S_ISDIR(st.st_mode);
  }
  return 0;
}

static const char *get_extension(const char *path) {
  const char *dot = strrchr(path, '.');
  return dot ? dot : "";
}

static int copy_file(const char *src, const char *dst) {
  struct stat st;
  if (stat(src, &st) != 0)
    return -1;

  int src_fd = open(src, O_RDONLY);
  if (src_fd < 0)
    return -1;

  int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, st.st_mode & 07777);
  if (dst_fd < 0) {
    close(src_fd);
    return -1;
  }

  char buf[8192];
  ssize_t n;
  while ((n = read(src_fd, buf, sizeof(buf))) > 0) {
    ssize_t written = 0;
    while (written < n) {
      ssize_t ret = write(dst_fd, buf + written, n - written);
      if (ret < 0) {
        close(src_fd);
        close(dst_fd);
        return -1;
      }
      written += ret;
    }
  }

  close(src_fd);
  close(dst_fd);
  return n == 0 ? 0 : -1;
}

static void copy_install_recursive(const char *dir_path) {
  DIR *dir = opendir(dir_path);
  if (!dir)
    return;

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char src_path[MAX_PATH_LEN];
    snprintf(src_path, sizeof(src_path), "%s/%s", dir_path, entry->d_name);

    if (is_dir(src_path)) {
      if (strcmp(entry->d_name, ".git") != 0) {
        copy_install_recursive(src_path);
      }
      continue;
    }

    const char *ext = get_extension(entry->d_name);

    if (strncmp(ext, ".so", 3) == 0) {
      char dest_path[MAX_PATH_LEN];
      snprintf(dest_path, sizeof(dest_path), "%s/%s", lib, entry->d_name);
      if (!file_exists(dest_path)) {
        copy_file(src_path, dest_path);
        if (is_verbose) {
          printf("%scopied library: %s\n", print_pkgit, src_path);
        }
      }
    } else if (is_executable(src_path)) {
      if (strcmp(ext, ".sample") != 0 && strcmp(entry->d_name, "bldit") != 0 &&
          strcmp(entry->d_name, "build.sh") != 0 &&
          strcmp(entry->d_name, "compile.sh") != 0) {
        char dest_path[MAX_PATH_LEN];
        snprintf(dest_path, sizeof(dest_path), "%s/%s", bin, entry->d_name);
        if (!file_exists(dest_path)) {
          copy_file(src_path, dest_path);
          if (is_verbose) {
            printf("%scopied executable: %s\n", print_pkgit, src_path);
          }
        }
      }
    } else if (strcmp(ext, ".h") == 0) {
      char dest_path[MAX_PATH_LEN];
      snprintf(dest_path, sizeof(dest_path), "%s/%s", inc, entry->d_name);
      if (!file_exists(dest_path)) {
        copy_file(src_path, dest_path);
        if (is_verbose) {
          printf("%scopied include: %s\n", print_pkgit, src_path);
        }
      }
    }
  }

  closedir(dir);
}

void copy_install(const char *build_dir) {
  copy_install_recursive(build_dir);
}
