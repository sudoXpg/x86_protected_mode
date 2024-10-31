#ifndef PATHPARSER_H
#define PATHPARSER_H

#include "../kernel.h"
#include "../string/string.h"
#include "../memory/heap/kheap.h"
#include "../memory/memory.h"
#include "../status.h"

struct path_root{       // get the drive num from this and the rest is part of the path_part struct
    int drive_num;
    struct path_part *first;
};


struct path_part{
    const char *path;
    struct path_part *next;
};

struct path_root* pathparser_parse(const char* path, const char* current_directory_path);
void pathparser_free(struct path_root* root);

#endif