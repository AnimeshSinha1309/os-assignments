#ifndef ANISHELL_TOKENIZER_H
#define ANISHELL_TOKENIZER_H

#include "../utils/string.h"
#include "../utils/strmat.h"
#include "../globals.h"

Strmat tokenize_args(String input);
String process_path(String input);

#endif //ANISHELL_TOKENIZER_H
