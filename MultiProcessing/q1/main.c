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
    long double start_time_1 = time_abs();
    sort_values_1(arr, n);
    long double end_time_1 = time_abs();
    print_array(arr, n);
    // Getting the regular merge-sort
    copy_array(org, arr, n);
    long double start_time_2 = time_abs();
    sort_values_2(arr, n);
    long double end_time_2 = time_abs();
    print_array(arr, n);
    // Getting the regular merge-sort
    copy_array(org, arr, n);
    long double start_time_3 = time_abs();
    sort_values_3(arr, n);
    long double end_time_3 = time_abs();
    print_array(arr, n);
    // Wrap up and print stats
    printf("Time for regular merge sort: %Lf\n", end_time_1 - start_time_1);
    printf("Time for processes merge sort: %Lf\n", end_time_2 - start_time_2);
    printf("Time for threaded merge sort: %Lf\n", end_time_3 - start_time_3);
    return 0;
}
#pragma clang diagnostic pop