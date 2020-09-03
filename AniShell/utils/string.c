#include "string.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../globals.h"

// Constructors List

String string_copy(String from) {
    String to;
    to.c_str = (char*)calloc(MAX_LETTERS_IN_TOKEN, 1);
    to.length = from.length;
    strncpy(to.c_str, from.c_str, from.length);
    return to;
}

String string_make(const char* from) {
    String to;
    to.c_str = (char*)calloc(MAX_LETTERS_IN_TOKEN, 1);
    to.length = (int)strlen(from);
    strcpy(to.c_str, from);
    return to;
}

String string_cut(const char* from, int n) {
    String to;
    to.c_str = (char*)calloc(MAX_LETTERS_IN_TOKEN, 1);
    to.length = n;
    strncpy(to.c_str, from, n);
    return to;
}

String string_empty() {
    String to;
    to.c_str = (char*)calloc(MAX_LETTERS_IN_TOKEN, 1);
    to.c_str[0] = '\0';
    to.length = 0;
    return to;
}

// Tokenization type operations using single delimiter

void string_pop_back(String* str, char delimiter) {
    char* pos = strrchr(str->c_str, delimiter);
    if (pos == NULL) pos = str->c_str + str->length;
    str->length -= str->length - (int)pos + (int)str->c_str;
    *pos = 0;
}

void string_pop_front(String* str, char delimiter) {
    char* pos = strchr(str->c_str, delimiter);
    if (pos == NULL) pos = str->c_str + str->length; else pos++;
    str->length -= (int)pos - (int)str->c_str;
    str->c_str = pos;
}

String string_peek_back(String str, char delimiter) {
    char* pos = strchr(str.c_str, delimiter);
    if (pos == NULL) return str; else pos++;
    int length = str.length - (int)pos + (int)str.c_str;
    return string_cut(pos, length);
}

String string_peek_front(String str, char delimiter) {
    char* pos = strchr(str.c_str, delimiter);
    if (pos == NULL) return str;
    int length = (int)pos - (int)str.c_str;
    return string_cut(str.c_str, length);
}

int string_match(String str1, String str2) {
    for (int i = 0; i < str1.length && i < str2.length; i++) {
        if (str1.c_str[i] != str2.c_str[i])
            return i;
    }
    return (int)fmin(str1.length, str2.length);
}

// Other useful operations

String string_join(char* a, char* b) {
    String result = string_make(a);
    result.c_str = strcat(result.c_str, b);
    result.length = strlen(result.c_str);
    return result;
}

struct String string_itoa(int value) {
    String result = string_empty();
    sprintf(result.c_str, "%d", value);
    result.length = strlen(result.c_str);
    return result;
}
