#ifndef ANISHELL_JOBLIST_H
#define ANISHELL_JOBLIST_H

#include "../globals.h"
#include "string.h"

struct JobItem {
    int pid;
    String name;
};

int job_count;
struct JobItem *job_list;

void initialize_joblist();

void jobs_push(String name, int pid);


#endif //ANISHELL_JOBLIST_H
