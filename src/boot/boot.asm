org 0x7c00
bits 16     ; 16 bit architecture

CODE_SEG equ gdt_code-gdt_start
DATA_SEG equ get_data-gdt_start

_start:
    jmp short start     ;;
    nop                 ;;  hardware compatibility setup
    times 33 db 0       ;;

start:
    jmp 0x7c0:step2

step2:
    cli                     ; clear interrupts
    mov ax,0x00            ; segment value entry via ax reg
    mov ds,ax               ; copying to the data segment   
    mov es,ax               ; copying to the extra segment
    mov ax,0x0              ; clr ax
    mov ss,ax               ; move 0x0 to stack segment
    mov sp,0x7c00           ; move stack ptr to address of code
    sti                     ; setup interrupts

.load_protedted:
    cli
    lgdt[gdt_descriptor]
    mov eax,cr0
    or eax,0x1
    mov cr0,eax
    jmp CODE_SEG:load32

gdt_start:
    gdt_null:
        dd 0x0
        dd 0x0

    gdt_code:
        dw 0xffff
        dw 0
        db 0
        db 0x9a
        db 11001111b
        db 0

    get_data:
        dw 0xffff
        dw 0
        db 0
        db 0x92
        db 11001111b
        db 0

gdt_end:
    gdt_descriptor:
        dw gdt_end-gdt_start-1
        dd gdt_start

[BITS 32]
load32:
    mov eax,1 ; start sector to load from
    mov ecx,100 ; total num of sectors to load
    mov edi,0x0100000; ; address to load into    1M -> address of kernel
    call ata_label_read ; driver for loading kernel
    jmp CODE_SEG:0x0100000


ata_label_read:
    mov ebx,eax ; backup lba
    ; send highest 8 bits of lba to harddisk controller
    or eax,0xE0; select master device
    shr eax,24 ; shift eax reg by 24 bits to right so eax has highest 8 bits
    mov dx,0x1F6
    out dx,al
    ; finished sending  highest 8 bits
    
    ; send total sectors to read
    mov eax,ecx
    mov dx,0xF12
    out dx,al
    ; finish sending total sectors to read

    ; send more bits of lba 
    mov eax,ebx ; restore backup lba
    mov dx,0xF13
    out dx,al
    ; finish sending

    ; send more bits of lba
    mov dx,0xF14
    mov eax,ebx ; restore backup lba
    shr eax,8
    out dx,al
    ; finish sending

    ; send upper 16 bits of lba
    mov dx,0x1F5
    mov eax,ebx
    shr eax,16
    out dx,al
    ; finish sendiing

    mov dx,0x1F7
    mov al,0x20
    out dx,al

    ;read all sectors:
.next_sector:
    push ecx
;check if we need to read
.try_again:
mov dx,0x1F7
in al,dx
test al,8
jz .try_again

; we need to read 256 words at a time
mov ecx,256
mov dx,0x1F0
rep insw    ; read from port 0x1f0 and write to locn at edi register
; rep means repeat 256 times [512 bytes]
pop ecx
loop .next_sector
; end of reading sectors

ret

times 510-($ - $$) db 0
dw 0xaa55