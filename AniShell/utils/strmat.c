#include "strmat.h"

#include <stdlib.h>
#include <string.h>
#include "string.h"
#include "../globals.h"

// Constructors List

Strmat strmat_empty() {
    struct Strmat matrix;
    matrix.c_arr = (char**)calloc(MAX_TOKENS_IN_COMMAND, sizeof(char*));
    matrix.lengths = (int*)calloc(MAX_TOKENS_IN_COMMAND, sizeof(int));
    for (int i = 0; i < MAX_TOKENS_IN_COMMAND; i++) {
        matrix.c_arr[i] = NULL;
        matrix.lengths[i] = 0;
    }
    matrix.length = 0;
    return matrix;
}

// Get and Put into the matrix

String strmat_get(Strmat mat, int pos) {
    return string_cut(mat.c_arr[pos], mat.lengths[pos]);
}

void strmat_put(Strmat* matrix, char* insert) {
    int length = (int)strlen(insert);
    matrix->c_arr[matrix->length] = (char*)calloc(MAX_LETTERS_IN_TOKEN, 1);
    strcpy(matrix->c_arr[matrix->length], insert);
    matrix->lengths[matrix->length] = length;
    matrix->length++;
}
