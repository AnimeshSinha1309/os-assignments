// Per-CPU state
struct cpu {
  uchar apicid;                // Local APIC ID
  struct context *scheduler;   // swtch() here to enter scheduler
  struct taskstate ts;         // Used by x86 to find stack for interrupt
  struct segdesc gdt[NSEGS];   // x86 global descriptor table
  volatile uint started;       // Has the CPU started?
  int ncli;                    // Depth of pushcli nesting.
  int intena;                  // Were interrupts enabled before pushcli?
  struct proc *proc;           // The process running on this cpu or null
};

extern struct cpu cpus[NCPU];
extern int ncpu;

//PAGEBREAK: 17
// Saved registers for kernel context switches.
// Don't need to save all the segment registers (%cs, etc),
// because they are constant across kernel contexts.
// Don't need to save %eax, %ecx, %edx, because the
// x86 convention is that the caller has saved them.
// Contexts are stored at the bottom of the stack they
// describe; the stack pointer is the address of the context.
// The layout of the context matches the layout of the stack in swtch.S
// at the "Switch stacks" comment. Switch doesn't save eip explicitly,
// but it is on the stack and allocproc() manipulates it.
struct context {
  uint edi;
  uint esi;
  uint ebx;
  uint ebp;
  uint eip;
};

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// Per-process state
struct proc {
  uint sz;                     // Size of process memory (bytes)
  pde_t* pgdir;                // Page table
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state
  int pid;                     // Process ID
  struct proc *parent;         // Parent process
  struct trapframe *tf;        // Trap frame for current syscall
  struct context *context;     // swtch() here to run process
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)

  int start_time;
  int end_time;
  int running_time;

  int priority;
  int n_run;
  int cur_q;
  int q_ticks[5];
  int last_enqueue_ticks;
  int num_run;
};

// Process memory is laid out contiguously, low addresses first:
//   text
//   original data and bss
//   fixed-size stack
//   expandable heap


// Select the Scheduler you want
#undef SCHEDULER_FCFS
#undef SCHEDULER_RR
#define SCHEDULER_PBS
#undef SCHEDULER_MLFQ

// Priority Based Scheduling

#ifdef SCHEDULER_PBS

#define DEFAULT_PRIORITY 60
int should_preempt(int priority);

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
