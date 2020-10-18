#ifndef MULTIPROCESSING_STAGE_H
#define MULTIPROCESSING_STAGE_H

#include "musician.h"
#include "functions.h"
#include <semaphore.h>

sem_t *sem_a, *sem_e, *sem_s;
sem_t *coordinator_semaphore;

typedef enum StageType {ACOUSTIC_STAGE, ELECTRIC_STAGE, SINGER_STAGE} StageType;

typedef struct Queue {
    Musician* musician;
    StageType type;
} Queue;

void stage_init();

typedef struct Stage {
    Musician* musician;
    Musician* singer;
} Stage;
Stage* all_stages;

int book_singer(Musician* musician);
int book_musician(Musician* musician, StageType type);

pthread_mutex_t *coordinator_mutex, *stages_mutex, *waiting_mutex;

#endif //MULTIPROCESSING_STAGE_H
