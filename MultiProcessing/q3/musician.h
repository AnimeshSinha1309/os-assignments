#ifndef MULTIPROCESSING_MUSICIAN_H
#define MULTIPROCESSING_MUSICIAN_H

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
} Musician;

Musician* all_musicians;

void musician_init();
void* musician_process(void* input);

#endif //MULTIPROCESSING_MUSICIAN_H
