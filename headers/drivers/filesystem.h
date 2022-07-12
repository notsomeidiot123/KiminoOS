#include "diskdriver.h"
#include "../CLIOS.h"
#pragma once
#define EOF 0x3
typedef struct {
    char *filename;
    int ptr;
    uint32_t startAddress;
    uint8_t start_offset;
    uint32_t endAddress;
    uint8_t end_offset;
} file;

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

void fs_search(char *filename, file *fp){
    uint8_t matched = 0;
    char *drive = malloc(512);
    int addr = 0x234;
    uint8_t eof_found = 0;
    int addr_offset = 0;
    while(!matched){
        kLBAread(addr+= 1 , 1, 0, drive);
        matched = strmatch(drive, filename);
        printdc(addr);
        kprint(" Searching...\n");
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

int strmatch(char *str, char *delim){
    int len = strlen(delim);
    int index = 0;
    int offset = 0;
    while(*str){
        if(*str++ == delim[index]){
            index++;
            if(index == len){
                return offset - index;
            }
        }
        else{
            index = 0;
        }
        offset++;
    }
}