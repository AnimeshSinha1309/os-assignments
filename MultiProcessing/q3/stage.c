#include "functions.h"
#include "musician.h"
#include "stage.h"

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

void stage_init() {

    sem_init(&acoustic_semaphore, 0, n_stages_a);
    sem_init(&electric_semaphore, 0, n_stages_e);
    sem_init(&singer_semaphore, 0, n_stages_a);
    sem_init(&coordinator_semaphore, 0, n_coordinators);
}
