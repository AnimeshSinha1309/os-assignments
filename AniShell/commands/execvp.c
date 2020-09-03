#include "execvp.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void exec(char* command, char** args, bool background) {
    int child = fork();

    if (child == -1) {
        perror("Could not fork child");
    } else if (child == 0) {
        // If I am the child, I execute
        setpgid(0, 0);
        if (execvp(command, args) < 0)
            perror("System Command Execution Terminated Unexpectedly");
        exit(0);
    } else {
        // If I am the parent, I wait
        if (!background) {
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            setpgid(child, 0);
            tcsetpgrp(0, __getpgid(child));
            int status;
            waitpid(child, &status, WUNTRACED);
            tcsetpgrp(0, getpgrp());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
        } else {
            setpgid(child, 0);
            tcsetpgrp(0, getpgrp());
        }
    }
}