#ifndef ANISHELL_SYSCOM_H
#define ANISHELL_SYSCOM_H

#include "../utils/string.h"
#include "../globals.h"

void setenvr(String var, String value);

void unsetenvr(String var);

void jobs();

void kjob(int pid, int signal);

void overkill();

void bg(int pid);

void fg(int pid);


#endif //ANISHELL_SYSCOM_H
