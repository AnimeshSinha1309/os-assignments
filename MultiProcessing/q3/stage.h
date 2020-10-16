#ifndef MULTIPROCESSING_STAGE_H
#define MULTIPROCESSING_STAGE_H

#include "musician.h"
#include "functions.h"
#include <semaphore.h>

sem_t acoustic_semaphore, electric_semaphore, singer_semaphore;
sem_t coordinator_semaphore;

typedef enum StageType {ACOUSTIC_STAGE, ELECTRIC_STAGE, SINGER_STAGE} StageType;

typedef struct Queue {
    Musician* musician;
    StageType type;
} Queue;

void stage_init();

#endif //MULTIPROCESSING_STAGE_H
