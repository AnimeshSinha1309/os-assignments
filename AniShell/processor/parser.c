#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include "../globals.h"
#include "../utils/string.h"
#include "../commands/cd.h"

void await_input() {
    char* com_value = (char*)calloc(1, MAX_TOKENS_IN_COMMAND);
    int com_length = 0;
    while ((com_value[com_length++] = (char)getchar()) != '\n');
    com_value[com_length - 1] = 0;
    process_input(string_make(com_value));
}

bool shift_matches(const char* command, struct String input) {
    struct String com = string_make(command);
    int match = string_match(com, input);
    return (match == com.length && (input.length == com.length || input.c_str[com.length] == ' '));
}

void process_input(struct String input) {
    if (shift_matches("cd", input)) {
        string_pop_front(&input, ' ');
        cd(string_peek_front(input, ' '));
    } else if (shift_matches("ls", input)) {
        printf("LISTDIR\n");
    } else {
        printf("Could not recognize command %s\n", string_peek_front(input, ' ').c_str);
    }
}
