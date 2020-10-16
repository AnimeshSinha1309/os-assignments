#ifndef MULTIPROCESSING_STAGE_H
#define MULTIPROCESSING_STAGE_H

#include "musician.h"
#include "functions.h"
#include <semaphore.h>

sem_t *acoustic_semaphore, *electric_semaphore, *singer_semaphore;
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

void register_spot(Musician* musician, int pos);
int book_singer(Musician* musician);

#endif //MULTIPROCESSING_STAGE_H
