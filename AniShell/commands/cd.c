#include "cd.h"
#include "../processor/prompt.h"
#include <unistd.h>

// TODO: Fix the path processor to take ~ at arbitrary position
void cd(struct String path) {
    if (path.length == 1 && path.c_str[0] == '~') {
        chdir(home_path.c_str);
    } else if (path.length >= 2 && path.c_str[0] == '~' && path.c_str[1] == '/') {
        chdir(home_path.c_str);
        chdir(path.c_str + 1);
    } else {
        chdir(path.c_str);
    }
}