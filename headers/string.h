#include "CLIOS.h"
struct string{
    struct string *last_char;
    char chr;
    struct string *next_char;
};
typedef struct string v_string;


int strmatch(char *str, char *delim){
    int len = strlen(delim);
    int index = 0;
    int offset = 0;
    while(offset < strlen(str)){
        if(str[offset] == delim[index]){
            index++;
            if(index == (len)){
                return offset - index;
            }
        }
        else{
            index = 0;
        }
        offset++;
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
void strcap(char *src, char *dest, int offset){
    dest += offset;
    while(*src){
        *(dest++) = *(src++);
    }
    return;
}
void strcat(char *src, char *dest){
    
}