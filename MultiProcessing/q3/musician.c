#include "functions.h"
#include "musician.h"
#include "distrib.h"
#include "stage.h"

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <zconf.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
void musician_init() {
    all_musicians = (Musician*)share_memory(n_musician * sizeof(Musician));
    for (int i = 0; i < n_musician; i++) {
        char* name; char instrument; int arrival;
        scanf("%s %u %d",
                all_musicians[i].name,
                &all_musicians[i].instrument,
                &all_musicians[i].arrival_time);
        musician_init(name, instrument, arrival);
    }
}
#pragma clang diagnostic pop

void* musician_process(void* input) {
    Musician* musician = (Musician*) input;
    if (musician->arrival_time == 0)
    title_print(CLASS_MUSICIAN, , "started from room");
    sleep(musician->arrival_time);
}