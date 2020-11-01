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
