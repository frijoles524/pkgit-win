#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lua_state.h"
#include "remove_pkg.h"
#include "vars.h"

static int remove_installed(const char *src_path, const struct stat *sb,
                            int typeflag, struct FTW *ftwbuf) {
  (void)sb;
  (void)ftwbuf;

  if (typeflag == FTW_F) {
    const char *filename = src_path + ftwbuf->base;
    const char *ext = strrchr(filename, '.');
    if (!ext)
      ext = "";

    if (strncmp(ext, ".so", 3) == 0) {
      char dest[MAX_PATH_LEN];
      snprintf(dest, sizeof(dest), "%s/%s", get_install_dir("lib"), filename);
      if (file_exists(dest))
        remove(dest);
    } else if (access(src_path, X_OK) == 0) {
      if (strcmp(ext, ".sample") != 0 && strcmp(filename, "bldit") != 0 &&
          strcmp(filename, "build.sh") != 0 &&
          strcmp(filename, "compile.sh") != 0) {
        char dest[MAX_PATH_LEN];
        snprintf(dest, sizeof(dest), "%s/%s", get_install_dir("bin"), filename);
        if (file_exists(dest))
          remove(dest);
      }
    } else if (strcmp(ext, ".h") == 0) {
      char dest[MAX_PATH_LEN];
      snprintf(dest, sizeof(dest), "%s/%s", get_install_dir("include"),
               filename);
      if (file_exists(dest))
        remove(dest);
    }
  }

  return 0;
}

static int remove_tree(const char *fpath, const struct stat *sb, int typeflag,
                       struct FTW *ftwbuf) {
  (void)sb;
  (void)ftwbuf;
  if (typeflag == FTW_F || typeflag == FTW_SL) {
    unlink(fpath);
  } else if (typeflag == FTW_DP) {
    rmdir(fpath);
  }
  return 0;
}

void remove_pkg(Pkg pkg) {
  if (!file_exists(pkg.src)) {
    printf("%s%s is not installed!\n", print_pkgit, pkg.name);
    return;
  }
  chdir(pkg.src);

  if (repo_uninstall(pkg.name)) return;
  if (bldit_uninstall(pkg.target)) return;
  if (config_uninstall(pkg.src)) return;

  printf("%sno uninstall function availible for package: %s\n",
    print_error, pkg.name);
  return;

  //nftw(pkg.src, remove_installed, 64, FTW_PHYS);
  //const char *last_slash = strrchr(pkg.src, '/');
  //char target[MAX_PATH_LEN];
  //if (last_slash && last_slash != pkg.src) {
  //  size_t parent_len = last_slash - pkg.src;
  //  snprintf(target, sizeof(target), "%.*s", (int)parent_len, pkg.src);
  //} else {
  //  snprintf(target, sizeof(target), "%s", pkg.src);
  //}
  //nftw(target, remove_tree, 64, FTW_DEPTH | FTW_PHYS);
  //printf("%sremoved %s\n", print_pkgit, pkg.name);
}