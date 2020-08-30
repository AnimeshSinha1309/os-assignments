#include <fcntl.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "string.h"
#include "../globals.h"

struct String sys_username() {
    struct passwd* p = getpwuid(getuid());
    return string_make(p->pw_name);
}

struct String sys_machinename() {
    int fd = open("/proc/sys/kernel/hostname", O_RDONLY);
    struct String name;
    name.c_str = (char*)calloc(MAX_PATH_LENGTH, 1);
    getcwd(name.c_str, MAX_PATH_LENGTH);
    name.length = (int)strlen(name.c_str);
    if (fd < 0) {
        name.length = 11;
        name.c_str = "UNK-machine";
    } else {
        name.length = read(fd, name.c_str, 100);
        name.c_str[name.length - 1] = 0;
    }
    close(fd);
    return name;
}
