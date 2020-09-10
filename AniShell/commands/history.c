#include "history.h"

#include <bits/types/FILE.h>
#include <stdio.h>
#include "../processor/prompt.h"
#include "../utils/strmat.h"

void write_history(String str) {
    if (str.length <= 1) return;
    Strmat history_memory = load_history();
    strmat_put(&history_memory, string_join(str.c_str, "\n").c_str);
    save_history(history_memory);
}

Strmat load_history() {
    char buffer[MAX_TOKENS_IN_COMMAND];
    char* f_path = string_join(home_path.c_str, "/history.txt").c_str;
    FILE* f_ptr = fopen(f_path, "r");
    Strmat history_memory = strmat_empty();
    if (f_ptr == NULL) return history_memory;
    while(fgets(buffer, MAX_TOKENS_IN_COMMAND, f_ptr))
        strmat_put(&history_memory, buffer);
    fclose(f_ptr);
    return history_memory;
}

void save_history(Strmat history_memory) {
    char* f_path = string_join(home_path.c_str, "/history.txt").c_str;
    FILE* f_ptr = fopen(f_path, "w");
    if (f_ptr < 0) {
        perror("Could not write to history file.");
        return;
    }
    int start = history_memory.length > HISTORY_STORAGE_SIZE ? history_memory.length - HISTORY_STORAGE_SIZE : 0;
    for (int i = start; i < history_memory.length; i++) {
        fputs(history_memory.c_arr[i], f_ptr);
    }
    fclose(f_ptr);
}

void history(int items) {
    Strmat history_memory = load_history();
    int start = history_memory.length > items ? history_memory.length - items : 0;
    for (int i = start; i < history_memory.length; i++) {
        printf("%s", history_memory.c_arr[i]);
    }
}
