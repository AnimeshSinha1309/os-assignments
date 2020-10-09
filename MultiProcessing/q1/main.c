#include <stdio.h>

#include "functions.h"
#include "normal.h"
#include "processes.h"
#include "threads.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
int main() {
    // Getting the array
    int n;
    scanf("%d", &n);
    int arr[n], org[n];
    for (int i = 0; i < n; i++) {
        scanf("%d", org + i);
    }
    // Getting the regular merge-sort
    copy_array(org, arr, n);
    sort_values_1(arr, n);
    print_array(arr, n);
    // Getting the regular merge-sort
    copy_array(org, arr, n);
    sort_values_2(arr, n);
    print_array(arr, n);
    // Getting the regular merge-sort
    copy_array(org, arr, n);
    sort_values_3(arr, n);
    print_array(arr, n);
    return 0;
}
#pragma clang diagnostic pop