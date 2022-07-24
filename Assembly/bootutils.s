detect_mem:
    pushad
    clc
    mov eax, ebp
    mov es, eax
    mov byte di, 0


    mov eax, 0xE820
    mov edx, 0x534D4150
    mov ecx, 24
    xor ebx, ebx
    int 0x15
    cmp eax, 0x534d4150
    jne error
    jc error
    add di, 4
    mov eax, 0xe820
    mov ecx, 24
    int 0x15
    jc error
    add di, 4
    mov eax, 0xe820
    mov ecx, 24
    int 0x15
    jc error
    add di, 2
    mov eax, 0xe820
    mov ecx, 24
    int 0x15

    ; it's not c, dummy
    looop:
        ;cmp cl, 24
        cmp ebx, 0
        je return
        inc di
        mov eax, 0xe820
        mov ecx, 24
        int 0x15
        jc error
        jmp looop
    int 0x15
    jc error

    jmp return
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

Extended_Size_1: db 0, 0
Extended_Size_2: db 0, 0


getMemoryMap:
    clc
    int 0x12
    jc ERROR
    

    xor cx, cx
    xor dx, dx
    mov ax, 0xe801
    int 0x15
    jc ERROR
    cmp ah, 0x86
    je ERROR
    cmp ah, 0x80
    je ERROR
    ;test ax, ax
    ;je ERROR
    
    mov [Extended_Size_1], cx
    mov [Extended_Size_2], dx
    

    ret
ERROR:
    ret