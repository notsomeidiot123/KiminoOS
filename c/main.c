#include<stdint.h>
#include "../headers/shell.h"
extern int main( void ){
    clear();
    enable_cursor(0, 15);
    uint16_t *bda = (uint16_t *)0x400;//0x400 is the base address of the BDA
    print("Boot Successful!\nLoading IDT:\t", 0);
    IDT_INIT();
    print("DONE\n", 0);
    print("Starting Keyboard Driver:\t", 0);
    irq_install_handler(1, *keyboard_handler);
    stdin = malloc(512); //beginning amount to allocate to stdin
    print("DONE\nInitializing CLIOSFS\n", 0);
    start_disk();
    
    uint16_t *d;
    kLBAread(0, 1, 0, d);
    
    if(*d != 1770){
        while(*d != 1770){
            reset_Disk();
            kLBAread(0, 1, 0, d);
        }
    }
    shell_init();
    //print("Starting Shell...\n", 0);
    update_cursor(xpos, ypos);
    showOutp =1;
    past = 1;
    kLBAwrite(0, "Hello World!\n", 0, 1);
    uint16_t *b;
    kLBAread(0, 1, 0, b);
    if(!parse(b, "Hello World!")){
        print("FAILED\n", 0);
    }
    else{
        print("PASSED\n", 0);
    }
}