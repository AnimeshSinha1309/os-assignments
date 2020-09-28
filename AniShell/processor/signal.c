#include "signal.h"

#include <signal.h>

void signal_c() {
    if (current_process >= 0)
        raise(SIGINT);
}

void signal_z() {
    if (current_process >= 0)
        raise(SIGTSTP);
}

void initialize_signals() {
    signal(SIGINT, signal_c);
    signal(SIGTSTP, signal_z);
}