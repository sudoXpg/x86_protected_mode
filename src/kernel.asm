[BITS 32]

global _start   ; for symbol to be global for obj files
extern kernel_main  ; use the c function


CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Enable the A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    ; start remap the master PIC
    mov al,00010001b    ; keep PIC in initialisation mode
    out 0x20,al         ; tell master PIC to start initialisation mode

    mov al,0x20         ; interrupt 0x20 is where master ISR should start
    
    out 0x21,al

    mov al,00000001b    ; finish init
    out 0x21,al

    ; 0x20 => master PIC [command]
    ; 0x21 => master PIC [data]

    ; since we have mapped IRQ to 20 , IRQ 1 = 20+1= int 21

    ; end remap master PIC
    
    ; quite dangerous to do this since idt not enabled yet so the system could panic !!
    ;sti
         
    call kernel_main

    jmp $


times 512-($ - $$) db 0