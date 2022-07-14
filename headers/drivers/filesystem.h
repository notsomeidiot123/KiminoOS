#include "diskdriver.h"
// #include "../drivers/keyboard.h"
#include "../CLIOS.h"
#pragma once
#define EOF 0x3


void exec_file(file *file);

int install_fs(){
    char end_found= 0;
    int start_search = 0x47200;
    while(!end_found){
        
    }
}
//start searching near end (0x35 dector)
/*mov al, 0x36 ;0x36 sectors
    mov ch, 0    ;cylinder 0
    mov cl, 0x02 ;sector 0
    mov dh, 0   ;head 0
    mov dl, [BootDisc] ;disk 0
    mov es, bx
    mov bx, KERNEL
    int 13h*/
//LBA = (0 x 16 + 0) x 63 + (0x35 - 1)

void fs_search(char *filename, file *fp, DRIVE *driveStruct){
    uint8_t matched = 0;
    char *drive = malloc(512);
    int addr = 0x234;
    uint8_t eof_found = 0;
    int addr_offset = 0;
    while(!matched){
        kLBAread(addr+= 1 , 1, 0, drive, driveStruct);
        matched = strmatch(drive, filename);
        if(addr >= driveStruct->lba_max_address){
            addr = 0;
        }
        if(matched > 0){
            fp->startAddress = addr;
            fp->start_offset = matched;
            break;
            // while(!eof_found){
            //     kLBAread(addr++, 1, 0, drive);
            //     eof_found = strmatch(drive, EOF);
            //     if(eof_found){
            //         fp->endAddress = addr;
            //         fp->end_offset = eof_found;
            //     }
            // }
        }
    } //find max address, then if addr >= max, overflow to 0
}

