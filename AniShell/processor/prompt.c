#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../globals.h"
#include "../commands/pwd.h"
#include "../utils/string.h"
#include "../utils/sysinfo.h"

struct String home_path;

void prompt_init() {
    home_path.c_str = (char*)calloc(MAX_PATH_LENGTH, 1);
    getcwd(home_path.c_str, MAX_PATH_LENGTH);
    home_path.length = (int)strlen(home_path.c_str);
}

void prompt_print() {
    printf("<%s@%s:%s> ", sys_username().c_str, sys_machinename().c_str, sys_pwd().c_str);
}