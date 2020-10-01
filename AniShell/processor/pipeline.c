#include "pipeline.h"
#include "prompt.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/fcntl.h>

void run_statement(String input) {
    Strmat commands = tokenize_str(input, ";");
    for (int i = 0; i < commands.length; i++) {
        run_expression_and(strmat_get(commands, i));
    }
}

void run_expression_and(String input) {
    Strmat commands = tokenize_str(input, "@");
    for (int i = 0; i < commands.length; i++) {
        run_expression_or(strmat_get(commands, i));
        if (exit_code == 0) break;
    }
}

void run_expression_or(String input) {
    Strmat commands = tokenize_str(input, "$");
    for (int i = 0; i < commands.length; i++) {
        pipeline(strmat_get(commands, i));
        if (exit_code != 0) break;
    }
}

void pipeline(String input) {
    int *buffer_1 = calloc(2, sizeof(int)), *buffer_2 = calloc(2, sizeof(int));
    Strmat commands = tokenize_str(input, "|");
    const int BUFFER_INPUT = 0, BUFFER_OUTPUT = 1;
    if (commands.length == 1) {
        redirections(strmat_get(commands, 0));
        return;
    }
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
            redirections(strmat_get(commands, i));
            exit(0);
        } else {
            wait(NULL);
            if (i > 0) close(other_buffer[BUFFER_INPUT]);
            if (i < commands.length - 1) close(current_buffer[BUFFER_OUTPUT]);
        }
    }
    free(buffer_1);
    free(buffer_2);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

void redirections(String input) {
    // Parse the input
    char *file_in = calloc(MAX_LETTERS_IN_TOKEN, sizeof(char)), *file_out = calloc(MAX_LETTERS_IN_TOKEN, sizeof(char));
    char *command = calloc(MAX_LETTERS_IN_TOKEN, sizeof(char));
    bool append_mode = false;
    int len_in = 0, len_out = 0, len_command = 0, reading_mode = 0;
    for (int i = 0; i < input.length; i++) {
        if (input.c_str[i] == '<') {
            reading_mode = 1;
        } else if (input.c_str[i] == '>') {
            reading_mode = 2;
            if (input.c_str[i + 1] == '>') i++, append_mode = true;
        } else if (reading_mode == 0) {
            command[len_command++] = input.c_str[i];
        } else if (reading_mode == 1 && input.c_str[i] != ' ') {
            file_in[len_in++] = input.c_str[i];
        } else if (reading_mode == 2 && input.c_str[i] != ' ') {
            file_out[len_out++] = input.c_str[i];
        }
    }
    command[len_command] = 0, file_in[len_in] = 0, file_out[len_out] = 0;
    // Open the files for writing
    int dup_0 = dup(0), dup_1 = dup(1);
    int fd_1 = -1, fd_2 = -1;
    if (len_in != 0) {
        fd_1 = open(file_in, O_RDONLY, 0644);
        if (fd_1 < 0) {
            perror("Could not open Input file");
            return;
        }
        dup2(fd_1, STDIN_FILENO);
    }
    if (len_out != 0) {
        unsigned int open_mode = (append_mode ? O_APPEND : O_TRUNC);
        fd_2 = open(file_out, O_CREAT | O_WRONLY | open_mode, 0644);
        if (fd_2 < 0) {
            perror("Could not open Output file");
            return;
        }
        dup2(fd_2, STDOUT_FILENO);
    }
    // Call the function
    process_input(string_make(command));
    // Close and restore output
    if (fd_1 >= 0) close(fd_1);
    if (fd_2 >= 0) close(fd_2);
    dup2(dup_0, STDIN_FILENO);
    dup2(dup_1, STDOUT_FILENO);
}

#pragma clang diagnostic pop