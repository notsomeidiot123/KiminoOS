#include <stdint.h>
#pragma once
//basics
#define null 0
#define _CLIOS_ 0x1000
#define _BITS 32
#define _VGAFB 0xb8000
#define _CLIOSVER "0.1.0a"
#define _RESX 80
#define _RESY 25
//colors
#define WHITE 0xf
#define YELLOW 0xe
#define PINK 0xd
#define LRED 0xc
#define LCYAN 0xb
#define LGREEN 0xa
#define LBLUE 0x9
#define DGRAY 0x8
#define LGRAY 0x7
#define BROWN 0x6
#define MAGENTA 0x5
#define RED 0x4
#define CYAN 0x3
#define GREEN 0x2
#define BLUE 0x1
#define BLACK 0x00
#define true 1
#define false 0
uint16_t *vga_framebuffer = (uint16_t*)_VGAFB;
int index = 80;
int xpos = 0;
int ypos = 0;
unsigned int *memIndex = (unsigned int*)0x00000000;
unsigned int **usedMem;
unsigned int usedmemindex = 0;
char keepLastLine = 1;
enum RET_ERRORS{
    SUCCESS = 0,
    GENERIC_FAIL = 1,
    MEM_FAIL = 2,
    FILE_ERROR = 3,
    FILE_NOT_FOUND = 4,
    DRIVE_ERROR = 5,
    FRAMEBUFFER_ERROR = 6,
    FLOPPY_ERROR = 7,
    KEYBOARD_ERROR = 8,
    MOUSE_ERROR = 9, //not implemented
    UNSUPPORTED_ERROR = 10,
    UNKNOWN_ERROR = 11,
    BUFFER_TOO_LARGE = 12,
    RESERVED_LO = 13,
    RESERVED_MID = 14,
    RESERVED_HI = 15,
};
enum APP_ERRORS{
    //123456789ABCDEF
    NOT_RESPONDING = 0xE1F124, //elfish
    FAILED_START = 0x10095, //!OOPS
    MEMORY_PROTECTION = 0x74E1F, //THEIF
};
int past = 0;
uint8_t inb(uint16_t port){
	uint8_t res;
	asm volatile("inb %1, %0" : "=a"(res) : "Nd"(port));
	return res;
}
void outb(uint16_t port, uint8_t data){
	asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
	return;
}
uint16_t inw(uint16_t port){
    uint16_t res;
    asm volatile("inw %1, %0" : "=a"(res) : "Nd"(port));
    return res;
}
void outw(uint16_t port, uint16_t data){
    asm volatile("outw %0, %1" : : "a"(data), "Nd"(port));
    return;
}
void update_cursor(int x, int y)
{
	uint16_t pos = y * _RESX + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
int print(const char *str, char caps){
    
    while(*str){
        xpos++;
        if (*str == '\n')
        {
            back:
            index += 80-xpos + 1;
            xpos = 0;
            str++;
            if(index > _RESX * _RESY){  
                memcpy(vga_framebuffer, vga_framebuffer + _RESX, _RESY  * _RESX * 2);
                ypos = _RESY -1;
                index = (_RESX * _RESY)-80;
                xpos = 0;
            }
            continue;
        }
        else if (*str == '\r'){
            index -= xpos - 1;
            xpos = 0;
            str++;
            continue;
        }
        else if (*str == '\t')
        {
            for(int i = 4; i > 0; i--){
                vga_framebuffer[index++] == 0x20 | WHITE << 8;
                xpos++;
                if(xpos >= 80){
                    xpos = 0;
                    break;
                }
            }
            index++;
            str++;
            continue;
        }
        if(caps && *str < 123 && *str > 96){
            vga_framebuffer[index++] = (*str++ - 32) | WHITE << 8;
            continue;
        }
        else if(caps){
            if(*str == '\''){
                vga_framebuffer[index++] = '\"' | WHITE << 8;
                
            }
            str++;
            continue;
        }
        else{
            vga_framebuffer[index++] = *str++ | WHITE << 8;
        }
        if(xpos >= 80){
            xpos = 0;
        }
    }
    ypos = index / _RESX;
    if(index > _RESX * _RESY){ 
        memcpy(vga_framebuffer, vga_framebuffer + _RESX, _RESY  * _RESX * 2);
        ypos = _RESY -1;
        index = (_RESX * _RESY)-80;
        xpos = 0;
        if(past = 1){
            index+=2;
            xpos +=2;
        }
    }
    update_cursor(xpos, ypos);
    return 0;
}
void delkey(){
    index--;
    vga_framebuffer[index] = 0x00 | 0xf << 8;
    xpos--;
    //fix cursor glitch eventually
    if(xpos < 0){
        xpos = 79;
        ypos --;
    }
    update_cursor(xpos, ypos);
    return;
}
int clear(void){
    index = 0;
    xpos = 0;
    for(int i = 0; i < _RESX * _RESY; i++){
        vga_framebuffer[i] = 0x00 | WHITE << 8;
    }
}
char *malloc(int size);
char *free(int size);
int printdc(int num){
    int tmpn = num;
    char *buffer = malloc(512);
    char *base = buffer;
    *buffer = 0;
    do{
        *(buffer++) = tmpn % 10 + 48;
        tmpn /= 10;
    }while(tmpn > 0);
    do{
        char *tmp = malloc(2);
        *tmp = *(buffer--);
        *(tmp+1) = 0;
        print(tmp, 0);
        num /= 10;
        tmp = free(2);
    }while(*buffer);
}
/*
int printdc(int num){
    int tmpn = num;
    char *buffer = malloc(512);
    char *base = buffer;
    *buffer = 0;
    do{
        *(buffer++) = tmpn % 10 + 48;
        tmpn /= 10;
    }while(tmpn > 0);
    do{
        char *tmp = malloc(2);
        *tmp = *(buffer--);
        *(tmp+1) = 0;
        print(tmp, 0);
        num /= 10;
        tmp = free(2);
    }while(*buffer);
}
*/
int freemem = 0x100000;
char* malloc(int size){
    unsigned int* dest = (unsigned int*)size;
    char* address = (char*)freemem;
    freemem += size;
    return address;
}
char *inmalloc(int size, int initializer){
    unsigned int* dest = (unsigned int*)size;
    for(int i = 0; i < size; i++){
        dest[i] = initializer;
    }
    char* address = (char*)freemem;
    freemem += size;
    return address;
}
char* free(int size){
    freemem -= size;
    return null;
}
char* clearmem(int size){
    unsigned int* dest = (unsigned int*)size;
    freemem -= size;
    for(int i = 0; i < size; i++){
        dest[i] = 0;
    }
}
void* setmem(void *addr, char setter, int count){
    unsigned char* destc = (unsigned char*)addr;
    for(int i = 0; i < count; i++){
        *destc = setter;
        destc++;
    }
    return addr;
}
//test functions
int testPrint( void ){
    index = 0;
    vga_framebuffer[0] = 0x41 | 0x0f << 8;
    vga_framebuffer[_RESX] = 0x42 | 0x0f << 8;
    vga_framebuffer[_RESX * 2] = 0x43 | 0x0f << 8;
    vga_framebuffer[_RESX * 3] = 0x44 | 0x0f << 8;
    char tst = 0x41;
    char color = 0x0;
    for(int i = 0; i < _RESX*_RESY; i++){
        if(tst >= 0x7b){
            tst = 0x41;
        }
        if(color == 0xf){
            color = 0x00;
        }
        vga_framebuffer[i] = tst| color << 8;
        tst++;
        color++;
    }
    int dYpos = ypos;
    ypos = index/80;
    if(ypos != dYpos){
        ypos +=1;
    }
    update_cursor(xpos, ypos);
}
int wait(int time){
    for(int i = 0; i < time; i++){
        for(int j = 0; j < 100000; j++){
            asm volatile("nop");
        }
    }
    return 0;
}

float f_pow(int base, int exp){
    float res = 1;
    if(exp < 0){
        float negativeres = base;
        for(int i = exp; i < 0; i++){
            negativeres /= base;
        }
        return negativeres;
    }
    for(int i = 0; i < exp; i++){
        res *= base;
    }
    return res;
}

int i_pow(int base, int exp){
    int res = 1;
    for(int i = 0; i < exp; i++){
        res *= base;
    }
    return res;
}
//int sqrt, float not implemented
// int sqrt(int num){
//     int res = 0;
//     int i = 0;
//     while(res <= num){
//         res = pow(2, i);
//         i++;
//     }
//     return res - 1;
// } //this is honestly awful, doesnt even work. what it does do is return the square ((x ^ 2))?? why does this even exist???
//why did i write that??
int abs(int num){
    if(num < 0){
        return num * -1;
    }
    return num;
}
int memcpy(void *dest, void *src, unsigned int size){
    unsigned char* destc = (unsigned char*)dest;
    unsigned char* srcc = (unsigned char*)src;
    for(int i = 0; i < size; i++){
        *destc = *srcc;
        destc++;
        srcc++;
    }
    return 0;
}
int a20_enable(void){

}

int strlen(char *ptr){
    char *base = ptr;
    int len = 0;
    while(*ptr++) len++;
    return len;
}

void strcpy(char* src, char* dest){
    //this would probably cause a segfault on a linux/windows machine
    while(*src){
        *(dest++) = *(src++);
    }
}
void n_strcpy(char *src, char *dest){
    for(int i = 0; i < strlen(src)+1; i++){
        *(dest + i) = *(src + i);
    }
}
/*********************
* TEXT MODE: 0xB8000 *
* GR.  MODE: 0xA000  *
*********************/

//IMPLEMENT STACK SMASHING PROTECTOR!!!!
char nums[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' 
};
void toHex(int num, char* str){
    char * str1 = malloc(16);
    *(str1++) = 0;
    char *cpy;
    cpy = str;
    while(*cpy){
        *str1++ = nums[num % 16];
        num /=16;
    }
    while(*str1){
        *str++ = *str1--;
    }
}
char strcmp(char *str, char *cmd){
    //match str to command
    //if match, return 1
    //else return 0
    int i = 0;
    while(str[i]){
        if(str[i] == cmd[i]){
            if(cmd[i+1] == 0 && str[i+1] == 0){
                return 1;
            }
        }else{
            return 0;
        }
        i++;
    }
}
char *strtok(char *s, const char d)
{
    // Stores the state of string
    static char* input = null;
 
    // Initialize the input string
    if (s != null)
        input = s;
 
    // Case for final token
    if (input == null)
        return null;
 
    // Stores the extracted string
    char* result = malloc(strlen(input) + 1);
    int i = 0;
 
    // Start extracting string and
    // store it in array
    for (; input[i] != '\0'; i++) {
 
        // If delimiter is not reached
        // then add the current character
        // to result[i]
        if (input[i] != d)
            result[i] = input[i];
 
        // Else store the string formed
        else {
            result[i] = '\0';
            input = input + i + 1;
            return result;
        }
    }
 
    // Case when loop ends
    result[i] = '\0';
    input = null;
 
    // Return the resultant pointer
    // to the string
    return result;
}
char **splitss(char *str){
    //make sure strarr is appropriately sized, strarr splits string based on spaces
    char **strarr = malloc(128 * 8);
    char **base = strarr;
    int arrindex = 0;
    int index = 0;
    char *token = strtok(*str, ' ');
    while(token){
        strcpy(token, strarr[arrindex++]);
        print(token, 0);
        token = strtok(null, ' ');
    }
    return base;
}

void splits(char *str, char **strarr, char* delims){
    return;
}

void kprint(char *data){
    print(data, 0);
}

// void shutdown(){
//     kprint("shutting down...\n");

// }