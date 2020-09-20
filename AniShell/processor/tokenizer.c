#include "tokenizer.h"

#include <string.h>
#include "prompt.h"

Strmat tokenize_args(String input) {
    Strmat result = strmat_empty();
    char *pointer = strtok(input.c_str, " ");
    while (pointer != NULL) {
        strmat_put(&result, pointer);
        pointer = strtok(NULL, " ");
    }
    return result;
}

Strmat tokenize_str(String input, char *delimeter) {
    Strmat result = strmat_empty();
    char *pointer = strtok(input.c_str, delimeter);
    while (pointer != NULL) {
        strmat_put(&result, pointer);
        pointer = strtok(NULL, delimeter);
    }
    return result;
}

// TODO: Fix the path processor to take ~ at arbitrary position
String process_path(String path) {
    if (path.length == 1 && path.c_str[0] == '~') {
        return home_path;
    } else if (path.length >= 2 && path.c_str[0] == '~' && path.c_str[1] == '/') {
        return string_join(home_path.c_str, path.c_str + 1);
    } else {
        return path;
    }
}
