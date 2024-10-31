#ifndef FILE_H
#define FILE_H

#include "pparser.h"

typedef unsigned int FILE_SEEK_MODE;
enum{
    SEEK_SET,
    SEEK_CURR,
    SEEK_END
};

typedef unsigned int FILE_SEEK_MODE;
enum{
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};

struct disk;

// function pointers since exact file system unknown so just point to the function needed, this is just a skeleton
typedef void *(*FS_OPEN_FUNCTION)(struct disk *disk, struct path_part *path, FILE_SEEK_MODE mode);
typedef int (*FS_RESOLVE_FUNCTION)(struct disk *disk);


struct filesystem{
    // filesystem should return 0 from resolve if the disk is using a known filesystem
    FS_RESOLVE_FUNCTION resolve;
    FS_OPEN_FUNCTION open;

    char name[20];  // file-system name [FAT16/NTFS]
};

struct file_descriptor{
    int index;
    struct filesystem *filesystem;

    // private data for internal file descriptor
    void *private;  // pvt data to be stored

    // the disk the file descriptor should be used on
    struct disk *disk;
};

void fs_init();
int fopen(const char *filename, const char *mode);
void fs_insert_filesystem(struct filesystem *filesystem);
struct filesystem *fs_resolve(struct disk *disk);



#endif