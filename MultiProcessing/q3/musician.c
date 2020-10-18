#include "functions.h"
#include "musician.h"
#include "stage.h"

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <zconf.h>
#include <assert.h>
#include <errno.h>

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
    // Put yourself in the queue
    Queue* input = (Queue*) input_raw;
    Musician* m = input->musician;
    struct timespec* patience = share_memory(sizeof(struct timespec));
    timespec_get(patience, TIMER_ABSTIME);
    patience->tv_sec += t_patience;
    int return_code_sem = 0;
    switch (input->type) {
        case ACOUSTIC_STAGE: return_code_sem = sem_timedwait(sem_a, patience); break;
        case ELECTRIC_STAGE: return_code_sem = sem_timedwait(sem_e, patience); break;
        case SINGER_STAGE: return_code_sem = sem_timedwait(sem_s, patience); break;
    }
    // Implements patience and quitting if it exceeds time
    pthread_mutex_lock(&m->mutex);
    if (return_code_sem != 0) {
        if (m->status == WAITING_TO_PERFORM) {
            m->status = EXITED;
            pthread_mutex_unlock(&m->mutex);
            printf("%s " COLOR_RED "%8s " COLOR_RESTORE "got impatient " COLOR_RED "and left\n"
                   COLOR_RESTORE, get_time(), m->name);
            fflush(stdout);
        }
        return NULL;
    } else {
        pthread_mutex_unlock(&m->mutex);
    }
    // Check if they used another stage, if so then unlock and leave
    pthread_mutex_lock(&m->mutex);
    if (m->status != WAITING_TO_PERFORM) {
        switch (input->type) {
            case ACOUSTIC_STAGE: sem_post(sem_a); break;
            case ELECTRIC_STAGE: sem_post(sem_e); break;
            case SINGER_STAGE: sem_post(sem_s); break;
        }
        pthread_mutex_unlock(&m->mutex);
        return NULL;
    } else {
        // Let's find us a stage
        switch (input->type) {
            case ACOUSTIC_STAGE: book_musician(m, ACOUSTIC_STAGE); break;
            case ELECTRIC_STAGE: book_musician(m, ELECTRIC_STAGE); break;
            case SINGER_STAGE: book_singer(m); break;
        }
        pthread_mutex_unlock(&m->mutex);
    }
    if (input->type == ACOUSTIC_STAGE) assert(m->stage_idx < n_stages_a);
    if (input->type == ELECTRIC_STAGE) assert(m->stage_idx >= n_stages_a);
    // Do the performance
    sleep(randint(t_duration_min, t_duration_max));
    // And leave that stage
    pthread_mutex_lock(stages_mutex);
    if (input->type == SINGER_STAGE) {
        all_stages[m->stage_idx].singer = NULL;
        sem_post(sem_s);
    } else {
        all_stages[m->stage_idx].musician = NULL;
    }
    printf("%s " COLOR_GREEN "%8s " COLOR_RESTORE "completed performance on " COLOR_GREEN "stage %d\n"
           COLOR_RESTORE, get_time(), m->name, m->stage_idx + 1);
    sem_post((m->stage_idx < n_stages_a) ? sem_a : sem_e);
    // Checking if the combo extended the performance
    if (m->status == PERFORMING_COMBO) sleep(2);
    pthread_mutex_unlock(stages_mutex);
    // Collecting the T-Shirt
    if (input->type != SINGER_STAGE || SINGERS_GET_TSHIRTS) {
        m->status = WAITING_FOR_TSHIRT;
        sem_wait(coordinator_semaphore);
        m->status = COLLECTING_TSHIRT;
        sleep(2);
        pthread_mutex_lock(coordinator_mutex);
        int coordinator_id; sem_getvalue(coordinator_semaphore, &coordinator_id);
        printf("%s " COLOR_BLUE "%8s " COLOR_RESTORE "was given t-shirt by " COLOR_BLUE "coordinator\n"
               COLOR_RESTORE, get_time(), m->name);
        fflush(stdout);
        sem_post(coordinator_semaphore);
        pthread_mutex_unlock(coordinator_mutex);
    }
    m->status = EXITED;
    return NULL;
}