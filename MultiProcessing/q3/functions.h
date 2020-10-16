#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/shm.h>

#ifndef MULTIPROCESSING_FUNCTIONS_H
#define MULTIPROCESSING_FUNCTIONS_H

typedef int bool;
#define TRUE 1
#define FALSE 0

#define SINGERS_GET_TSHIRTS TRUE

#define COLOR_RESTORE "\x1B[0m"
#define COLOR_RED "\x1B[31m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_BLUE "\x1b[34m"

int n_musician, n_stages_e, n_stages_a, n_coordinators;
int t_duration_min, t_duration_max, t_patience;

int randint(int l, int r);
void delay(int time);
char* get_time();
void *share_memory(size_t size);
int min_3(int a, int b, int c);

#endif //MULTIPROCESSING_FUNCTIONS_H
