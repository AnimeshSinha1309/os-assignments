#include "history.h"

#include <bits/types/FILE.h>
#include <stdio.h>
#include "../processor/prompt.h"


void write_history(String str) {
    char* f_path = string_join(home_path.c_str, "/history.txt").c_str;
    FILE* f_ptr = fopen(f_path, "a");
    if (f_ptr < 0) {
        perror("Could not read from history file.");
        return;
    }
    fputs(string_join(str.c_str, "\n").c_str, f_ptr);
    fclose(f_ptr);
}

void history() {
    char buffer[MAX_TOKENS_IN_COMMAND];
    char* f_path = string_join(home_path.c_str, "/history.txt").c_str;
    FILE* f_ptr = fopen(f_path, "r");
    if (f_ptr < 0) {
        perror("Could not read from history file.");
        return;
    }
    while(fgets(buffer, MAX_TOKENS_IN_COMMAND, f_ptr))
        printf("%s", buffer);
    fclose(f_ptr);
}
