#ifndef ANISHELL_EXECVP_H
#define ANISHELL_EXECVP_H

#include "../utils/string.h"
#include "../globals.h"

void exec(char* command, char** args, bool background);

#endif //ANISHELL_EXECVP_H
