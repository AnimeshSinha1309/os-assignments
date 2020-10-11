#include <stdlib.h>
#include <pthread.h>
#include "functions.h"

struct List { int l, r, *arr; };
struct List* get_list(int *arr, int l, int r) { struct List x; x.l = l; x.r = r; x.arr = arr; return &x; }

void merge_sort_3(void *input) {
    struct List *arr = (struct List *)input;
    if (arr->r - arr->l + 1 <= 5) { selection_sort(arr->arr, arr->l, arr->r); return; }
    int m = (arr->l + arr->r) / 2;
    pthread_t tid_1;
    pthread_create(&tid_1, NULL, merge_sort_3, get_list(arr->arr, arr->l, m));
    pthread_t tid_2;
    pthread_create(&tid_2, NULL, merge_sort_3, get_list(arr->arr, m, arr->r));
    pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);
    int arr_l[m - arr->l], arr_r[arr->r - m];
    for (int i = arr->l; i < m; i++) arr_l[i] = arr_l[i - arr->l];
    for (int i = m; i < arr->r; i++) arr_r[i] = arr_r[i - m];
    int i = 0, j = 0, k = 0;
    for (; i < m - arr->l && j < arr->r - m; k++)
        arr->arr[k] = (arr_l[i] < arr_r[j]) ? arr_l[i++] : arr_r[j++];
    for (; i < m - arr->l; i++, k++) arr->arr[k] = arr_l[i];
    for (; j < arr->r - m; j++, k++) arr->arr[k] = arr_r[j];
}

void sort_values_3(int *arr, int n) {
    merge_sort_3(get_list(arr, 0, n));
}