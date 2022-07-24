#include "../idt.h"
#include "../shell/shell.h"
#include "../CLIOS.h"
#pragma once

const char *scancodes[128] = {
    0, "\e","1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    "-", "=", "\b", "\t", "q", "w", "e", "r", "t", "y",
    "u", "i", "o", "p", "[", "]", (char)0x1c, 0x1d, "a", "s",
    "d", "f", "g", "h", "j", "k", "l", ";", "'", "`", 0x2a,
    "\\", "z", "x", "c","v","b", "n", "m", ",", ".", "/",
     0x36, "*", 0x38, " ", 0x3a
};
unsigned char stdincodes[128] = {
    0, '\e','1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y',
    'u', 'i', 'o', 'p', '[', ']', 0x1c, 0x1d, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0x2a,
    '\\', 'z', 'x', 'c','v','b', 'n', 'm', ',', '.', '/',
     0x36, '*', 0x38, ' ', 0x3a
};
//\e=escape, 0x38 = l alt , 0x3a = capslock
//\b=backspace, 0x36 = right shift
//\t = tab, 0x2a = left shift
//0x1c = enter, 0x1d=lctrl
char shiftpressed = 0;
int curposy = 3;
int curposx = 0;

int stdindex = 0;
char *stdin = 0; //beginning amount to allocate to stdin;
char run = 0;
char ctrlpressed = 0;
void keyboard_handler(registers *r){
    unsigned char scancode = inb(0x60);
    if(scancode > 0x80){
        switch(scancode){
            case 0xaa:
                shiftpressed = 0;
                break;
            case 0xb6:
                shiftpressed = 0;
                break;
            case 0x9d:
                ctrlpressed = 0;
                break;
        }
    }else{
        switch(scancode){
            case 16:
                for(int i = 0; i < 4; i++){
                    stdin[stdindex++] = ' ';
                }
            case 0x38:
                break;
            case 0x1d:
                ctrlpressed = 1;
                break;
            case 0x36:
                shiftpressed = 1;
                break;
            case 0x2a:
                shiftpressed = 1;
                break;
            case 0x3a:
                shiftpressed = !shiftpressed;
                break;
            case 0x1c:
                stdin[stdindex] = '\0';
                if(showOutp = 1) {
                    print("\n", 0);
                    shell_run(stdin);
                    while(stdindex){
                        stdin[stdindex] = '\0';
                        stdindex--;
                    }
                }
                
                
                break;
            case 0xe:
                if(stdindex > 0){
                    if(ctrlpressed){
                        while(stdin[stdindex] != ' ' || stdin[stdindex] != 0 || stdindex != 0 || stdin[stdindex] == '\t'){
                            stdin[stdindex--] = 0;
                            delkey();
                            if(stdin[stdindex] == ' ' || stdin[stdindex] == 0 || stdindex == 0 || stdin[stdindex] == '\t') break;
                        }
                    }
                    else{
                        int swp = stdin[--stdindex];
                        if(swp == '\t'){
                            if(stdindex == 1){
                                for(int i = 0; i < 4; i++);
                                delkey();
                            }
                            for(int i = 4; i > 0 ; i--){
                                delkey();
                            }
                            delkey();
                        }
                        else {
                            delkey();
                        }
                        stdin[stdindex] = 0;
                    }
                }
                break;
            default:
             if(showOutp){
                char tstdin = stdincodes[scancode];
                if(shiftpressed){
                    
                    switch(tstdin){
                        case '\'':
                            stdin[stdindex] = '\"';
                            break;
                        case ';':
                            stdin[stdindex] = ':';
                            break;
                        case '[':
                            stdin[stdindex] = '{';
                            break;
                        case ']':
                            stdin[stdindex] = '}';
                            break;
                        case ',':
                            stdin[stdindex] = '<';
                            break;
                        case '.':
                            stdin[stdindex] = '>';
                            break;
                        case '/':
                            stdin[stdindex] = '?';
                            break;
                        case '\\':
                            stdin[stdindex] = '|';
                            break;
                        case '-':
                            stdin[stdindex] = '_';
                            break;
                        case '=':
                            stdin[stdindex] = '+';
                            break;
                        case '`':
                            stdin[stdindex] = '~';
                            break;
                        case '1':
                            stdin[stdindex] = '!';
                            break;
                        case '2':
                            stdin[stdindex] = '@';
                            break;
                        case '3':
                            stdin[stdindex] = '#';
                            break;
                        case '4':
                            stdin[stdindex] = '$';
                            break;
                        case '5':
                            stdin[stdindex] = '%';
                            break;
                        case '6':
                            stdin[stdindex] = '^';
                            break;
                        case '7':
                            stdin[stdindex] = '&';
                            break;
                        case '8':
                            stdin[stdindex] = '*';
                            break;
                        case '9':
                            stdin[stdindex] = '(';
                            break;
                        case '0':
                            stdin[stdindex] = ')';
                            break;
                        default:
                            stdin[stdindex] = stdincodes[scancode] - 32;
                            break;
                    }       
                }
                else{
                    stdin[stdindex] = stdincodes[scancode];
                }
                print(scancodes[scancode], shiftpressed);
                stdindex++;
                break;
                
            }
        }
    }
    return;
}