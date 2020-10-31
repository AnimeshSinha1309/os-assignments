// Select the Scheduler you want
#define SCHEDULER_FCFS
#undef SCHEDULER_RR
#undef SCHEDULER_PBS
#undef SCHEDULER_MLFQ

// Priority Based Scheduling

#ifdef SCHEDULER_PBS

#define DEFAULT_PRIORITY 60

#endif

// Multi Level Feedback Queue Scheduling

#ifdef SCHEDULER_MLFQ

#define MAX_PROC_COUNT (int)1e4
struct proc *mlqf_queue[PQ_COUNT][MAX_PROC_COUNT];

struct proc *mlfq_peek_front(int qIdx);
struct proc *mlfq_pop_front(int qIdx);
void mlfq_push_back(int qIdx, struct proc *p);
void mlfq_delete_idx(int qIdx, int idx);
void mlfq_decrease_priority(struct proc *queueIdx, int retain);
void mlfq_increase_priority(struct proc *queueIdx);
int mlfq_process_index(struct proc *currp);
int mlfq_size(int qIdx);
void mlfq_update();

#endif

// First Come First Serve Scheduling

#ifdef SCHEDULER_FCFS

#endif

// Round Robin Scheduling

#ifdef SCHEDULER_RR
#endif
