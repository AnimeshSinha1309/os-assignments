#include "string.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../globals.h"

// Constructors List

struct String string_read() {
    struct String reading;
    reading.c_str = (char*)calloc(MAX_LETTERS_IN_TOKEN, 1);
    return reading;
}

struct String string_copy(struct String from) {
    struct String to;
    to.c_str = (char*)calloc(MAX_LETTERS_IN_TOKEN, 1);
    to.length = from.length;
    strncpy(to.c_str, from.c_str, from.length);
    return to;
}

struct String string_make(const char* from) {
    struct String to;
    to.c_str = (char*)calloc(MAX_LETTERS_IN_TOKEN, 1);
    to.length = (int)strlen(from);
    strcpy(to.c_str, from);
    return to;
}

struct String string_cut(const char* from, int n) {
    struct String to;
    to.c_str = (char*)calloc(MAX_LETTERS_IN_TOKEN, 1);
    to.length = n;
    strncpy(to.c_str, from, n);
    return to;
}

struct String string_empty() {
    struct String to;
    to.c_str = "";
    to.length = 0;
    return to;
}

// Tokenization type operations using single delimiter

void string_pop_back(struct String* str, char delimiter) {
    char* pos = strrchr(str->c_str, delimiter);
    if (pos == NULL) pos = str->c_str + str->length;
    str->length -= str->length - (int)pos + (int)str->c_str;
    *pos = 0;
}

void string_pop_front(struct String* str, char delimiter) {
    char* pos = strchr(str->c_str, delimiter);
    if (pos == NULL) pos = str->c_str + str->length; else pos++;
    str->length -= (int)pos - (int)str->c_str;
    str->c_str = pos;
}

struct String string_peek_back(struct String str, char delimiter) {
    char* pos = strchr(str.c_str, delimiter);
    if (pos == NULL) return str; else pos++;
    int length = str.length - (int)pos + (int)str.c_str;
    return string_cut(pos, length);
}

struct String string_peek_front(struct String str, char delimiter) {
    char* pos = strchr(str.c_str, delimiter);
    if (pos == NULL) return str;
    int length = (int)pos - (int)str.c_str;
    return string_cut(str.c_str, length);
}

int string_match(struct String str1, struct String str2) {
    for (int i = 0; i < str1.length && i < str2.length; i++) {
        if (str1.c_str[i] != str2.c_str[i])
            return i;
    }
    return (int)fmin(str1.length, str2.length);
}