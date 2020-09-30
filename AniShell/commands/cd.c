#include "cd.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "../globals.h"
#include "../processor/prompt.h"

void cd(struct String path) {
    // Storing the previous path for cd -
    if (path.length == 1 && path.c_str[0] == '-') {
        chdir(prev_path.c_str);
    }
    prev_path.c_str = (char*)calloc(MAX_PATH_LENGTH, 1);
    getcwd(prev_path.c_str, MAX_PATH_LENGTH);
    prev_path.length = (int)strlen(prev_path.c_str);
    // CD flags and path changes
    if (path.length == 1 && path.c_str[0] == '~') {
        chdir(home_path.c_str);
    } else if (path.length >= 2 && path.c_str[0] == '~' && path.c_str[1] == '/') {
        chdir(home_path.c_str);
        chdir(path.c_str + 1);
    } else if (path.c_str[0] != '-') {
        chdir(path.c_str);
    }
}