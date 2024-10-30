#include "paging.h"
#include "../heap/kheap.h"

void paging_load_directory(uint32_t *directory);

static uint32_t* current_directory=0;

struct paging_4gb_chunk *paging_new_4gb(uint8_t flags){     // gives linear memory || every virt address points to phy adderess
    uint32_t *directory = kzalloc(sizeof(uint32_t)*PAGING_TOTAL_ENTRIES_PER_TABLE); // 1024 entries of 32bit ptrs  || page directory entry
    int offset=0;
    for(int i=0;i<PAGING_TOTAL_ENTRIES_PER_TABLE;i++){
        uint32_t *entry= kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE); // page table entry || 1024 page table entries
        for(int j=0;j<PAGING_TOTAL_ENTRIES_PER_TABLE;j++){
            entry[j]=(offset+(j*PAGING_PAGE_SIZE))|flags;   // maping the memory
        }
        offset+=(PAGING_TOTAL_ENTRIES_PER_TABLE*PAGING_PAGE_SIZE);
        directory[i]=(uint32_t)entry|flags|PAGING_IS_WRITABLE;      // directory = pointer to page entry, only initial bits needed for paging so we or the lower bits as status flags
    }

    struct paging_4gb_chunk *chunk_4gb=kzalloc(sizeof(struct paging_4gb_chunk));
    chunk_4gb->directory_entry=directory;
    return chunk_4gb;
}


uint32_t *paging_4gb_chunk_get_directory(struct paging_4gb_chunk *chunk){
    return chunk->directory_entry;
}

void paging_switch(uint32_t *directory){
    paging_load_directory(directory);
    current_directory=directory;
}