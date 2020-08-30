#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include "../globals.h"

void await_input() {
    char* com_value = (char*)calloc(1, MAX_TOKENS_IN_COMMAND);
    int com_length = 0;
    char temp_input;
    while ((temp_input = (char)getchar()) != '\n')
        com_value[com_length++] = temp_input;
}

void process_input() {

}
