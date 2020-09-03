#ifndef ANISHELL_STRING_H
#define ANISHELL_STRING_H

struct String {
    char* c_str;
    int length;
};

// Constructors List
struct String string_read();
struct String string_copy(struct String from);
struct String string_make(const char* from);
struct String string_cut(const char* from, int n);

// Tokenization type operations using single delimeter
void string_pop_back(struct String* str, char delimeter);
void string_pop_front(struct String* str, char delimeter);
struct String string_peek_back(struct String str, char delimeter);
struct String string_peek_front(struct String str, char delimeter);
int string_match(struct String str1, struct String str2);

// Other useful operations
struct String string_join(char* a, char* b);

#endif //ANISHELL_STRING_H
