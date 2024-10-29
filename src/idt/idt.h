#ifndef IDT_H
#define IDT_H
#include<stdint.h>

void idt_init();
extern void enable_interrupts();
extern void disable_interrupts();
struct idt_desc{
    
    uint16_t offset_1;    // offset bits 0-15
    uint16_t selector;    // selector in the GDT
    uint8_t zero;         // unused set to zero
    uint8_t type_attr;      // descriptor type and attributes
    uint16_t offset_2;      // offset bits 16-31

}__attribute__((packed));   // to prevent any alignment issues

struct idtr_desc{
    uint16_t limit;         // size of descriptor table
    uint32_t base;          // address of start of descriptor table

}__attribute__((packed));   // to prevent any alignment issues


#endif