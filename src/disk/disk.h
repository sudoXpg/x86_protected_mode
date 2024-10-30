#ifndef DISK_H
#define DISK_H

#define AeOS_SECTOR_SIZE 512    // sector size of a normal hdd 
#define AeOS_DISK_TYPE_REAL 0   // represents a real physical harddisk
typedef unsigned int AeOS_DISK_TYPE;

struct disk{
    AeOS_DISK_TYPE type;
    int sector_size;
};


void disk_search_init();
struct disk *disk_get(int index);
int disk_read_block(struct disk *idisk, unsigned int lba, int total, void *buff);
#endif