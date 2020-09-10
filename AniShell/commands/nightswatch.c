#include "nightswatch.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "../utils/io.h"
#include "../utils/string.h"

void nightswatch(bool cpu, int interval) {
    clock_t before = clock();
    while (get_key() != 'q') {
        clock_t time = (clock() - before) / CLOCKS_PER_SEC;
        if (time >= interval) {
            before = clock();
            cpu ? watch_cpu() : watch_processes();
        }
    }
}

void watch_cpu() {
    FILE* f_ptr = fopen("/proc/interrupts", "r");
    if (f_ptr == NULL) {
        perror("Could not read from Proc file");
        return;
    }
    char* buffer = (char*)calloc(MAX_TOKENS_IN_COMMAND, 1);
    for (int i = 0; i < 3; i++) {
        fgets(buffer, MAX_TOKENS_IN_COMMAND, f_ptr);
        if (i == 0)
            printf("%s\n", buffer);
    }
    printf("%s\n", buffer + 5);
    fflush(stdout);
    fclose(f_ptr);
}

void watch_processes() {
    printf("%s\n", string_itoa(getpid()).c_str);
    fflush(stdout);
}
