#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    printf("%s", argv[1]);
    FILE *f = fopen("bin/fsheader.txt", "w");
    fprintf(f,"KIMINOFS\nKERNELSIZE:\n");
    fclose(f);
    return 0;
}