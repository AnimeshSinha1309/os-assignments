#include <stdio.h>
#include <sys/shm.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
int *share_memory(size_t size) {
    key_t mem_key = IPC_PRIVATE;
    // get shared memory of this much size and with this private key
    int shm_id = shmget(mem_key, size, IPC_CREAT | 0666);
    // attach the address space of shared memory to myself (callee)
    return (int *)shmat(shm_id, NULL, 0);
}
#pragma clang diagnostic pop

// Swaps 2 integers given the pointers
void swap(int* i, int* j) {
    if (i == j) return;
    *j = *i + *j;
    *i = *j - *i;
    *j = *j - *i;
}

// Sorts the array a[l..r], left-inclusive right-exclusive
void selection_sort(int *arr, int l, int r) {
    for (int i = l; i < r; i++) {
        int pos = i;
        for (int j = i + 1; j < r; j++) {
            if (arr[j] < arr[pos])
                pos = j;
        }
        swap(arr + pos, arr + i);
    }
}

void print_array(int *arr, int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void copy_array(const int *org, int *dup, int n) {
    for (int i = 0; i < n; i++)
        dup[i] = org[i];
}
