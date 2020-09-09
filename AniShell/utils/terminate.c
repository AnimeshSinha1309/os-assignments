#include "terminate.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void setup_exit() {
    signal(SIGCHLD, exit_status);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
void exit_status() {
    int proc_stat, proc_id = waitpid(-1, &proc_stat, WNOHANG);
    if (proc_id <= 0) return;
    WIFEXITED(proc_stat) && WEXITSTATUS(proc_stat) == EXIT_SUCCESS
            ? printf("\n%s with pid %d exited normally ", process_name, proc_id)
            : printf("\n%s with pid %d exited with error code %d ", process_name, proc_id, WEXITSTATUS(proc_stat));
    fflush(stdout);
}
#pragma clang diagnostic pop
