#ifndef MULTIPROCESSING_MUSICIAN_H
#define MULTIPROCESSING_MUSICIAN_H

#include <stdio.h>
#include <pthread.h>

enum Status {
    NOT_YET_ARRIVED,
    WAITING_TO_PERFORM,
    PERFORMING_SOLO,
    PERFORMING_COMBO,
    WAITING_FOR_TSHIRT,
    COLLECTING_TSHIRT,
    EXITED,
};

enum Instrument {
    PIANO = 'p',
    GUITAR = 'g',
    VIOLIN = 'v',
    BASS = 'b',
    SINGER = 's',
};

typedef struct Musician {
    enum Status status;
    enum Instrument instrument;
    int arrival_time;
    char* name;
    int stage_idx;
    pthread_mutex_t mutex;
} Musician;

Musician* all_musicians;

void musician_init();
void* musician_process(void* input);
void* musician_perform(void* input_raw);

#endif //MULTIPROCESSING_MUSICIAN_H
