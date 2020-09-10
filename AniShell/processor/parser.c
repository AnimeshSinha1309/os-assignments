#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../globals.h"
#include "../utils/string.h"
#include "../commands/cd.h"
#include "../commands/pinfo.h"
#include "../commands/pwd.h"
#include "../commands/ls.h"
#include "../commands/execvp.h"
#include "../processor/tokenizer.h"
#include "../commands/echo.h"
#include "../commands/history.h"
#include "../commands/nightswatch.h"


void await_input() {
    char* com_value = (char*)calloc(1, MAX_TOKENS_IN_COMMAND);
    int com_length = 0;
    while ((com_value[com_length++] = (char)getchar()) != '\n');
    com_value[com_length - 1] = 0;
    process_input(string_make(com_value));
}

bool shift_matches(const char* command, String input) {
    struct String com = string_make(command);
    int match = string_match(com, input);
    return (match == com.length && (input.length == com.length || input.c_str[com.length] == ' '));
}

String clean_input(String input) {
    String output = string_empty();
    for (int i = 0; i < input.length; i++) {
        if (input.c_str[i] != ' ' || (output.length != 0 && output.c_str[output.length - 1] != ' '))
            output.c_str[output.length++] = input.c_str[i];
    }
    return output;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
void process_input(String input) {
    input = clean_input(input);
    write_history(input);
    if (shift_matches("cd", input)) {
        string_pop_front(&input, ' ');
        cd(string_peek_front(input, ' '));
    } else if (shift_matches("echo", input)) {
        string_pop_front(&input, ' ');
        echo(input);
        printf("\n");
    } else if (shift_matches("history", input)) {
        string_pop_front(&input, ' ');
        if (input.length == 0)
            history(HISTORY_PRINT_DEFAULT);
        else
            history(strtol(string_peek_front(input, ' ').c_str, NULL, 10));
    } else if (shift_matches("nightswatch", input)) {
        string_pop_front(&input, ' ');
        string_pop_front(&input, ' ');
        int interval = (int)strtol(string_peek_front(input, ' ').c_str, NULL, 10);
        string_pop_front(&input, ' ');
        bool cpu_mode = shift_matches("interrupt", input);
        nightswatch(cpu_mode, interval);
    } else if (shift_matches("pwd", input)) {
        string_pop_front(&input, ' ');
        pwd(string_peek_front(input, ' '));
        printf("\n");
    } else if (shift_matches("pinfo", input)) {
        string_pop_front(&input, ' ');
        if (input.length == 0) pinfo(string_empty());
        else pinfo(string_peek_front(input, ' '));
        printf("\n");
    } else if (shift_matches("ls", input)) {
        bool l = false, a = false;
        string_pop_front(&input, ' ');
        Strmat args = tokenize_args(input);
        for (int i = 0; i < args.length; i++) {
            if (args.lengths[i] > 0 && args.c_arr[i][0] == '-') {
                for (int j = 1; j < args.lengths[i]; j++) {
                    if (args.c_arr[i][j] == 'l') l = true;
                    if (args.c_arr[i][j] == 'a') a = true;
                }
            }
        }
        bool printed_something = false;
        for (int i = 0; i < args.length; i++) {
            if (args.lengths[i] > 0 && args.c_arr[i][0] != '-') {
                ls(string_cut(args.c_arr[i], args.lengths[i]), a, l);
                printed_something = true;
            }
        }
        if (!printed_something) ls(string_make("~"), a, l);
    } else {
        char* bg_marker = strchr(input.c_str, '&');
        if (bg_marker != NULL) { input.length = (int)bg_marker - (int)input.c_str; *bg_marker = '\0'; }
        Strmat args = tokenize_args(input);
        if (string_peek_front(input, ' ').length > 1)
            exec(string_peek_front(input, ' ').c_str, args.c_arr, bg_marker != NULL);
    }
}
#pragma clang diagnostic pop
