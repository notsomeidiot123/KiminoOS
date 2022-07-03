#include "shell.h"
typedef struct {
    char *filename;
    int ptr;
    uint32_t startAddress;
    uint32_t endAddress;
} FILE;