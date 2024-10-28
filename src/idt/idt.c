#include "idt.h"
#include "../config.h"
#include "../memory/memory.h"
#include "../kernel.h"

struct idt_desc idt_descriptors[AeOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

extern void idt_load(struct idtr_desc* ptr);

void idt_zero(){
    print("Divide by zero error!\n");
}


void idt_set(int interrupt_num,void *address){
    struct idt_desc* desc = &idt_descriptors[interrupt_num];
    desc->offset_1=(uint32_t) address & 0x0000ffff;     // get lower part of address
    desc->selector=KERNEL_CODE_SELECTOR;
    desc->zero=0x00;
    desc->type_attr=0xEE ;      // EE = 1 11 0 1110 {P DPL STORAGE_SEG(=0 for int) GATE_TYPE}
    desc->offset_2=(uint32_t) address>>16; // to get higher part of address

}


void idt_init(){
    memset(idt_descriptors,0,sizeof(idt_descriptors));
    idtr_descriptor.limit= sizeof(idt_descriptors)-1;
    idtr_descriptor.base=(uint32_t) idt_descriptors;

    idt_set(0,idt_zero);    // stores the address from the name of fxn
    idt_load(&idtr_descriptor);  // pass to the asm code
}