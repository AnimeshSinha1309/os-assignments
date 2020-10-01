#ifndef ANISHELL_PIPELINE_H
#define ANISHELL_PIPELINE_H

#include "../globals.h"
#include "../utils/string.h"
#include "../utils/strmat.h"
#include "tokenizer.h"
#include "parser.h"

void pipeline(String input);
void redirections(String input);

void run_statement(String input);
void run_expression_or(String input);
void run_expression_and(String input);

#endif //ANISHELL_PIPELINE_H
