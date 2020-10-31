#ifdef MLFQ

/**
 * Updates the stats in the Multi-layer queue
 * Implments aging of processes
 */
void mlfq_update() {
    if (cpuid() != 0)
        panic("Must be called from cpu0 only");
    ticks++;
    acquire(&ptable.lock);
    for (struct proc *p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
        if (!p || p->killed || p->pid == 0)
            continue;
        if (p->state == RUNNABLE || p->state == RUNNING) {
            int qIdx = getQIdx(p);
            int tcks = getTicks(p) + 1;
            if (p->state == RUNNING)
                p->stat.actualTicks[qIdx]++;

            if (p->state == RUNNABLE && (ticks - p->last_enqueue_time) >= WAIT_LIMIT[qIdx]) {
                incPrio(p);
            }
        }
    }
    release(&ptable.lock);
}

struct proc *peek_front(int qIdx) {
    if (!prioQSize[qIdx]) {
        cprintf("queue %d\n", qIdx);
        panic("Getting front of empty queue");
    }
    struct proc *p = prioQ[qIdx][prioQStart[qIdx]];
    return p;
}

struct proc *mlfq_pop_front(int qIdx) {
    if (!prioQSize[qIdx]) {
        cprintf("queue %d\n", qIdx);
        panic("Empty stack, cannot pop");
    }
    // DO NOT CHANGE allotted q since they help in memory
    struct proc *p = getFront(qIdx);
    prioQStart[qIdx]++;
    if (prioQStart[qIdx] == MAX_PROC_COUNT)
        prioQStart[qIdx] = 0;
    prioQSize[qIdx]--;
    return p;
}

// index used to insert new elements into the queue
int mlfq_size(int qIdx) {
    return (prioQStart[qIdx] + prioQSize[qIdx]) % MAX_PROC_COUNT;
}

void mlfq_push_back(int qIdx, struct proc *p) {
    if (!p) {
        panic("Cannot push back empty proc");
    }

    for (int i = prioQStart[qIdx]; i != backIndex(qIdx);
         i++, i %= MAX_PROC_COUNT) {
        if (prioQ[qIdx][i] && prioQ[qIdx][i]->pid == p->pid) {
            return;
        }
    }

    p->stat.allotedQ[0] = qIdx;
    p->stat.allotedQ[1] = backIndex(qIdx);
    prioQ[qIdx][p->stat.allotedQ[1]] = p;
    ++prioQSize[qIdx];
}

void mlfq_delete_idx(int qIdx, int idx) {
    if (!prioQ[qIdx][idx]) {
        cprintf("queue %d idx %d\n", qIdx, idx);
        panic("Already deleted index");
    }

    prioQ[qIdx][idx] = 0;
    prioQSize[qIdx]--;
    int bi = backIndex(qIdx);
    for (int i = idx; i != bi; i++, i %= MAX_PROC_COUNT) {
        prioQ[qIdx][i] = prioQ[qIdx][(i + 1) % MAX_PROC_COUNT];
        prioQ[qIdx][i]->stat.allotedQ[1] = i;
    }
}

void mlfq_increase_priority(struct proc *currp) {
    int queueIdx = getQIdx(currp), qPos = getQPos(currp);
    if (queueIdx < 0 || qPos < 0) {
        cprintf("%d %d\n", queueIdx, qPos);
        panic("Invalid qi");
    }
    deleteIdx(queueIdx, qPos);

    if (!currp)
        panic("b");
    int dest = queueIdx;
    if (queueIdx == HIGHEST_PRIO_Q) {
        pushBack(queueIdx, currp);
    } else {
        dest--;
        pushBack(queueIdx - 1, currp);
    }
}

void mlfq_decrease_priority(struct proc *currp, int retain) {
    int queueIdx = getQIdx(currp);
    if (queueIdx < 0)
        panic("Invalid q");
    if (!currp)
        panic("a");

    currp->latestQTime = ticks;
    int dest = queueIdx;

    if (queueIdx == PQ_COUNT - 1 || retain) {
        pushBack(queueIdx, currp);
    } else {
        pushBack(queueIdx + 1, currp);
        dest++;
    }
}
#endif