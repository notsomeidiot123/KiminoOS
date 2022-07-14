#include "../drivers/filesystem.h"
#include "../string.h"
#include "../CLIOS.h"

#pragma once
char showOutp = 0;
char started = 0;
int shell(char* args){
    
    return 0;
}
int shell_run(char *args){
    //refactor to allow for passing of arguments
    if(!showOutp) return -1;
    else{
        int argc = 0;
        char * command = strtok(args, ' ');
        if(strcmp(command, "syshelp")){
            print("Commands:\n\tsyshelp: Show this information\n", 0);
            print("\tclear: clear the screen\n", 0);
            // kprint("\texit: shutdown the system\n");
            kprint("\twrt: write text to stdout | THIS IS FOR DEBUG PURPOSES\n");
        }else if(strcmp(command, "clear")){
            clear();
        }
        else if(strcmp(command, "testscreen")){
            testPrint();
        }
        else if(strcmp(command, "exit")){
            // shutdown();
        }
        else if(strcmp(command, "wrt")){
            int arrlen = 0;
            char *s = strtok(args, ' ');
            while(s){
                arrlen++;
                s = strtok(null, ' ');
                
            }
            char *text = strtok(args, ' ');
            int count = 1;
            while(text){
                text = strtok(null, ' ');
                if(text == null){
                    break;
                }
                print(text, 0);
                if(++count < arrlen){
                    kprint(" ");
                }
            }
        }
        else if(strcmp(command, "drvinf")){
            kprint(boot_drive.drive_name);
            kprint("\n\tMaximum Address:\t");
            printdc(boot_drive.lba_max_address);
            kprint("\n\tSector Size:    \t");
            int s = i_pow(2, boot_drive.lba_sector_size);
            if(s == 0){
                 s = 512;
            }
            printdc(s);
            kprint("\n\tDrive Size:     \t");
            printdc(((boot_drive.lba_max_address * s)/1024)/1024);
            kprint("MB\n");
        }
        print("$>", 0);
    }
}
extern void reset();
int shell_init(void){
    print("KiminoOS v0.1.0-alpha\n", 0);
    print("Type 'syshelp' for a list of commands\n", 0);
    print("Type 'exit' to exit the shell\n", 0);
    print("Unstable release, use at your own risk\n", 0);
    print("Notsomeidiot123 @ github, 5/7/22\n", 0);
    print("$>", 0);
}
