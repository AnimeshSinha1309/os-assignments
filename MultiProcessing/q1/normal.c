#include "functions.h"

void merge_sort_1(int *arr, int l, int r) {
    if (r - l + 1 <= 5) { selection_sort(arr, l, r); return; }
    int m = (l + r) / 2;
    merge_sort_1(arr, l, m);
    merge_sort_1(arr, m, r);
    int arr_l[m - l], arr_r[r - m];
    for (int i = l; i < m; i++) arr_l[i - l] = arr[i];
    for (int i = m; i < r; i++) arr_r[i - m] = arr[i];
    int i = 0, j = 0, k = l;
    for (; i < m - l && j < r - m; k++)
        arr[k] = (arr_l[i] < arr_r[j]) ? arr_l[i++] : arr_r[j++];
    for (; i < m - l; i++, k++) arr[k] = arr_l[i];
    for (; j < r - m; j++, k++) arr[k] = arr_r[j];
}

void sort_values_1(int *arr, int n) {
    merge_sort_1(arr, 0, n);
}