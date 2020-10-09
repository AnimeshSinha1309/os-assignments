#include <unistd.h>
#include <wait.h>
#include "functions.h"

void merge_sort_2(int *arr, int l, int r) {
    if (r - l + 1 <= 5) { selection_sort(arr, l, r); return; }
    int m = (l + r) / 2;

    pid_t pid_1 = fork();
    if (pid_1 == 0) {
        // Left child
        merge_sort_2(arr, l, m);
        _exit(1);
    } else {
        pid_t pid_2 = fork();
        if (pid_2 == 0) {
            // Right child
            merge_sort_2(arr, m, r);
            _exit(1);
        } else {
            // Parent
            int status;
            waitpid(pid_1, &status, 0);
            waitpid(pid_2, &status, 0);
        }
    }

    int arr_l[m - l], arr_r[r - m];
    for (int i = l; i < m; i++) arr_l[i - l] = arr[i];
    for (int i = m; i < r; i++) arr_r[i - m] = arr[i];
    int i = 0, j = 0, k = 0;
    for (; i < m - l && j < r - m; k++)
        arr[k] = (arr_l[i] < arr_r[j]) ? arr_l[i++] : arr_r[j++];
    for (; i < m - l; i++, k++) arr[k] = arr_l[i];
    for (; j < r - m; j++, k++) arr[k] = arr_r[j];
}

void sort_values_2(int *arr, int n) {
    // getting shared memory address
    int *mem = share_memory(sizeof(int) * (n + 1));
    if (*arr == -1) {
        perror("Couldn't attach memory segment");
        return;
    }
    copy_array(arr, mem, n);
    merge_sort_2(mem, 0, n);
    copy_array(mem, arr, n);
}