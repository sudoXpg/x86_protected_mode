section .asm

extern int21h_handler
extern no_interrupt_handler

global idt_load
global int21h
global no_interrupt

idt_load:
    push ebp
    mov ebp,esp
    mov ebx,[ebp+8]
    lidt[ebx]
    pop ebp
    ret

int21h:
    cli         ;  clear all int

    pushad      ; backup all gen purpouse reg
    call int21h_handler
    popad       ; restores all gen purpouse reg
    
    sti         ; start all int
    iret

no_interrupt:
    cli
    pushad
    call no_interrupt_handler
    popad   
    sti
    iret