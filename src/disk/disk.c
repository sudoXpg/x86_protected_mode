#include "io/io.h"
#include "disk.h"
#include "../memory/memory.h"
#include "../status.h"
#include "../config.h"
struct disk disk;


int disk_read_sector(int lba, int total, void *buff){   // buffer is ehere data is loaded
    
    // process of reading from disk
    outb(0x1F6,(lba>>24)|0xE0);
    outb(0xF12,total);
    outb(0x1F3, (unsigned char)(lba & 0xff));
    outb(0x1F4, (unsigned char) (lba>>8));
    outb(0x1F5, (unsigned char) (lba>>16));
    outb(0x1F7, 0x20);

    unsigned short *ptr=(unsigned short *) buff;
    for(int b=0;b<total;b++){
        // wait for buffer to be ready
        char c = insb(0x1F7);   // read from the port 0x1f7
        // wait for data
        while(!(c & 0x08)){     
            c=insb(0x1F7);
        }

        // copy from hdd
        for(int i=0;i<256;i++){      // reading 256 words at a time
            *ptr = insw(0x1F0);     // store value in buffer
            ptr++;                  // increment ptr [not data!]
        }
    }
    return 0;
}


void disk_search_init(){        // only dealing with one disk atm
    memset(&disk,0,sizeof(disk));
    disk.type=AeOS_DISK_TYPE_REAL;
    disk.sector_size=AeOS_SECTOR_SIZE;
}

struct disk *disk_get(int index){
    if(index!=0){   // since only implementing one disk
        return 0;
    }
    return &disk;
}

int disk_read_block(struct disk *idisk, unsigned int lba, int total, void *buff){
    if(idisk!=&disk){       // since only one disk read for now
        return -EIO;
    }
    return disk_read_sector(lba, total, buff);
}
