[bits 32]
[extern main]
global a20_on
    a20_on db 0
    mov [a20_on], cx
call main    ;jump into kernel
jmp $