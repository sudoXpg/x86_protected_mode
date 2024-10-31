#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "string/string.h"
#include "fs/pparser.h"
#include "disk/streamer.h"

#include <stdint.h>
#include<stddef.h>


uint16_t * video_mem =0;   // create ptr to abs address in memory
                                                // 16 bits since 2 bytes {char,color}

uint16_t terminal_row ;
uint16_t terminal_col ;



uint16_t terminal_make_char(char c, char color){
    return (color<<8)|c;    // color shifted 8 posn right and ORd with char
}

void terminal_putchar(int x,int y,char c,char color){
    video_mem[(y*VGA_WIDTH+x)]=terminal_make_char(c,color);
    // get the pixel locn from (x,y)
}

void terminal_writechar(char c, char color){ // increments the posns as well
    if(c=='\n'){
        terminal_row++;
        terminal_col=0;
        return;
    }
    terminal_putchar(terminal_col,terminal_row,c,color);
    terminal_col++;
    if(terminal_col>=VGA_WIDTH){
        terminal_col=0;
        terminal_row++;
    }
}

void terminal_initialise(){// clear src
    video_mem= (uint16_t *)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;
    
    for(int y=0;y<VGA_HEIGHT;y++){
        for(int x=0;x<VGA_WIDTH;x++){
            terminal_putchar(x,y,' ',0);
        }
    }
}

void print(const char* str){
    size_t len=strlen(str);
    for(int i=0;i<len;i++){
        terminal_writechar(str[i],15);
    }
}

static struct paging_4gb_chunk *kernel_chunk=0;

void panic(const char *msg){
    print(msg);
    while(1);
}


void kernel_main(){

// initialise display
    terminal_initialise();
    print(AeOS_LOGO);

// initialise heap
    kheap_init();

// search and initialise the disks
    disk_search_init();

//initialise the interrupts
    idt_init(); 

// initialise paging
    kernel_chunk=paging_new_4gb(PAGING_IS_WRITABLE|PAGING_IS_PRESENT|PAGING_ACCESS_FROM_ALL);
    // switch to paging chunk
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));
    // enable paging
    enable_paging();
// enable interrupts
    enable_interrupts();

// streamer 
    struct disk_stream* stream = disk_streamer_new(0);
    diskstream_seek(stream, 0x201);
    unsigned char c = 0;
    diskstreamer_read(stream, &c, 1);
    int i;
    while(1){
        i++;
    }
}