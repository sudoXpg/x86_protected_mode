#ifndef STREAMER_H
#define STREAMER_H



struct disk_stream{
    int pos;        // byte posn
    struct disk *disk;
};

struct disk_stream *disk_streamer_new(int disk_id);
int diskstream_seek(struct disk_stream *stream, int pos);
int diskstreamer_read(struct disk_stream *stream, void * out, int total);
void diskstreaer_close(struct disk_stream *stream);


#endif