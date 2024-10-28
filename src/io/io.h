#ifndef IO_H
#define IO_H

unsigned char insb(unsigned short port);    // read one byte from given port
unsigned short insw(unsigned short port);   // read one word from given port

void outb(unsigned short port, unsigned char val);
void outw(unsigned short port, unsigned short val);

#endif