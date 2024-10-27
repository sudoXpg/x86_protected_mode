#include "kernel.h"

#include <stdint.h>
#include<stddef.h>


uint16_t * video_mem =0;   // create ptr to abs address in memory
                                                // 16 bits since 2 bytes {char,color}

uint16_t terminal_row ;
uint16_t terminal_col ;

size_t strlen(const char* str){
    size_t len=0;
    while(str[len]){
        len++;
    }
    return len;
}


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




void kernel_main(){
    //video_mem[0]=0x0341; // small endian system !
    //video_mem[0]=terminal_make_char('B',14);    

    terminal_initialise();
    print("Hello World!\nFirst program!");

}