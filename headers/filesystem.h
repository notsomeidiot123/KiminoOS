#include "diskdriver.h"
typedef struct {
    char *filename;
    int ptr;
    uint32_t startAddress;
    uint32_t endAddress;
} FILE;

void exec_file(FILE *file);

int install_fs(){
    char end_found= 0;
    int start_search = 0x47200;
    while(!end_found){
        
    }
}

FILE *fs_search(char *filename){

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