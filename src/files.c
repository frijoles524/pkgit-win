#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

const char *get_filename_ext(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if(!dot || dot == filename) return "";
  return dot + 1;
}

void cpdir(const char *path, const char *dest) {
  DIR *dirp = opendir(path);
  struct dirent *dirp_ent;

  struct stat dest_st;
  stat(dest, &dest_st);
  if (dest_st.st_mode && dest_st.st_mode == S_IFDIR) {
    mkdir(dest, dest_st.st_mode);
  }

  while ((dirp_ent = readdir(dirp))) {
    if (dirp_ent->d_name[0] == '.' &&
      (strlen(dirp_ent->d_name) == 1 ||
      (strlen(dirp_ent->d_name) == 2 &&
      dirp_ent->d_name[1] == '.'))
    ) continue;
    char rfile_line[1024];
    struct stat st;
    char *filename = malloc(sizeof(path)+sizeof(dirp_ent->d_name)+2);
    snprintf(
      filename,
      sizeof(path)+sizeof(dirp_ent->d_name)+2,
      "%s/%s",
      path, dirp_ent->d_name
    );
    printf("FILENAME: %s\n", filename);
    stat(filename, &st);
    if (st.st_mode) {
      char *newfile = malloc(sizeof(dest)+sizeof(dirp_ent->d_name)+2);
      snprintf(
        newfile,
        sizeof(dest)+sizeof(dirp_ent->d_name)+2,
        "%s/%s",
        dest, dirp_ent->d_name
      );
      if (st.st_mode == S_IFDIR) {
        printf("DIRECTORY\n");
        printf("NEWDIR: %s\n", newfile);
        mkdir(newfile, st.st_mode);
        cpdir(filename, newfile);
        continue;
      }
      FILE *rfile = fopen(dirp_ent->d_name, "r");
      FILE *wfile = fopen(newfile, "w+");
      if (!rfile || !wfile) { continue; }
      size_t ret = fread(
        rfile_line,
        //sizeof(*rfile_line),
        //sizeof(rfile_line)/sizeof(rfile_line[0]),
        1,
        1024,
        rfile
      );
      if (ret != sizeof(rfile_line)/sizeof(rfile_line[0]) && ferror(rfile)) {
        fprintf(stderr, "file read failed: %zu\n", ret);
        continue;
      }
      size_t write = fwrite(
        rfile_line,
        //sizeof(rfile_line[0]),
        1,
        ret,
        wfile
      );
      if (write != sizeof(rfile_line)/sizeof(rfile_line[0])) {
        fprintf(stderr, "file write failed: %zu\n", ret);
        continue;
      }
      if (feof(rfile)) {
        fclose(rfile);
        fclose(wfile);
        continue;
      }
      free(newfile);
    }
    free(filename);
  }
}
