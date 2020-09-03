#include "tokenizer.h"

#include <string.h>
#include "prompt.h"

Strmat tokenize_args(String input) {
    Strmat result = strmat_empty();
    char* pointer = strtok(input.c_str, " ");
    do {
        strmat_put(&result, pointer);
        pointer = strtok(NULL, " ");
    } while (pointer);
    return result;
}

// TODO: Fix the path processor to take ~ at arbitrary position
String process_path(String input) {
    if (input.c_str[0] == '~') return home_path;
    return input;
}
