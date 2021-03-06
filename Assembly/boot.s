[org 0x7c00]
KERNEL equ 0x1000
jmp 0x0000:Start16
BootDisc db 0
Start16:
    mov [BootDisc], dl
    xor ax, ax
    xor bx, bx
    xor dx, dx
    xor cx, cx
    xor si, si
    mov bp, 0x8000
    mov sp, bp
    mov bx, KERNEL
Set_Stack:
    mov ax, 0x0000
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov sp, 0x8000

    call getMemoryMap
Boot:
    mov ah, 0x00
    mov al, 0x3
    int 10h
    mov bx, 0
    ;read from disk, 128 sectors
    mov ah, 0x02
    mov al, 0x36 ;0x36 sectors
    mov ch, 0    ;cylinder 0
    mov cl, 0x03 ;sector 2
    mov dh, 0   ;head 0
    mov dl, [BootDisc] ;disk 0
    mov es, bx
    mov bx, KERNEL
    int 13h
    ; call detect_mem
    
    ProtectedStart:
    cli
    lgdt[GDTDESC]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp CODE:Protectedmode
    jmp $

;startmsg db "type start to start", 0x00
GDTSTART:
    nulldesc:
        dd 0
        dd 0
    codedesc:
        dw 0xffff
        dw 0
        db 0
        db 0b10011010
        db 0b11001111 ;long mode on gdt
        db 0
    datadesc:
        dw 0xffff
        dw 0
        db 0
        db 0b10010010
        db 0b11001111
        db 0
    GDTEND:
GDTDESC:
    dw GDTEND - GDTSTART -1
    dd GDTSTART
CODE equ codedesc - GDTSTART
DATA equ datadesc - GDTSTART
%include "Assembly/bootutils.s"
Extended_Memory_Size: db 0, 0
[bits 32]
Protectedmode:
    mov eax, 0
    mov ebx, 0
    mov ecx, 0
    cmp dl, [BootDisc]
    
    je check_a20
jmp $
check_a20:
    pushad
    push edi
    push esi
    mov edi, 0x112345
    mov esi, 0x012345
    mov [esi], esi
    mov [edi], edi
    cmpsd
    popad
    je a20_off
    ; call detect_mem
    mov byte cx, 1
    mov ax, [Extended_Size_1]
    mov bx, [Extended_Size_2]
    jmp KERNEL
    jmp $
a20_off:
    ; call detect_mem
    mov byte cx, 0
    mov ax, [Extended_Size_1]
    mov bx, [Extended_Size_2]
    jmp KERNEL
    jmp $
jmp $
a20_on: db 0
memory: dw 0
times 510-($-$$) db 0
db 0x55, 0xaa