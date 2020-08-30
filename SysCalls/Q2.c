#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void display(const char* party, const char* type, const char* file,
             int boolean) {
    char output[100];
    sprintf(output, "%s has %s permissions on %s: %s\n", party, type, file,
            boolean ? "Yes" : "No");
    write(1, output, strlen(output));
}

const int CHUNK_SIZE = 8000;
char buffer_1[CHUNK_SIZE + 100];
char buffer_2[CHUNK_SIZE + 100];

int equivalent(const char* a, const char* b, int n) {
    // Checks if the two strings are reverse of each other
    int l = 0, r = n - 1;
    for (int l = 0, r = n - 1; l <= r; l++, r--)
        if (a[l] != b[r] || a[r] != b[l])
            return 0;
    return 1;
}

void permissions(const char* path, const char* file) {
    struct stat status;
    if (stat(path, &status) < 0)
        perror("Could not stat file"), exit(1);
    display("User", "read", file, (status.st_mode & S_IRUSR));
    display("User", "write", file, (status.st_mode & S_IWUSR));
    display("User", "execute", file, (status.st_mode & S_IXUSR));
    display("Group", "read", file, (status.st_mode & S_IRGRP));
    display("Group", "write", file, (status.st_mode & S_IWGRP));
    display("Group", "execute", file, (status.st_mode & S_IXGRP));
    display("Others", "read", file, (status.st_mode & S_IROTH));
    display("Others", "write", file, (status.st_mode & S_IWOTH));
    display("Others", "execute", file, (status.st_mode & S_IXOTH));
}

int main(int argc, char* argv[]) {
    // Get the arguments ready in a usable form
    if (argc != 4)
        perror("Arguments missing, please provide input file."), exit(0);
    // Directory exists
    struct stat status;
    if (stat(argv[3], &status) >= 0 && S_ISDIR(status.st_mode))
        write(1, "Directory is created: Yes\n", 26);
    else
        write(1, "Directory is created: No\n", 25);

    // Files are reverse
    int fd_i = open(argv[1], O_RDONLY);
    int fd_o = open(argv[2], O_RDONLY);
    long long size = lseek(fd_i, 0, SEEK_END);
    long long cursor = size, counter = 0;
    int flag = 1;
    do {
        int shift = cursor > CHUNK_SIZE ? CHUNK_SIZE : cursor;
        cursor -= shift;
        lseek(fd_i, cursor, SEEK_SET);
        int n = read(fd_i, buffer_1, shift);
        int m = read(fd_o, buffer_2, n);
        if (n != m || !equivalent(buffer_1, buffer_2, n)) {
            flag = 0;
            break;
        }
    } while (cursor > 0);

    if (flag == 1)
        write(1, "Whether file contents are reversed in newfile: Yes\n", 51);
    else
        write(1, "Whether file contents are reversed in newfile: No\n", 50);

    // Print permissions
    permissions(argv[1], "newFile");
    permissions(argv[2], "oldFile");
    permissions(argv[3], "directory");
}