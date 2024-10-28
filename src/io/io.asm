section .asm

global isnb
global isnw
global outb
global outw



insb:
    push ebp
    mov ebp,esp
    
    xor eax,eax         ;make zero
    mov edx,[ebp+8]     ; retieves the port num passed to insb fxn 
                        ; accesses the stack where parameters are usually placed
    in al,dx            ; reads the port at address [ebp+8] => second parameter
    pop ebp
    ret                 ; returns al

;;
;;  [ebp+8] => second parameter
;;  [ebp+4] => first parameter
;;  [ebp] => return address
;;

insw:
    push ebp
    mov ebp,esp
    xor eax,eax
    mov edx,[ebp+8]
    in ax,dx
    pop ebp
    ret


outb:
    push ebp
    mov ebp,esp
    mov eax,[ebp+12] ; second arg
    mov edx,[ebp+8]
    out dx,al
    pop ebp
    ret

outw:
    push ebp
    mov ebp,esp
    mov eax,[ebp+12] ; second arg
    mov edx,[ebp+8]
    out dx,ax
    pop ebp
    ret
