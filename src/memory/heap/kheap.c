#include "kheap.h"
#include "heap.h"
#include "../../config.h"
#include "../../kernel.h"

struct heap kernel_heap;                // no need to init
struct heap_table kernel_heap_table;    // need to init


void kheap_init(){
    int total_table_entries = AeOS_HEAP_SIZE_BYTES  /AeOS_HEAP_BLOCK_SIZE ;
    kernel_heap_table.entries=(HEAP_BLOCK_TABLE_ENTRY *)AeOS_HEAP_TABLE_ADDRESS;
    kernel_heap_table.total=total_table_entries;

    void *end = (void *) (AeOS_HEAP_ADDRESS+AeOS_HEAP_SIZE_BYTES);  // end = start + total values 
    int res = heap_create(&kernel_heap,(void *)AeOS_HEAP_ADDRESS,end,&kernel_heap_table);
    if(res<0){
        print("Failed to create heap\n");
    }
}


void *kmalloc(size_t size){
    return heap_malloc(&kernel_heap,size);
}


void kfree(void *ptr){
    heap_free(&kernel_heap,ptr);
}