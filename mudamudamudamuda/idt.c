#include "headers/idt.h"
#include <stdint.h>
#define FLAGSET(number, flag) number |= flag
#define FLAGUNSET(number, flag) number &= -flag
typedef struct {
    uint16_t BaseLow;
    uint16_t SegmentSelector;
    uint8_t Reserved;
    uint8_t Flags;
    uint16_t BaseHigh;
} __attribute__((packed)) IDTEntry;
typedef struct {
    uint16_t Limit;
    IDTEntry *Ptr;
}__attribute__((packed)) IDTDescriptor;
typedef enum {
    IDT_FLAG_GATE_TASK          = 0x5,
    IDT_FLAG_GATE_16BIT_INT     = 0x6,
    IDT_FLAG_GATE_16BIT_TRAP    = 0x7,
    IDT_FLAG_GATE_32BIT_INT     = 0xE,
    IDT_FLAG_GATE_32BIT_TRAP    = 0xF,

    IDT_FLAG_RING0              = (0 << 5),
    IDT_FLAG_RING1              = (1 << 5),
    IDT_FLAG_RING2              = (2 << 5),
    IDT_FLAG_RING3              = (3 << 5),

    IDT_FLAG_PRESENT            = 0x80,
} IDT_FLAGS;
IDTEntry g_IDT[256];
IDTDescriptor g_IDTDesc = {sizeof(g_IDT -1), g_IDT};
void __attribute__ ((cdecl)) IDT_LOAD(IDTDescriptor *idtdesc);
void SetGateIDT(int interrupt, void* base, uint16_t segmentDesc, uint8_t flags){
    g_IDT[interrupt].BaseLow = ((uint32_t)base) & 0xffff;
    g_IDT[interrupt].SegmentSelector = segmentDesc;
    g_IDT[interrupt].Reserved = 0;
    g_IDT[interrupt].Flags = flags;
    g_IDT[interrupt].BaseHigh = ((uint32_t)base >> 16) & 0xffff;
}
void enableIDTGate(int interrput){
    FLAGSET(g_IDT[interrput].Flags, IDT_FLAG_PRESENT);
}
void disableIDTGate(int interrupt){
    FLAGUNSET(g_IDT[interrupt].Flags, IDT_FLAG_PRESENT);
}
void IDT_INIT(){
    IDT_LOAD(&g_IDTDesc);
}