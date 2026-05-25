#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "fetch_git.h"
#include "vars.h"

int fetch_git(Pkg pkg) {
    pid_t pid = fork();
    if (pid == 0) {
        const char *argv[8];
        int i = 0;
        argv[i++] = "git";
        argv[i++] = "-c";
        argv[i++] = "advice.detachedHead=false";
        argv[i++] = "clone";
        if (strcmp(pkg.ver, "HEAD") != 0) {
            argv[i++] = "--branch";
            argv[i++] = pkg.ver;
        }
        argv[i++] = pkg.url;
        argv[i++] = pkg.src;
        argv[i] = NULL;
        execvp("git", (char *const *)argv);
        _exit(127);
    }

    int status;
    waitpid(pid, &status, 0);
    int result = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    if (result != 0) {
        printf("clone failed\n");
    }

    return result;
}