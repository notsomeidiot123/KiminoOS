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
void drive_test();
int lastDiskAddress = 0;
char *fread(int size, FILE file){
    if(size > file.size){
        size = file.size;
    }else if(size <= 0){
        return FILE_ERROR;
    }
    else if(size > 8192){
        //prevent file data from flooding RAM
        //ik 8kb is small, but i dont want to risk anything
        return BUFFER_TOO_LARGE;
    }
    char fchar = 0;
    //pre-check for control characters
    char *buffer = malloc(size);

    if(fchar == 0){

    }
}
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
void kLBAread(int address, int size, char driveNum, uint16_t *buffer);
void kLBAwrite(int address, char *data, char driveNum, int sec_count);

//0 for hard drive, 1 for SSD
char drive_type = 0;
//0 for CHS, 1 for LBA, 2 for LBA48
char Address_Mode = 0;

void reset_Disk(){
    outb(COMMAND_REGISTER, 0x4);
}
int AHCI_Drive();
int start_disk(void){
    print("Starting Disk...\t", 0);
    int status = inb(STATUS_REGISTER);
    if(status == 0xff){
        print("FAILED\nNo Drive Connected!\n", 0);
    }
    else if(status & 0x80){
        print("Waiting for drive...\t", 0);
        while (inb(STATUS_REGISTER) & 0x80);
        print("Done\n", 0);
    }
    //identify drive
    outb(DRIVE_SELECT, 0xA0);
    outb(SECTOR_COUNT,0);
    outb(SECTOR_NUMBER,0);
    outb(CYLINDER_LOW,0);
    outb(CYLINDER_HIGH,0);
    outb(STATUS_REGISTER, 0xEC);
    if(inb(STATUS_REGISTER) == 0x00){
        print("FAILED\nNo Drive Connected!\n", 0);
    }
    else{
        while(inb(STATUS_REGISTER) & 0x80){
            //wait for drive to be ready
            if(inb(CYLINDER_HIGH) || inb(CYLINDER_LOW)){
                AHCI_Drive();
                break;
            }
            else if(inb(STATUS_REGISTER) & 0x01){
                print("FAILED\nNo Drive Connected!\n", 0);
            }
            else if(inb(STATUS_REGISTER) & 4){
                print("Testing...\t", 0);
            }
        }
    }
    int cylow = inb(CYLINDER_LOW);
    int cyhigh = inb(CYLINDER_HIGH);
    if(cylow == 0x14 && cyhigh == 0xeb || cylow == 0x3c && cyhigh == 0xc3){
        print("Unsupported Drive type\n", 0);
        return 1;
    }
    else if(cylow == 0 && cyhigh == 0 && inb(STATUS_REGISTER) & 1){
        print("Error Reading Disk\n", 0);
        return 1;
    }
    else{
        int status = inb(STATUS_REGISTER);
        if(status & 1 || inb(ERROR_REGISTER)){
            //software reset
            print("Resetting...\t", 0);
            outb(COMMAND_REGISTER, 0x08);
            print("Done\n", 0);
        }
        if(status & 32){
            print("Resetting...\t", 0);
            outb(COMMAND_REGISTER, 0x08);
            print("Done\n", 0);
        }
        outb(DRIVE_SELECT, inb(DRIVE_SELECT) | 0x40);
        if(inb(ERROR_REGISTER)){
            print("Error Reading Disk\n", 0);
            return DRIVE_ERROR;
        }
        print("Disk Started\n",0);
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
            print("LBA48 Detected\n", 0);
        }
        if(data_buffer[60] && data_buffer[61]){
            Address_Mode = 1; //supports LBA addressing mode
        }
        if(inb(ERROR_REGISTER)){
            print("Disk Error\n", 0);
            return DRIVE_ERROR;
        }
    }
    return 0;
}

int AHCI_Drive(){
    print("Currently Not Supported\n", 0);
    print("Please use a non-AHCI drive\n", 0);
    print("R/W to the drive will not work\n", 0);
    print("AHCI drives support will begin in v0.5-alpha\n", 0);
}

void drive_test(registers *regs){
    print("Disk Interrupt\n", 0);
}
//sector mus be < 1

void Drive_Error_Handler();
void kLBAwrite(int address, char *data, char driveNum, int sec_count){
    pollDrive_BSY(driveNum);
    Drive_Error_Handler();
    outb(DRIVE_SELECT, 0xE0 | driveNum <<4 | (address >>24) & 0xF);//output drive data in LBA format, one byte at a time
    outb(ERROR_REGISTER, 0);
    outb(SECTOR_COUNT, sec_count + 1); //how many seectors to read
    outb(SECTOR_NUMBER, (unsigned char) address); 
    outb(CYLINDER_LOW, (unsigned char) (address >>8));
    outb(CYLINDER_HIGH, (unsigned char) (address >>16));
    outb(COMMAND_REGISTER, 0x30);
    for(int i = 0; i < sec_count + 1; i++){
        pollDrive_BSY(driveNum);
        pollDrive_DRQ(driveNum);
        for(int j = 0; j < 256; j++){
            uint16_t toOut = *data++ | (*data++ << 8);
            outw(DATA_REGISTER, toOut);
        }
    }
    int status = inb(STATUS_REGISTER);
    if(status & 1){
        print("Error Reading Disk\n", 0);
    }
    else if(status & 32){
        print("Error Reading Disk\n", 0);
    }
    outb(COMMAND_REGISTER, 0xE7);
    Drive_Error_Handler();
}
void kLBAread(int address, int size, char driveNum, uint16_t *bufferADdr){
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
        print("Error Reading Disk\n", 0);
    }
    else if(status & 32){
        print("Error Reading Disk\n", 0);
    }
}
void Drive_Error_Handler(){
    char err = inb(ERROR_REGISTER);
    if(err & 1){
        print("Address mark not found\n", 0);
    }
    if(err & 2){
        print("Track zero not found\n", 0);
    }
    if(err & 4){
        print("Aborted command\n", 0);
    }
    if(err & 8){
        print("Media change request\n", 0);
    }
    if(err & 16){
        print("ID not found\n", 0);
    }
    if(err & 32){
        print("Uncorrectable data error\n", 0);
    }
    if(err & 64){
        print("Bad Block\n", 0);
    }
    return DRIVE_ERROR;
}