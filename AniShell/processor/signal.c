#include "signal.h"

#include <signal.h>
#include <unistd.h>

void signal_c() {
    if (current_process >= 0 && current_process != getpid())
        raise(SIGINT);
}

void signal_z() {
    if (current_process >= 0 && current_process != getpid())
        raise(SIGTSTP);
}

void initialize_signals() {
    current_process = -1;
    signal(SIGINT, signal_c);
    signal(SIGTSTP, signal_z);
}