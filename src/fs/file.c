#include "file.h"
#include "../config.h"
#include "../memory/memory.h"
#include "../status.h"
#include "../memory/heap/kheap.h"

struct filesystem *filesystems[AeOS_MAX_FILESYSTEMS];                // array of file_system ptrs
struct file_descriptor *file_descriptors[AeOS_MAX_FILE_DESCRIPTORS]; // array of file_descriptors

static struct filesystem ** fs_get_free_filesystem(){
    for(int i=0;i<AeOS_MAX_FILESYSTEMS;i++){
        if(filesystems[i]==0){
            return &filesystems[i];
        }
    }
    return 0;
}

void fs_insert_filesystem(struct filesystem *filesystem){       // for drivers to insert itself in the filesystem
    struct filesystem **fs;
    fs=fs_get_free_filesystem();
    if(!fs){
        //panic
        while(1);
    }

    *fs =filesystem;    // fills the filesystem array
}

static void fs_static_load(){   // filesystems built in the kernel
    //fs_insert_filesystem(fat16_init());

}

void fs_load(){     // load all available filesystems
    memset(filesystems,0,sizeof(filesystems));
    fs_static_load();
}

void fs_init(){
    memset(file_descriptors,0,sizeof(file_descriptors));
    fs_load();
}


static int file_new_descriptor(struct file_descriptor **desc_out){
    int res=-ENOMEM;    // no mem error
    for(int i=0;i<AeOS_MAX_FILE_DESCRIPTORS;i++){   // loop through all possible file descriptors
        if(file_descriptors[i]==0){  // file descriptor is free to use
            struct file_descriptor *desc =kzalloc(sizeof(struct file_descriptor));
            // descriptors start at 1
            desc->index=i+1;
            file_descriptors[i]=desc;
            *desc_out=desc;
            res =0;
            break;
        }
    }
    return res;
}

static struct file_descriptor *file_get_descriptor(int fd){
    if(fd<=0 || fd>=AeOS_MAX_FILE_DESCRIPTORS){
        return 0;
    }
    // descriptors start at one
    int index=fd-1;
    return file_descriptors[index];
}

struct filesystem *fs_resolve(struct disk *disk){
    struct filesystem *fs=0;
    for(int i=0;i<AeOS_MAX_FILESYSTEMS;i++){
        if(filesystems[i]!=0 && filesystems[i]->resolve(disk)==0){
            fs = filesystems[i];
            break;
        }
    }
    return fs;
}

int fopen(const char *filename, const char *mode){
    return -EIO;
}