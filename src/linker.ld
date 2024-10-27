ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
    . = 1M; /* kernel will be loaded at 1MB in memory address (1024*1024) */
    .text :
    {
        *(.text)
    }
    .rodata : 
    {
        *(.rodata)
    }
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(COMMON)
        *(.bss)
    }
}