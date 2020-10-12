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

#define COLOR_RESTORE "\x1B[0m"
#define COLOR_RED "\x1B[31m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_BLUE "\x1b[34m"

#define CLASS_COMPANY 0
#define CLASS_ZONE 1
#define CLASS_STUDENT 2

#define L_ARRIVAL_TIME 1
#define R_ARRIVAL_TIME 5 + 1
#define L_PRODUCTION_TIME 2
#define R_PRODUCTION_TIME 5 + 1
#define L_BATCHES_PRODUCED 1
#define R_BATCHES_PRODUCED 5 + 1
#define L_DOSES_IN_BATCH 1
#define R_DOSES_IN_BATCH 1 + 1

int randint(int l, int r);
void delay(int time);
char* get_time();
void title_print(int type, int id, char* str);
void *share_memory(size_t size);
int min_3(int a, int b, int c);

#endif //MULTIPROCESSING_FUNCTIONS_H
