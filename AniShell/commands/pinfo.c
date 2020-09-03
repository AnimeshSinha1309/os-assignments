#include "pinfo.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pwd.h"

char* pstatus(char* pid) {
    String pid_path = string_join("/proc/", string_join(pid, "/stat").c_str);
    int stat_file = open(pid_path.c_str, O_RDONLY);
    if (stat_file < 0) return NULL;
    char* buf = (char*)calloc(MAX_LETTERS_IN_TOKEN, 1);
    if (read(stat_file, buf, 10000) == 0) {
        perror("Couldn't read /proc/pid/stat");
        return NULL;
    }
    close(stat_file);
    strtok(buf, " ");
    strtok(NULL, " ");
    return strtok(NULL, " ");
}

void pinfo(String pid) {
    bool shell_process = pid.length == 0;
    pid = pid.length == 0 ? string_itoa(getpid()) : pid;

    printf("PID: -- %s\n", pid.c_str);
    printf("Process status: -- %s\n", pstatus(pid.c_str));

    String stat_path = string_join(string_join("/proc/", pid.c_str).c_str, "/statm");
    int stat_file = open(stat_path.c_str, O_RDONLY);
    char* stat_buffer = (char*)calloc(MAX_LETTERS_IN_TOKEN, 1);
    read(stat_file, stat_buffer, MAX_LETTERS_IN_TOKEN);
    char* memory_val = strtok(stat_buffer, " ");
    printf("Memory: -- %s\n", memory_val);
    close(stat_file);

    String exe_path = string_join(string_join("/proc/", pid.c_str).c_str, "/exe");
    char* exe_buffer = (char*)calloc(MAX_LETTERS_IN_TOKEN, 1);
    readlink(exe_path.c_str, exe_buffer, MAX_LETTERS_IN_TOKEN);

    shell_process
        ? printf("Executable path -- %s/AniShell\n", sys_pwd().c_str)
        : printf("Executable path -- %s\n", exe_buffer);
    fflush(stdout);
}
