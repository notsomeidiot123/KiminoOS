[bits 32]
[extern main]
global memory
global a20_on
    a20_on db 0
    mov [a20_on], bx
    memory dw 0
    mov [memory], ax
call main    ;jump into kernel
jmp $