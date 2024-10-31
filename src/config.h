#ifndef CONFIG_H
#define CONFIG_H

#define AeOS_TOTAL_INTERRUPTS 512           // total num of interrupts
#define KERNEL_CODE_SELECTOR 0x08           // from the boot.asm
#define KERNEL_DATA_SELECTOR 0x10           // from the boot.asm
#define AeOS_HEAP_SIZE_BYTES 1048576        // 100MB [size of the heap]
#define AeOS_HEAP_BLOCK_SIZE 4096           // min value to be returned in bytes upon malloc call
#define AeOS_HEAP_ADDRESS 0x01000000        // refer OSdev for the 'memory map x86'
#define AeOS_HEAP_TABLE_ADDRESS 0x00007E00  // convevtional memory for the table of 25600B size
#define AeOS_MAX_FILESYSTEMS 12
#define AeOS_MAX_FILE_DESCRIPTORS 512

#endif