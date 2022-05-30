#include<stdint.h>
#include "../headers/shell.h"
extern const char a20_on;
extern int main( void ){
    
    //320x200 256 color
    //make a graphics library, after I finish with AHCI drives
    clear();
    enable_cursor(0, 15);
    uint16_t *bda = (uint16_t *)0x400;//0x400 is the base address of the BDA
    print("Boot Successful!\nLoading IDT:\t", 0);
    IDT_INIT();
    print("DONE\n", 0);
    if(a20_on){
        print("enabling A20 line...\t", 0);
        a20_enable();
        print("DONE\n", 0);
    }
    print("Starting Keyboard Driver:\t", 0);
    irq_install_handler(1, *keyboard_handler);
    stdin = malloc(512); //beginning amount to allocate to stdin
    print("DONE\nInitializing CLIOSFS\n", 0);
    showOutp = 1;
    print("Please Choose a Disk driver Type by typing the drive type [AHCI, ATA_PIO, SATA]\n", 0);
    shell_init();
}
