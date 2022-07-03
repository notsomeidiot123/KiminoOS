#include "diskdriver.h"
char started = 0;
char parse(char* str, char* cmd);
int shell(char* args){
    
    return 0;
}
int shell_run(char *args){
    int argc = 0;
    char *argv[256];
    //requires else to work
    if(parse(args, "test")){
        print("Test1", 0);
    }else if(parse(args, "clear")){
        clear();
    }
    else if(parse(args, "testscreen")){
        testPrint();
    }
    print("$>", 0);
}
extern void reset();
int shell_init(void){
    stdin = malloc(512); //beginning amount to allocate to stdin
    print("CLIOS v0.1.0-alpha\n", 0);
    print("Type 'help' for a list of commands\n", 0);
    print("Type 'exit' to exit the shell\n", 0);
    print("Unstable release, use at your own risk\n", 0);
    print("Notsomeidiot123 @ github, 12/4/22\n", 0);
    print("$>", 0);
}
char parse(char *str, char *cmd){
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
