#include "heap.h"
#include "../../kernel.h"
#include "../../status.h"
#include "../memory.h"

static int heapp_validate_table(void *ptr, void *end, struct heap_table *table){
    int res=0;

    size_t table_size=(size_t) (end-ptr);
    size_t total_blocks=table_size/AeOS_HEAP_BLOCK_SIZE;
    if(table->total!=total_blocks){
        res=-EINVARG;
        goto out;
    }
out:
    return res;
}


static int heap_validate_alignment(void *ptr){ 
    return (((unsigned int)ptr % AeOS_HEAP_BLOCK_SIZE)==0);            // returns true if the pointer is aligned
}


int heap_create(struct heap *heap, void *ptr, void *end, struct heap_table *table){ // ptr => address of heap pool
    int res=0;
    // use 
    // res = -EIO ; => for sending an error symbol!
    if(!heap_validate_alignment(ptr) || !heap_validate_alignment(ptr)){
        res=-EINVARG;   // error in valid arg
        goto out;
    }

    memset(heap,0,sizeof(struct heap));     // initialises the whole heap to zero
    heap->saddr=ptr;
    heap->table=table;

    res=heapp_validate_table(ptr,end,table);
    if(res<0){
        goto out;
    }

    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY)*table->total;    // 1byte (since a char) * table.total
    memset(table->entries,HEAP_BLOCK_TABLE_ENTRY_FREE,table_size);      // set all table entries as 0x00

out:
    return res;
}


static uint32_t heap_align_value_to_upper(uint32_t val){
    if((val%AeOS_HEAP_BLOCK_SIZE)==0){      // already a multiple of 4096 ==> aligned
        return val;
    }
    val=(val-(val%AeOS_HEAP_BLOCK_SIZE));   // align as per the block size (4096)
    val+=AeOS_HEAP_BLOCK_SIZE;
    return val;
}

static int heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry){
    return entry & 0x0f;    // get lower 4 bytes [entry type desc]
}

int heap_get_start_block(struct heap *heap, uint32_t total_blocks){
    struct heap_table *table = heap->table;     // store the table deets from the heap struct onto 'table' var
    int bc=0;   // current block
    int bs=-1;  // free found block's start block

    for(size_t i=0;i<table->total;i++){
        if(heap_get_entry_type(table->entries[i])!=HEAP_BLOCK_TABLE_ENTRY_FREE){
            bc=0;
            bs=-1;
            continue;
        }

        // since this is the first block we need to check if consecutive blocks free
        if(bs==-1){
            bs=i;
        }
        bc++;       // checks until bc = blocks needed
        if(bc==total_blocks){
            break;
        }
    }
    if(bs==-1){
        return -ENOMEM;
    }

    return bs;
}

void *heap_block_to_address(struct heap *heap, uint32_t start_block){
    return heap->saddr+(start_block*AeOS_HEAP_BLOCK_SIZE);
}

void heap_mark_blocks_taken(struct heap *heap, uint32_t start_block, uint32_t total_blocks){
    uint32_t end_block = start_block+total_blocks-1;    // accomodate zero indexing
    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    if(total_blocks>1){
        entry|=HEAP_BLOCK_HAS_NEXT;
    }
    for(int i=start_block;i<=end_block;i++){
        heap->table->entries[i]=entry;
        entry=HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if(i!=end_block-1){
            entry|=HEAP_BLOCK_HAS_NEXT;
        }
    }
}

void *heap_malloc_blocks(struct heap *heap, uint32_t total_blocks){
    void *address=0;        // initialise as zero
    int start_block=heap_get_start_block(heap,total_blocks);    // look for space by using the heap allocation table
    if(start_block<0){
        goto out;       // couldnt find memory
    }

    address=heap_block_to_address(heap,start_block);            // find abs address by using offset by table
    heap_mark_blocks_taken(heap,start_block,total_blocks);      // mark the blocks as taken type


out:
    return address;
}


void *heap_malloc(struct heap *heap, size_t size){
    size_t aligned_size=heap_align_value_to_upper(size);
    uint32_t total_blocks=aligned_size/AeOS_HEAP_BLOCK_SIZE;
    return heap_malloc_blocks(heap,total_blocks);
}

int heap_addres_to_block(struct heap *heap, void * address){
    return (int)(address -(int) heap->saddr)/AeOS_HEAP_BLOCK_SIZE;
}

void heap_mark_blocks_free(struct heap *heap, int starting_block){
    struct heap_table *table = heap->table;
    for(int i=starting_block;i<(int)table->total; i++){  // goes from start of the entry block to the end of the whole table since we dontknow how many blocks to free
        HEAP_BLOCK_TABLE_ENTRY entry=table->entries[i];
        table->entries[i]=HEAP_BLOCK_TABLE_ENTRY_FREE;
        if(!(entry & HEAP_BLOCK_HAS_NEXT)){     // bit not set
            break;
        }
    }
}


void heap_free(struct heap *heap, void *ptr){

    heap_mark_blocks_free(heap,heap_addres_to_block(heap,ptr));
}