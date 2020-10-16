#include "functions.h"
#include "stage.h"
#include <semaphore.h>
#include <assert.h>

void stage_init() {
    acoustic_semaphore = share_memory(sizeof(sem_t));
    electric_semaphore = share_memory(sizeof(sem_t));
    singer_semaphore = share_memory(sizeof(sem_t));
    coordinator_semaphore = share_memory(sizeof(sem_t));
    all_stages = share_memory(sizeof(Musician) * (n_stages_a + n_stages_e));
    sem_init(acoustic_semaphore, 0, n_stages_a);
    sem_init(electric_semaphore, 0, n_stages_e);
    sem_init(coordinator_semaphore, 0, n_coordinators);
    sem_init(singer_semaphore, 0, n_stages_a + n_stages_e);

    coordinator_mutex = share_memory(sizeof(sem_t));
    pthread_mutex_init(coordinator_mutex, NULL);
    stages_mutex = share_memory(sizeof(sem_t));
    pthread_mutex_init(stages_mutex, NULL);
}


void register_spot(Musician* musician, int pos) {
    assert(musician->instrument != SINGER);
    all_stages[pos].musician = musician;
}

int book_singer(Musician* musician) {
    while (TRUE) {
        int pos = randint(0, n_stages_a + n_stages_e);
        if (all_stages[pos].singer == NULL) {
            all_stages[pos].singer = musician;
            if (all_stages[pos].musician != NULL && (
                    all_stages[pos].musician->status == PERFORMING_SOLO
                    || all_stages[pos].musician->status == PERFORMING_COMBO
                    )) {
                all_stages[pos].musician->status = PERFORMING_COMBO;
                musician->status = PERFORMING_COMBO;
            }
            return pos;
        }
    }
}
