#include<stdint.h>
#include "../headers/drivers/filesystem.h"
#include "../headers/drivers/keyboard.h"
#include "../headers/shell/shell.h"
#include "../headers/CLIOS.h"
#include "../headers/idt.h"
extern const char a20_on;
const int *memory = 0x7e00;


extern int main( void ){
    clear();
    enable_cursor(0, 15);
    uint16_t *bda = (uint16_t *)0x400;//0x400 is the base address of the BDA
    print("Boot Successful!\nLoading IDT:\t", 0);
    IDT_INIT();
    print("DONE\n", 0);
    // uint64_t base = *(memory++) << 32 | *(memory++);
    // uint64_t length = *(memory++) << 32 | *(memory++);
    // uint32_t type = *(memory++);
    if(a20_on){ 
        print("enabling A20 line...\t", 0);
        a20_enable();
        print("DONE\n", 0);
    }
    print("Starting Keyboard Driver:\t", 0);
    irq_install_handler(1, *keyboard_handler);
    stdin = malloc(512); //beginning amount to allocate to stdin
    print("DONE\n", 0);
    print("Starting Disk...\t", 0);
    showOutp = 1;
    char *e = malloc(512);
    kLBAread(0, 1, 0, e);
    shell_init();
}
