#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define CHUNK_SIZE 8000
char buffer[CHUNK_SIZE + 100];
char text[100];

void reverse(char *str, int n) {
    // Reverses a string
    for (int l = 0, r = n - 1; l < r; l++, r--) {
        str[l] ^= str[r];
        str[r] ^= str[l];
        str[l] ^= str[r];
    }
}

int main(int argc, char *argv[]) {
    // Get the arguments ready in a usable form
    if (argc != 2)
        perror("Arguments missing, please provide input file."), exit(0);
    if (mkdir("Assignment", S_IRUSR | S_IWUSR | S_IXUSR) != 0)
        perror("Could not make directory, permission or path error.");
    char name[] = "Assignment/";
    strcat(name, strrchr(argv[1], (int)'/') == NULL
                     ? argv[1]
                     : strrchr(argv[1], (int)'/') + 1);

    // Initialize all the variables, open the files
    int fd_i = open(argv[1], O_RDONLY);
    int fd_o = open(name, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    long long size = lseek(fd_i, 0, SEEK_END);
    long long cursor = size, counter = 0;
    float percent = size / (100 * CHUNK_SIZE);

    do {
        // Read and Write with cursor manipulation
        int shift = cursor > CHUNK_SIZE ? CHUNK_SIZE : cursor;
        cursor -= shift;
        lseek(fd_i, cursor, SEEK_SET);
        int n = read(fd_i, buffer, shift);
        reverse(buffer, n);
        write(fd_o, buffer, n);
        // Progress display
        sprintf(text, "\rPercentage Done: %.2f", counter++ / percent);
        write(1, text, 50);
    } while (cursor > 0);
    write(1, "\rPercentage Done: 100.00\n", 26);
    exit(0);
}