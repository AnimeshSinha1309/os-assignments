#include "ls.h"

#include <dirent.h>
#include <grp.h>
#include <math.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

static int directory_sort(const struct dirent** a, const struct dirent** b) {
    const char *name_a = (*a)->d_name, *name_b = (*b)->d_name;
    if (*name_a == '.')
        name_a++;
    if (*name_b == '.')
        name_b++;
    return strcasecmp(name_a, name_b);
}

static bool directory_filter(const struct dirent* dir) {
    const char* name = dir->d_name;
    return strlen(name);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
void func_l(const char **files_list, int files_count, int flag_a) {
    // Collect the lengths of fields in the first pass
    int field_len_uname = 0, field_len_gname = 0, field_len_size = 0, field_len_link = 0, field_len_sum = 0;
    for (int i = 0; i < files_count; i++) {
        struct stat s;
        const char* file = files_list[i];

        if (strlen(file) == 0) continue;
        if (!flag_a && *file == '.') continue;
        if (lstat(file, &s) < 0) { perror(file); return; }
        char *uname = getpwuid(getuid())->pw_name, *gname = getgrgid(getgid())->gr_name;
        field_len_uname = (int)fmax(field_len_uname, (int)strlen(uname));
        field_len_gname = (int)fmax(field_len_gname, (int)strlen(gname));
        field_len_size = (int)fmax(field_len_size, (int)log10(s.st_size) + 1);
        field_len_link = (int)fmax(field_len_link, (int)log10(s.st_nlink) + 1);
        field_len_sum += (int)s.st_blocks / 2;
    }
    // Now start actually printing the values
    printf("Total %d\n", field_len_sum);
    for (int i = 0; i < files_count; i++) {
        struct stat s;
        const char* file = files_list[i];
        if (strlen(file) == 0) continue;
        if (!flag_a && *file == '.') continue;
        if (lstat(file, &s) < 0) { perror(file); return; }

        char permissions_string[11] = "drwxrwxrwx";
        if (!S_ISDIR(s.st_mode))
            permissions_string[0] = '-';
        int permission_values[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
        for (int permission_counter = 0; permission_counter < 9; permission_counter++)
            if (!(s.st_mode & permission_values[permission_counter])) permissions_string[permission_counter + 1] = '-';

        char *user_name = getpwuid(getuid())->pw_name,
                *group_name = getgrgid(getgid())->gr_name,
                *make_time = string_cut(ctime(&s.st_mtime) + 4, 12).c_str;
        printf("%s %*ld %.*s %.*s %*ld %.*s %s\n",
                permissions_string,
                field_len_link, s.st_nlink,
                field_len_uname, user_name,
                field_len_gname, group_name,
                field_len_size, s.st_size,
                15, make_time,
                file);
    }
}
#pragma clang diagnostic pop

void ls(struct String directory, int flag_a, int flag_l) {
    const char* output[1000];
    int list_length;
    struct dirent** files_list;
    list_length = scandir(directory.c_str, &files_list, directory_filter, directory_sort);
    if (list_length == -1) {
        perror("Scan-Dir Failed.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < list_length; i++) {
        output[i] = files_list[i]->d_name;
    }
    struct stat s;
    lstat(directory.c_str, &s);
    chdir(directory.c_str);

    if (flag_l) {
        func_l(output, list_length, flag_a);
    } else {
        for (int i = 0; i < list_length; i++) {
            if (!flag_a && output[i][0] == '.')
                continue;
            printf("%s\n", output[i]);
        }
    }
}
