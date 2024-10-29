#ifndef HEAP_H
#define HEAP_H

#include "../../config.h"
#include <stdint.h>
#include <stddef.h>

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00

// bitmasks
#define HEAP_BLOCK_HAS_NEXT 0b10000000 // block to right is part of allocation
#define HEAP_BLOCK_IS_FIRST 0b01000000 // is the first entry of allocation

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;   // alias for an unsigned char

struct heap_table{
    HEAP_BLOCK_TABLE_ENTRY *entries;    // points to status of mem addresses
    size_t total;   // total blocks
};

struct heap{
    struct heap_table *table;   // pointer to heap table
    void *saddr;                // start address
};

int heap_create(struct heap *heap, void *ptr, void *end, struct heap_table *table);
void heap_free(struct heap *heap, void *ptr);
void *heap_malloc(struct heap *heap, size_t size);


#endif