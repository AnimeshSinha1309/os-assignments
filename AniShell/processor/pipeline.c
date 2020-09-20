#include "pipeline.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>


void pipeline(String input) {
    int *buffer_1 = calloc(sizeof(int), 2), *buffer_2 = calloc(sizeof(int), 2);
    Strmat commands = tokenize_str(input, "|");
    const int BUFFER_INPUT = 0, BUFFER_OUTPUT = 1;
    for (int i = 0; i < commands.length; i++) {
        int *current_buffer = i % 2 == 0 ? buffer_1 : buffer_2, *other_buffer = i % 2 == 0 ? buffer_2 : buffer_1;
        if (pipe(current_buffer) < 0) {
            perror("Error in pipeline, pipe returned negative integer");
            return;
        }
        pid_t pid = fork();
        if (pid < 0) {
            perror("Couldn't fork sub-processes in pipeline");
            return;
        } else if (pid == 0) {
            if (i < commands.length - 1) dup2(current_buffer[BUFFER_OUTPUT], STDOUT_FILENO);
            if (i > 0) dup2(other_buffer[BUFFER_INPUT], STDIN_FILENO);
            process_input(strmat_get(commands, i));
            exit(0);
        } else {
            wait(NULL);
            if (i > 0) close(other_buffer[BUFFER_INPUT]);
            if (i < commands.length - 1) close(current_buffer[BUFFER_OUTPUT]);
        }
    }
}