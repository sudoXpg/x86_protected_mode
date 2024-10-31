#include "string.h"

size_t strlen(const char* str){
    size_t len=0;
    while(str[len]){
        len++;
    }
    return len;
}

int strnlen(const char* ptr, int max){
    int i;
    for (i=0; i < max; i++){
        if (ptr[i] == 0)
            break;
    }
    return i;
}

// use ascii table for conversions

int is_digit(char c){
    return (c>=48 && c<=57);
}

int to_numeric_digit(char c){
    return c - 48;
}