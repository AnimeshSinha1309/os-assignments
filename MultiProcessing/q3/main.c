#include <stdio.h>

#include "musician.h"
#include "stage.h"
#include "distrib.h"
#include "functions.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
int main() {
    // Take the inputs and initialize classes
    printf("Enter the number and time inputs: ");
    fflush(stdout);
    scanf("%d %d %d %d %d %d %d", &n_musician, &n_stages_a, &n_stages_e,
            &n_coordinators, &t_duration_min, &t_duration_max, &t_patience);
    musician_init();
    // Branch off and start all processes
    pthread_t *musician_threads = (pthread_t *) share_memory(sizeof(pthread_t) * n_musician);
    for (int i = 0; i < n_musician; i++)
        pthread_create(&musician_threads[i], NULL, musician_process, all_musicians + i);
    // Wrap the starter process up
    for (int i = 0; i < n_musician; i++) {
        pthread_join(musician_threads[i], NULL);
    }
    return 0;
}
#pragma clang diagnostic pop