#include "floppy.h"
enum disk_registers{
    PRIMARY_BASE = 0x1F0,
    PRIMARY_CTRL = 0x3F6,
    SECONDARY_BASE = 0x170,
    SECONDARY_CTRL = 0x376,
    DATA_REGISTER = 0x1f0,
    ERROR_REGISTER = 0x1f1,
    SECTOR_COUNT = 0x1f2,
    SECTOR_NUMBER = 0x1f3,
    CYLINDER_LOW = 0x1f4,
    CYLINDER_HIGH = 0x1f5,
    DRIVE_SELECT = 0x1f6,
    STATUS_REGISTER = 0x1f7,
    COMMAND_REGISTER = 0x1f7,
    //all primary
    ALTERNATE_STATUS = 0x3f6,
    DEVICE_CONTROL = 0x3f6,
    DRIVE_ADDRESS = 0x3f7
};
typedef struct file{
    char *name;
    int size;
    int *address;
    int *caddress;
} FILE;
int disk_mode = 0;
void drive_test();
int lastDiskAddress = 0;
// char *fread(int size, FILE *file){
//     if(size > file.size){
//         size = file.size;
//     }else if(size <= 0){
//         return FILE_ERROR;
//     }
//     else if(size > 8192){
//         //prevent file data from flooding RAM
//         //ik 8kb is small, but i dont want to risk anything
//         return BUFFER_TOO_LARGE;
//     }
//     char fchar = 0;
//     //pre-check for control characters
//     char *buffer = malloc(size);

//     if(fchar == 0){

//     }
// }
char *fwrite(char *data, FILE file){

}
FILE *fopen(char *name){

}
void fclose(FILE *file){

}
void pollDrive_BSY(char drive){
    while(inb(STATUS_REGISTER) & 0x80);
}
void pollDrive_DRQ(char drive){
    while(!(inb(STATUS_REGISTER) & 0x40));
}
char kLBAread(int address, int size, char driveNum, uint16_t *buffer);
char kLBAwrite(int address, char *data, char driveNum, int sec_count);

//0 for hard drive, 1 for SSD
char drive_type = 0;
//0 for CHS, 1 for LBA, 2 for LBA48
char Address_Mode = 0;

void reset_Disk(){
    outb(COMMAND_REGISTER, 0x4);
}
int AHCI_Drive();
int start_disk(void){
    int status = inb(STATUS_REGISTER);
    if(status == 0xff){
    }
    else if(status & 0x80){
        while (inb(STATUS_REGISTER) & 0x80);
    }
    //identify drive
    outb(DRIVE_SELECT, 0xA0);
    outb(SECTOR_COUNT,0);
    outb(SECTOR_NUMBER,0);
    outb(CYLINDER_LOW,0);
    outb(CYLINDER_HIGH,0);
    outb(STATUS_REGISTER, 0xEC);
    if(inb(STATUS_REGISTER) == 0x00){
        return DRIVE_ERROR;
    }
    else{
        while(inb(STATUS_REGISTER) & 0x80){
            //wait for drive to be ready
            if(inb(CYLINDER_HIGH) || inb(CYLINDER_LOW)){
                AHCI_Drive();
                break;
            }
            else if(inb(STATUS_REGISTER) & 0x01){
                break;
            }
            else if(inb(STATUS_REGISTER) & 4){
                continue;
            }
        }
    }
    int cylow = inb(CYLINDER_LOW);
    int cyhigh = inb(CYLINDER_HIGH);
    if(cylow == 0x14 && cyhigh == 0xeb || cylow == 0x3c && cyhigh == 0xc3){
        return 1;
    }
    else if(cylow == 0 && cyhigh == 0 && inb(STATUS_REGISTER) & 1){
        return 1;
    }
    else{
        int status = inb(STATUS_REGISTER);
        if(status & 1 || inb(ERROR_REGISTER)){
            //software reset
            outb(COMMAND_REGISTER, 0x08);
        }
        if(status & 32){
            outb(COMMAND_REGISTER, 0x08);
        }
        outb(DRIVE_SELECT, inb(DRIVE_SELECT) | 0x40);
        if(inb(ERROR_REGISTER)){
            return DRIVE_ERROR;
        }
        uint16_t *data_buffer = (uint16_t*)malloc(sizeof(uint16_t) * 256);
        uint16_t *start = data_buffer;
        for(int i = 0; i < 256; i++){
            data_buffer[i] = inw(DATA_REGISTER);
        }
        if(data_buffer[0]){
            drive_type = 1;
        }
        if(data_buffer[83] & 0x200){
            Address_Mode = 2;
        }
        if(data_buffer[60] && data_buffer[61]){
            Address_Mode = 1; //supports LBA addressing mode
        }
        if(inb(ERROR_REGISTER)){
            return DRIVE_ERROR;
        }
    }
    return 0;
}

int AHCI_Drive(){
}

void drive_test(registers *regs){
}
//sector mus be < 1
//I MIGHT HAVE TO RE-WRITE LIKE 300 LINES OF CODE
//BECAUSE IM DUMB AND CHOSE ATA OVER AHCI
//AHCI IS SO MUCH BETTERRRR
void Drive_Error_Handler();
char kLBAwrite(int address, char *data, char driveNum, int sec_count){
    pollDrive_BSY(driveNum);
    Drive_Error_Handler();
    outb(DRIVE_SELECT, 0xE0 | driveNum <<4 | (address >>24) & 0xF);//output drive data in LBA format, one byte at a time
    outb(ERROR_REGISTER, 0);
    outb(SECTOR_COUNT, sec_count + 1); //how many seectors to read
    outb(SECTOR_NUMBER, (unsigned char) address); 
    outb(CYLINDER_LOW, (unsigned char) (address >>8));
    outb(CYLINDER_HIGH, (unsigned char) (address >>16));
    outb(COMMAND_REGISTER, 0x30);
    char *toWrite = malloc(512 * sec_count);
    if(strlen(data) % 512 > 0){
        kLBAread(address, 512 * sec_count, driveNum, toWrite);
        strcpy(data, toWrite);
    }
    for(int i = 0; i < sec_count + 1; i++){
        pollDrive_BSY(driveNum);
        pollDrive_DRQ(driveNum);
        for(int j = 0; j < 256; j++){
            uint16_t toOut = *toWrite++ | (*toWrite << 8);
            outw(DATA_REGISTER, toOut);
            data++;
        }
    }
    int status = inb(STATUS_REGISTER);
    if(status & 1){
        return DRIVE_ERROR;
    }
    else if(status & 32){
        return DRIVE_ERROR;
    }
    outb(COMMAND_REGISTER, 0xE7); // clear buffer
    Drive_Error_Handler();
}
char kLBAread(int address, int size, char driveNum, uint16_t *bufferADdr){
    //check status
    pollDrive_BSY(driveNum);
    //read data
    outb(DRIVE_SELECT, 0xE0 | driveNum <<4 | (address >>24) & 0xF);//output drive data in LBA format, one byte at a time
    outb(SECTOR_COUNT, size); //how many seectors to read
    outb(SECTOR_NUMBER, (unsigned char) address); 
    outb(CYLINDER_LOW, (unsigned char) (address >>8));
    outb(CYLINDER_HIGH, (unsigned char) (address >>16));
    outb(COMMAND_REGISTER, 0x20);
    uint16_t *buffer = bufferADdr;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < 256; j++){
            buffer[j] = inw(DATA_REGISTER);
        }
        buffer += 256;
    }
    outb(COMMAND_REGISTER, 0xE7);
    Drive_Error_Handler();
    int status = inb(STATUS_REGISTER);
    if(status & 1){
        return DRIVE_ERROR;
    }
    else if(status & 32){
        return DRIVE_ERROR;
    }
}
void Drive_Error_Handler(){
    return DRIVE_ERROR;
}
uint16_t *readDisk(int address, int secnum, char drivenum){
    uint16_t *buffer = malloc(sizeof(uint16_t) * 256* secnum);
    switch(disk_mode){
        case 0:
            kLBAread(address, secnum, drivenum, buffer);
            break;
        case 1:
            //AHCI_read
            break;
    }
    return buffer;
}