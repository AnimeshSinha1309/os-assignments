#ifndef ANISHELL_HISTORY_H
#define ANISHELL_HISTORY_H

#include "../utils/string.h"
#include "../globals.h"

void write_history(String str);
Strmat load_history();
void save_history(Strmat history_memory);
void history(int i);

#endif //ANISHELL_HISTORY_H
