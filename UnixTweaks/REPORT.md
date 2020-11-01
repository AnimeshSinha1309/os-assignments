# xv6: Syscalls, Scheduling and Processes

## Implementing Features

### Changes made to implement syscalls

The files that need to be added/edited are:
* syscall.h: Adding the syscall in the list, assigning it a syscall number
* syscall.c: Adding the syscall in the list, and a function definition for sys_fn
* sysproc.h: Implemented the getting args logic and calling fn
* defs.h: Adding the syscall in the list
* Makefile: Adding to compilation
* usys.S: Adding the syscall in the list
* **proc.c**: Actual implementation of the syscall

### Changes made to implement user processes

The files that need to be added/edited are:
* The process.c file: needs to contain the logic of the program in the main function.
It will receive the args and then pass to the relevant syscalls to do it's job.
* Makefile: needs to be added in 2 locations, identical to the wc user process, so
that our user process is a part of the compilation.

### Changes made to implement schedulers

The files with major changes are:
* proc.c: To implement all the changes to the processes selection algorithm in the 
scheduler method together with a function to check for preemption.
* proc.h: Changes to the process structure to store the added facets of the process,
keep global constants (preprocessor directives).
* trap.c: To implement relinquishing the CPU if a process has exceeded it's runtime
or for preempting the process.

## Exploiting MLFQ

A user process can hog the CPU by voluntarily leaving the queue just before it runs 
out of time and then getting pushed back in the same queue, thereby spending all
it's time in the highest priority queue and blocking other processes which are in
lower priority queues.

## Relative Performance

On the task of 4 competing processes with rare I/O statements that all run very long,
following are the performance metrics:
* RR: Running time - 3206; Waiting Time - 3201
* FCFS: Running time - 3178; Waiting Time - 0
* MLFQ: Running time - 3310; Waiting Time - 1757
* PBS: Running time - 3490; Waiting Time - 3545

We see that here FCFS performs the best (over the entire process, the one which has
all the forks), since it wastes no time switching. Of course, it's worse because it
will keep the other forks waiting thereby hanging the I/O of the computer.
PBS is the slowest by performing the maximum number of switches and preemption.
RR is a little faster, MLFQ is the best, not wasting a lot of time in switching,
round-robins slowly in the slow processes while staying free for the high priority
ones. Therefore MLFQ seems to be the best, and FCFS is the fastest.

## Note on the plots

I have shown a short term and long term plot, the plotting data is also attached.
The long term plot ages very slowly, and only oscillates between queue 3 and 4,
because the aging time is high. If we have a lower AGING_THRESHOLD, then osciallation
will include queue 2.
