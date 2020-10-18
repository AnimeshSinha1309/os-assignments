#include "functions.h"
#include "stage.h"
#include <semaphore.h>

void stage_init() {
    sem_a = share_memory(sizeof(sem_t));
    sem_e = share_memory(sizeof(sem_t));
    sem_s = share_memory(sizeof(sem_t));
    coordinator_semaphore = share_memory(sizeof(sem_t));
    all_stages = share_memory(sizeof(Musician) * (n_stages_a + n_stages_e));
    for (int i = 0; i < n_stages_a + n_stages_e; i++) all_stages[i].musician = NULL, all_stages[i].singer = NULL;
    sem_init(sem_a, 0, n_stages_a);
    sem_init(sem_e, 0, n_stages_e);
    sem_init(coordinator_semaphore, 0, n_coordinators);
    sem_init(sem_s, 0, n_stages_a + n_stages_e);

    coordinator_mutex = share_memory(sizeof(sem_t));
    pthread_mutex_init(coordinator_mutex, NULL);
    stages_mutex = share_memory(sizeof(sem_t));
    pthread_mutex_init(stages_mutex, NULL);
    waiting_mutex = share_memory(sizeof(sem_t));
    pthread_mutex_init(waiting_mutex, NULL);
}


int book_singer(Musician* musician) {
    musician->status = PERFORMING_SOLO;
    pthread_mutex_lock(stages_mutex);
    for (int i = 0; i < n_stages_a + n_stages_e; i++) {
        if (all_stages[i].singer == NULL) {
            all_stages[i].singer = musician;
            // Check if you got a combo
            pthread_mutex_lock(waiting_mutex);
            if (all_stages[i].musician != NULL) {
                all_stages[i].musician->status = PERFORMING_COMBO;
                all_stages[i].singer->status = PERFORMING_COMBO;
            }
            pthread_mutex_unlock(waiting_mutex);
            // Print the stage it got
            musician->stage_idx = i;
            sem_wait((i < n_stages_a) ? sem_a : sem_e); // block musician from joining
            if (i >= n_stages_a)
                printf("%s " COLOR_GREEN "%8s " COLOR_RESTORE "got to " COLOR_RED "electric stage %d\n"
                       COLOR_RESTORE, get_time(), musician->name, i + 1 - n_stages_a);
            else
                printf("%s " COLOR_GREEN "%8s " COLOR_RESTORE "got to " COLOR_RED "acoustic stage %d\n"
                       COLOR_RESTORE, get_time(), musician->name, i + 1);
            fflush(stdout);
            pthread_mutex_unlock(stages_mutex);
            return i;
        }
    }
    pthread_mutex_unlock(stages_mutex);
#ifdef DEBUGGING_MODE
    printf(COLOR_RED "ERROR: NO STAGES WERE FOUND for singer %s\n" COLOR_RESTORE, musician->name);
#endif
    return -1;
}

int book_musician(Musician* musician, StageType t) {
    musician->status = PERFORMING_SOLO;
    pthread_mutex_lock(stages_mutex);
    for (int i = (t == ACOUSTIC_STAGE ? 0 : n_stages_a);
         i < n_stages_a + (t == ELECTRIC_STAGE ? n_stages_e : 0); i++) {
        if (all_stages[i].musician == NULL && all_stages[i].singer == NULL) {
            all_stages[i].musician = musician;
            musician->stage_idx = i;
            if (i >= n_stages_a)
                printf("%s " COLOR_GREEN "%8s " COLOR_RESTORE "got to " COLOR_RED "electric stage %d\n"
                       COLOR_RESTORE, get_time(), musician->name, i + 1 - n_stages_a);
            else
                printf("%s " COLOR_GREEN "%8s " COLOR_RESTORE "got to " COLOR_RED "acoustic stage %d\n"
                       COLOR_RESTORE, get_time(), musician->name, i + 1);
            fflush(stdout);
            pthread_mutex_unlock(stages_mutex);
            return i;
        }
    }
    pthread_mutex_unlock(stages_mutex);
#ifdef DEBUGGING_MODE
    printf(COLOR_RED "%s ERROR: NO STAGES WERE FOUND for musician %s\n" COLOR_RESTORE, get_time(), musician->name);
#endif
    return -1;
}
