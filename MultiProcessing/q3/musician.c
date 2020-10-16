#include "functions.h"
#include "musician.h"
#include "stage.h"

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <zconf.h>
#include <assert.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
void musician_init() {
    all_musicians = (Musician*)share_memory(n_musician * sizeof(Musician));
    for (int i = 0; i < n_musician; i++) {
        all_musicians[i].name = (char*) calloc(20, sizeof(char));
        char instrument_value;
        scanf("%s %c %d",
                all_musicians[i].name,
                &instrument_value,
                &all_musicians[i].arrival_time);
        all_musicians[i].status = NOT_YET_ARRIVED;
        all_musicians[i].instrument = instrument_value;
        pthread_mutex_init(&all_musicians[i].mutex, NULL);
        assert(all_musicians[i].instrument == PIANO || all_musicians[i].instrument == GUITAR
                || all_musicians[i].instrument == VIOLIN || all_musicians[i].instrument == BASS
                || all_musicians[i].instrument == SINGER);
    }
}
#pragma clang diagnostic pop

void* musician_process(void* input) {
    Musician* musician = (Musician*) input;
    sleep(musician->arrival_time);
    printf("%s " COLOR_BLUE "%8s " COLOR_RESTORE "arrived at the event\n", get_time(), musician->name);
    fflush(stdout);
    musician->status = WAITING_TO_PERFORM;
    if (musician->instrument == SINGER) {
        Queue queue; queue.musician = musician; queue.type = SINGER_STAGE;
        musician_perform(&queue);
    } else {
        pthread_t *thread_1 = NULL, *thread_2 = NULL;
        if (musician->instrument != VIOLIN) {
            Queue queue; queue.musician = musician; queue.type = ELECTRIC_STAGE;
            thread_1 = (pthread_t *) share_memory(sizeof(pthread_t));
            pthread_create(thread_1, NULL, musician_perform, &queue);
        }
        if (musician->instrument != BASS) {
            Queue queue; queue.musician = musician; queue.type = ACOUSTIC_STAGE;
            thread_2 = (pthread_t *) share_memory(sizeof(pthread_t));
            pthread_create(thread_2, NULL, musician_perform, &queue);
        }
        if (thread_1 != NULL) pthread_join(*thread_1, NULL);
        if (thread_2 != NULL) pthread_join(*thread_2, NULL);
    }
    return NULL;
}

void* musician_perform(void* input_raw) {
    Queue* input = (Queue*) input_raw;
    Musician* musician = input->musician;
    switch (input->type) {
        case ACOUSTIC_STAGE: sem_wait(acoustic_semaphore); break;
        case ELECTRIC_STAGE: sem_wait(electric_semaphore); break;
        case SINGER_STAGE: sem_wait(singer_semaphore); break;
    }
    pthread_mutex_lock(&musician->mutex); // The check-status update-status needs to be atomic
    if (musician->status == WAITING_TO_PERFORM) {
        int stage_id;
        switch (input->type) {
            case ACOUSTIC_STAGE:
                sem_getvalue(acoustic_semaphore, &stage_id);
                musician->status = PERFORMING_SOLO;
                pthread_mutex_unlock(&musician->mutex);
                printf("%s " COLOR_BLUE "%8s " COLOR_RESTORE "got to " COLOR_RED "acoustic stage %d\n"
                        COLOR_RESTORE, get_time(), musician->name, stage_id + 1);
                register_spot(musician, stage_id);
                break;
            case ELECTRIC_STAGE:
                sem_getvalue(electric_semaphore, &stage_id);
                musician->status = PERFORMING_SOLO;
                pthread_mutex_unlock(&musician->mutex);
                printf("%s " COLOR_BLUE "%8s " COLOR_RESTORE "got to " COLOR_RED "electric stage %d\n"
                       COLOR_RESTORE, get_time(), musician->name, stage_id + 1);
                register_spot(musician, stage_id + n_stages_a);
                break;
            case SINGER_STAGE:
                sem_getvalue(electric_semaphore, &stage_id);
                musician->status = PERFORMING_SOLO;
                int stage_pos = book_singer(musician);
                if (stage_pos >= n_stages_a)
                    printf("%s " COLOR_BLUE "%8s " COLOR_RESTORE "got to " COLOR_RED "electric stage %d\n"
                           COLOR_RESTORE, get_time(), musician->name, stage_id + 1 - n_stages_a);
                else
                    printf("%s " COLOR_BLUE "%8s " COLOR_RESTORE "got to " COLOR_RED "acoustic stage %d\n"
                           COLOR_RESTORE, get_time(), musician->name, stage_id + 1);
                break;
        }
        // TODO: Implement patience using timed-wait and time checks
        fflush(stdout);
        sleep(randint(t_duration_min, t_duration_max));
        // Checking if the combo extended the performance
        if (musician->status == PERFORMING_COMBO) {
            // FIXME: Check that the deregister operation atomic
            sleep(2);
        }
        switch (input->type) {
            case ACOUSTIC_STAGE: sem_post(acoustic_semaphore); break;
            case ELECTRIC_STAGE: sem_post(electric_semaphore); break;
            case SINGER_STAGE: sem_post(singer_semaphore); break;
        }

        printf("%s " COLOR_BLUE "%8s " COLOR_RESTORE "completed performance on " COLOR_RED "stage %d\n"
                COLOR_RESTORE, get_time(), musician->name, stage_id + 1);
        fflush(stdout);
        // Collecting the T-Shirt
        if (input->type != SINGER_STAGE || SINGERS_GET_TSHIRTS) {
            musician->status = WAITING_FOR_TSHIRT;
            sem_wait(coordinator_semaphore);
            musician->status = COLLECTING_TSHIRT;
            sleep(2);
            int coordinator_id; sem_getvalue(coordinator_semaphore, &coordinator_id);
            printf("%s " COLOR_BLUE "%8s " COLOR_RESTORE "was given t-shirt by " COLOR_RED "coordinator %d\n"
                   COLOR_RESTORE, get_time(), musician->name, coordinator_id + 1);
            // FIXME: Coordinator ID needs to be made atomic too
            fflush(stdout);
            sem_post(coordinator_semaphore);
        }
        musician->status = EXITED;
        return NULL;
    } else {
        switch (input->type) {
            case ACOUSTIC_STAGE: sem_post(acoustic_semaphore); break;
            case ELECTRIC_STAGE: sem_post(electric_semaphore); break;
            case SINGER_STAGE: sem_post(singer_semaphore); break;
        }
    }
    pthread_mutex_unlock(&musician->mutex);
    return NULL;
}