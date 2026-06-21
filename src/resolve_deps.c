#include <stdio.h>
#include <unistd.h>

#include "resolve_deps.h"

void resolve_deps(void) {
    const char *frame_top = "  (c _c)";
    const char *frame1_bot = "_/      \\-";
    const char *frame2_bot = "-/      \\_";

    printf("\033[2J\033[H");
    printf("Unfortunately due to budget issues, we could not afford a progress bar. Enjoy this instead:\n\n");

    for (int i = 0; i < 16; i++) {
        printf("%s\n%s\n", frame_top, i % 2 == 0 ? frame1_bot : frame2_bot);
        for (int j = 0; j <= i; j++) {
            printf(".");
        }
        printf("\n");
        fflush(stdout);
        sleep(1);
        if (i < 15) {
            printf("\033[3A");
        }
    }
    printf("\n");
    printf("Dependencies resolved! (this does nothing)\n");
}
