[bits 32]
[extern main]
global a20_on
    a20_on db 0
    mov [a20_on], cx
global memsizelow
    memsizelow db 0, 0
    mov [memsizelow], ax
global memsizehigh
    memsizehigh db 0, 0
    mov [memsizehigh], bx
call main    ;jump into kernel
jmp $