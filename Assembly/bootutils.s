detect_mem:
    pushad
    clc
    mov eax, 0x100000
    mov es, eax
    mov byte di, 0

    mov eax, 0xE820
    mov edx, 0x534D4150
    mov ecx, 24
    mov ebx, 0
    int 0x15
    cmp eax, 0x534d4150
    jne error
    je return
    error:
        mov byte di, 0
        mov byte [es:di], 0xff;
        mov bx, errormsg
        call prints
        jmp $
    return:
        popad
        ret
errormsg: db "Failed to get size of memory, please restart your computer", 10, 13, 0
prints:
    mov ah, 0x0e
    prloop:
        cmp byte [bx], 0
        jz endpr
        mov al, [bx]
        inc bx
        int 10h
        jmp prloop
    endpr:
        ret