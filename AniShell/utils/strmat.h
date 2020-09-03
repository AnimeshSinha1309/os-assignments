#ifndef ANISHELL_STRMAT_H
#define ANISHELL_STRMAT_H

#include "string.h"

struct Strmat {
    char** c_arr;
    int* lengths;
    int length;
};

// Constructors List

struct Strmat strmat_empty();

struct String strmat_get(struct Strmat mat, int pos);
void strmat_put(struct Strmat* matrix, char* insert);

#endif //ANISHELL_STRMAT_H
