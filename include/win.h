#ifndef WIN_COMPAT_H
#ifdef __CYGWIN__
#include <windows.h>
#include <sys/cygwin.h>
#define LUA_OK 0

static char *to_windows_path(const char *unix_path) { // bad practice but oh well
    ssize_t size = cygwin_conv_path(CCP_POSIX_TO_WIN_A, unix_path, NULL, 0);
    if (size < 0) return NULL;

    char *win_path = malloc(size);
    if (!win_path) return NULL;

    if (cygwin_conv_path(CCP_POSIX_TO_WIN_A, unix_path, win_path, size) != 0) {
        free(win_path);
        return NULL;
    }

    return win_path;
}
#define WIN_COMPAT_H
#endif
#endif