#ifndef MULTIPROCESSING_FUNCTIONS_H
#define MULTIPROCESSING_FUNCTIONS_H

#include <stdio.h>
#include <sys/shm.h>

void selection_sort(int *arr, int l, int r);
void swap(int* i, int* j);
void print_array(int *arr, int n);
void copy_array(const int *org, int *dup, int n);
int *share_memory(size_t size);

#endif //MULTIPROCESSING_FUNCTIONS_H
