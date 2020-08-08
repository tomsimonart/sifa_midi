#include <stdio.h>
#include <signal.h>

#include "mputil.h"
#include "joystick.h"

static volatile int keepRunning = 1;

void intHandler(int signal) {
    #ifdef WIN32
        UNREF_PARAM(signal);
    #else
        (void) signal;
    #endif
    keepRunning = 0;
}

int main() {
    printf("TSW ADVANCED CONTROLLER\n");

    signal(SIGINT, intHandler);

    return start_joystick(keepRunning);
}