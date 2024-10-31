#include "streamer.h"
#include "../memory/heap/kheap.h"
#include "disk.h"
#include "../config.h"

struct disk_stream *disk_streamer_new(int disk_id){
    struct disk *disk=disk_get(disk_id);
    if(!disk){
        return 0;
    }
    struct disk_stream *streamer= kzalloc(sizeof(struct disk_stream));
    streamer->pos=0;
    streamer->disk=disk;
    return streamer;
}

int diskstream_seek(struct disk_stream *stream, int pos){ // reposition the pos feild
    stream->pos=pos;
    return 0;
}

int diskstreamer_read(struct disk_stream *stream, void * out, int total){
    int sector=stream->pos / AeOS_SECTOR_SIZE;
    int offset=stream->pos % AeOS_SECTOR_SIZE;
    char buff[AeOS_SECTOR_SIZE];

    int res = disk_read_block(stream->disk,sector,1,buff);
    if(res<0){
        goto out;
    }
    int total_to_read=total>AeOS_SECTOR_SIZE?AeOS_SECTOR_SIZE:total;    // reads from buffer to out
    for(int i=0;i<total_to_read;i++){
        *(char *)out++=buff[offset+i];
    }

    // adjust stream
    stream->pos+=total_to_read;
    if(total>AeOS_SECTOR_SIZE){
        res=diskstreamer_read(stream,out,total-AeOS_SECTOR_SIZE);   // use recursion to read past first sector
    }

out:
    return res;
}

void diskstreaer_close(struct disk_stream *stream){
    kfree(stream);
}