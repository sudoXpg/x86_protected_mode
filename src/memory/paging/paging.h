#ifndef PAGING_H
#define PAGING_H

#include<stdint.h>
#include<stddef.h>

#define PAGING_CACHE_DISABLE   0b00010000 // bit 4 of page table entry = Cache disable bit
#define PAGING_WRITE_THROUGH   0b00001000 // bit 3 = W
#define PAGING_ACCESS_FROM_ALL 0b00000100 // U/S = 1 for access by all priviledge levels
#define PAGING_IS_WRITABLE     0b00000010
#define PAGING_IS_PRESENT      0b00000001

#define PAGING_TOTAL_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE 4096


struct paging_4gb_chunk{
    uint32_t *directory_entry;        // ptr to page directory

};

void paging_switch(uint32_t *directory);
struct paging_4gb_chunk *paging_new_4gb(uint8_t flags);
uint32_t *paging_4gb_chunk_get_directory(struct paging_4gb_chunk *chunk);

void enable_paging();   // call after the above fxns

#endif