#include "log.h"

#include <execinfo.h>
#include <stdlib.h>
#include <unistd.h>

void do_stacktrace(int log_level) {
    if (log_level < LOG_LEVEL_ERROR) return;
    void * frames[256];

    i32 num_frames = backtrace(frames, sizeof(frames) / sizeof(frames[0]));

    char ** desc = backtrace_symbols(frames, num_frames);

    printf("Stacktrace: \n");
    for (i32 i = 0; i < num_frames; i++) {
        fprintf(stderr, "    %s\n", desc[i]);
    }

    free(desc);
}
