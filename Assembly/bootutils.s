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
        jmp $
    return:
    popad
    ret
