#ifndef SRC_UTILITIES_H
#define SRC_UTILITIES_H

#include <stddef.h>

char** split_string_on_delimiter(const char* string, const char delimiter);
void split_string_cleanup(char** split_string);
size_t count_char_in_string(const char c, const char* string);
char* trim_whitespace(const char* string);

#endif // !DEBUG
