#include "syscom.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>

#include "../utils/joblist.h"
#include "pinfo.h"
#include "../processor/signal.h"
#include "../processor/prompt.h"


void setenvr(String var, String value) {
    String command = string_join(var.c_str, string_join("=", value.c_str).c_str);
    putenv(command.c_str);
}

void unsetenvr(String var) {
    unsetenv(var.c_str);
}

void getenvr(String var) {
    char* res = getenv(var.c_str);
    if (res == NULL) return;
    printf("%s\n", res);
}

void jobs() {
    for (int i = 0; i < job_count; i++) {
        char* status = pstatus(string_itoa(job_list[i].pid).c_str);
        if (status == NULL) {
            continue;
        } else if (status[0] == 'R') {
            status = "Running";
        } else if (status[0] == 'S' || status[0] == 'D') {
            status = "Sleeping";
        } else if (status[0] == 'Z' || status[0] == 'z') {
            status = "Zombie";
        } else if (status[0] == 'T' || status[0] == 't') {
            status = "Stopped";
        } else {
            perror("Unlisted status ID");
            exit_code = 1;
        }
        printf("[%d] %s %s [%d]\n",
               i + 1,
               status,
               job_list[i].name.c_str,
               job_list[i].pid);
    }
}

void kjob(int pid, int signal) {
    if (pid > job_count || pid < 1) {
        perror("Job ID is out of range or not has been removed");
        exit_code = 1;
        return;
    }
    kill(pid, signal);
}

void overkill() {
    for (int i = 0; i < job_count; i++) {
        kill(job_list[i].pid, SIGKILL);
    }
}

void bg(int idx) {
    idx--;
    if (idx >= job_count || idx < 0) {
        perror("Job ID is out of range or not has been removed");
        exit_code = 1;
        return;
    }
    kill(job_list[idx].pid, SIGTTIN);
    kill(job_list[idx].pid, SIGCONT);
}

void fg(int idx) {
    idx--;
    if (idx >= job_count || idx < 0) {
        perror("Job ID is out of range or not has been removed");
        exit_code = 1;
        return;
    }
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, job_list[idx].pid);
    kill(job_list[idx].pid, SIGCONT);

    int st;
    current_process = job_list[idx].pid;
    waitpid(job_list[idx].pid, &st, WUNTRACED);

    tcsetpgrp(0, getpgrp());
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
}
