#include "functions.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
int randint(int l, int r) {
    return rand() % (r - l) + l;
}
#pragma clang diagnostic pop

void delay(int time) {
    const clock_t start_time = clock();
    while (clock() - start_time < time * CLOCKS_PER_SEC);
}

char* get_time() {
    time_t time_int;
    time(&time_int);
    char *time_str = ctime(&time_int);
    time_str[19] = '\0';
    return time_str + 11;
}

void title_print(int type, char* name, char* str) {
    if (type == CLASS_MUSICIAN) {
        printf("%s " COLOR_BLUE  "MUSICIAN %6s\t" COLOR_RESTORE "%s\n", get_time(), name + 1, str);
    } else if (type == CLASS_STAGE) {
        printf("%s " COLOR_RED   "   STAGE %6s\t" COLOR_RESTORE "%s\n", get_time(), name + 1, str);
    } else if (type == CLASS_DISTRIB) {
        printf("%s " COLOR_GREEN " DISTRIB %6s\t" COLOR_RESTORE "%s\n", get_time(), name + 1, str);
    }
    fflush(stdout);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
void *share_memory(size_t size) {
    key_t mem_key = IPC_PRIVATE;
    int shm_id = shmget(mem_key, size, IPC_CREAT | 0666);
    return (void *)shmat(shm_id, NULL, 0);
}
#pragma clang diagnostic pop

int min_3(int a, int b, int c) {
    if (a <= b && a <= c) return a;
    else if (b <= c) return b;
    else return c;
}
