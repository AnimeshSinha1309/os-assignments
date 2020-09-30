#ifndef ANISHELL_PROMPT_H
#define ANISHELL_PROMPT_H

#include "../utils/string.h"

struct String home_path;
struct String prev_path;
int exit_code;

void prompt_init();
void prompt_print();

#endif //ANISHELL_PROMPT_H
