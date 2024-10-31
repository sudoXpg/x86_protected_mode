#ifndef MEMORY_H
#define MEMORY_H

#include<stddef.h>  // can use since is compiler provided || doesnt depend on os like other libs

void *memset(void *ptr,int c,size_t size);
int memcmp(void *s1, void *s2, int count);
#endif