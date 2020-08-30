#include "pwd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../globals.h"
#include "../utils/string.h"
#include "../processor/prompt.h"

struct String sys_pwd() {
    struct String current_path;
    current_path.c_str = (char*)calloc(MAX_PATH_LENGTH, 1);
    getcwd(current_path.c_str, MAX_PATH_LENGTH);
    current_path.length = (int)strlen(current_path.c_str);

    int match = string_match(current_path, home_path);
    if (match >= home_path.length) {
        sprintf(current_path.c_str , "~%s", current_path.c_str + match);
    }
    return current_path;
}

void pwd() {
    printf("%s", sys_pwd().c_str);
}