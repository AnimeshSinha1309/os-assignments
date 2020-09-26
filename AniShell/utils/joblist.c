#include "joblist.h"
#include <stdlib.h>

void initialize_joblist() {
    job_list = (struct JobItem *) calloc(sizeof(struct JobItem), MAX_JOBS);
    job_count = 0;
}

void jobs_push(String name, int pid) {
    job_list[job_count].pid = pid;
    job_list[job_count].name = name;
    job_list[job_count].fg = false;
    job_count++;
}
