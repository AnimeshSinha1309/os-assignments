#include "syscom.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>

#include "../utils/joblist.h"
#include "pinfo.h"

void setenvr(String var, String value) {
    String equal_sign = string_make("=");
    putenv(string_join(var.c_str, string_join(equal_sign.c_str, value.c_str).c_str).c_str);
}

void unsetenvr(String var) {
    unsetenv(var.c_str);
}

void jobs() {
    for (int i = 0; i < job_count; i++) {
        printf("[%d] %s %s [%d]\n",
               i + 1,
               pstatus(string_itoa(job_list[i].pid).c_str),
               job_list[i].name.c_str,
               job_list[i].pid);
    }
}

void kjob(int pid, int signal) {
    if (pid > job_count || pid < 1) {
        perror("Job ID is out of range or not has been removed");
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
    if (idx > job_count || idx <= 0) {
        perror("Job ID is out of range or not has been removed");
        return;
    }
    kill(job_list[idx].pid, SIGTTIN);
    kill(job_list[idx].pid, SIGCONT);
}

void fg(int idx) {
    if (idx > job_count || idx <= 0) {
        perror("Job ID is out of range or not has been removed");
        return;
    }
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, job_list[idx].pid);
    kill(job_list[idx].pid, SIGCONT);

    int st;
    waitpid(job_list[idx].pid, &st, WUNTRACED);

    tcsetpgrp(0, getpgrp());
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    job_list[idx].fg = true;

}
