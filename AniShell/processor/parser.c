#include "parser.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../globals.h"
#include "../utils/string.h"
#include "../commands/cd.h"
#include "../commands/pwd.h"
#include "../commands/ls.h"
#include "../commands/execvp.h"
#include "../processor/tokenizer.h"


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
    } else if (shift_matches("pwd", input)) {
        string_pop_front(&input, ' ');
        pwd(string_peek_front(input, ' '));
        printf("\n");
    } else if (shift_matches("ls", input)) {
        bool l = 0, a = 0;
        string_pop_front(&input, ' ');
        Strmat args = tokenize_args(input);
        for (int i = 0; i < args.length; i++) {
            if (args.lengths[i] > 0 && args.c_arr[i][0] == '-') {
                for (int j = 1; j < args.lengths[i]; j++) {
                    if (args.c_arr[i][j] == 'l') l = 1;
                    if (args.c_arr[i][j] == 'a') a = 1;
                }
            }
        }
        int printed_something = false;
        for (int i = 0; i < args.length; i++) {
            if (args.lengths[i] > 0 && args.c_arr[i][0] != '-') {
                ls(process_path(string_cut(args.c_arr[i], args.lengths[i])), a, l);
                printed_something = true;
            }
        }
        if (!printed_something) ls(process_path(string_make("~")), a, l);
    } else {
        Strmat args = tokenize_args(input);
        exec(string_peek_front(input, ' ').c_str, args.c_arr, 0);
    }
}
