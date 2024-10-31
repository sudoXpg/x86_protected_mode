#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

int is_digit(char c);
int to_numeric_digit(char c);
size_t strlen(const char* str);
int strnlen(const char* ptr, int max);

#endif