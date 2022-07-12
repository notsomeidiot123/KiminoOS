#include <stdint.h>
#include "CLIOS.h"

#define FLAGSET(number, flag) number |= flag
#define FLAGUNSET(number, flag) number &= -flag
typedef struct
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
} registers;
registers regs;
extern void isr_stub();
extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();

typedef struct {
    unsigned short BaseLow;
    unsigned short SegmentSelector;
    unsigned char Reserved;
    unsigned char Flags;
    unsigned short BaseHigh;
} __attribute__((packed)) IDTEntry;
typedef struct {
    unsigned short Limit;
    unsigned int base;
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
IDTDescriptor g_IDTDesc;
void __attribute__ ((cdecl)) IDT_LOAD(IDTDescriptor *idtdesc);
void SetGateIDT(uint8_t interrupt, unsigned long base, uint16_t segmentDesc, uint8_t flags){
    g_IDT[interrupt].BaseLow = (base & 0xffff);
    g_IDT[interrupt].SegmentSelector = segmentDesc;
    g_IDT[interrupt].Reserved = 0;
    g_IDT[interrupt].Flags = flags;
    g_IDT[interrupt].BaseHigh = (base >> 16) & 0xffff;
}
void enableIDTGate(int interrput){
    FLAGSET(g_IDT[interrput].Flags, IDT_FLAG_PRESENT);
}
void disableIDTGate(int interrupt){
    FLAGUNSET(g_IDT[interrupt].Flags, IDT_FLAG_PRESENT);
}
void irq_remap(void)
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}
void *irq_routines[16] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};
char currentInterrupt;
extern void _irq_handler(registers *r)
{
	currentInterrupt = r -> int_no;
	void (*handler)(registers *r);

	
	handler = (void (*)(registers*))irq_routines[r->int_no - 32];
	if (handler)
	{
		handler(r);
	}


	if (r->int_no >= 40)
	{
		outb(0xA0, 0x20);
	}

	outb(0x20, 0x20);
}

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

const char* exception_messages[] = 
{
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

void irq_install_handler(int irq, void (*handler)(registers *r))
{
	irq_routines[irq] = (void*)handler;
}

extern void _fhandler(registers *r)
{
    
    if (r->int_no < 32)
    {
        print(exception_messages[r->int_no], 0);
        print(" Exception. System Halted!\n",  0);
        if(r->int_no == 2){
            int system_port_a = 0x92;
            int system_port_b = 0x61;
            char sysctrla = inb(system_port_a);
            char sysctrlb = inb(system_port_b);
            if(sysctrla & 16){
                kprint("Watchdog Timer\n");
            }
            if(sysctrlb & 0x40){
                kprint("Channel Check, Bus error\n");
            }
            if(sysctrlb & 0x80){
                kprint("Parity Check\n");
            }
        }
        for(;;);
    }
}

void IDT_INIT(){
    //install isrs
    g_IDTDesc.Limit = (sizeof (IDTEntry) * 256) - 1;
	g_IDTDesc.base = (unsigned int)&g_IDT;
    setmem(&g_IDT, 0, sizeof(IDTDescriptor) * 256);
    SetGateIDT(0, (unsigned)_isr0, 0x08, 0x8e);
    SetGateIDT(1, (unsigned)_isr1, 0x08, 0x8e);
    SetGateIDT(2, (unsigned)_isr2, 0x08, 0x8e);
    SetGateIDT(3, (unsigned)_isr3, 0x08, 0x8e);
    SetGateIDT(4, (unsigned)_isr4, 0x08, 0x8e);
    SetGateIDT(5, (unsigned)_isr5, 0x08, 0x8e);
    SetGateIDT(6, (unsigned)_isr6, 0x08, 0x8e);
    SetGateIDT(7, (unsigned)_isr7, 0x08, 0x8e);
    SetGateIDT(8, (unsigned)_isr8, 0x08, 0x8e);
    SetGateIDT(9, (unsigned)_isr9, 0x08, 0x8e);
    SetGateIDT(10, (unsigned)_isr10, 0x08, 0x8e);
    SetGateIDT(11, (unsigned)_isr11, 0x08, 0x8e);
    SetGateIDT(12, (unsigned)_isr12, 0x08, 0x8e);
    SetGateIDT(13, (unsigned)_isr13, 0x08, 0x8e);
    SetGateIDT(14, (unsigned)_isr14, 0x08, 0x8e);
    SetGateIDT(15, (unsigned)_isr15, 0x08, 0x8e);
    SetGateIDT(16, (unsigned)_isr16, 0x08, 0x8e);
    SetGateIDT(17, (unsigned)_isr17, 0x08, 0x8e);
    SetGateIDT(18, (unsigned)_isr18, 0x08, 0x8e);
    SetGateIDT(19, (unsigned)_isr19, 0x08, 0x8e);
    SetGateIDT(20, (unsigned)_isr20, 0x08, 0x8e);
    SetGateIDT(21, (unsigned)_isr21, 0x08, 0x8e);
    SetGateIDT(22, (unsigned)_isr22, 0x08, 0x8e);
    SetGateIDT(23, (unsigned)_isr23, 0x08, 0x8e);
    SetGateIDT(24, (unsigned)_isr24, 0x08, 0x8e);
    SetGateIDT(25, (unsigned)_isr25, 0x08, 0x8e);
    SetGateIDT(26, (unsigned)_isr26, 0x08, 0x8e);
    SetGateIDT(27, (unsigned)_isr27, 0x08, 0x8e);
    SetGateIDT(28, (unsigned)_isr28, 0x08, 0x8e);
    SetGateIDT(29, (unsigned)_isr29, 0x08, 0x8e);
    SetGateIDT(30, (unsigned)_isr30, 0x08, 0x8e);
    SetGateIDT(31, (unsigned)_isr31, 0x08, 0x8e);
    
    irq_remap();
    SetGateIDT(32, (unsigned)irq0, 0x08, 0x8E);
	SetGateIDT(33, (unsigned)irq1, 0x08, 0x8E);
	SetGateIDT(34, (unsigned)irq2, 0x08, 0x8E);
	SetGateIDT(35, (unsigned)irq3, 0x08, 0x8E);
	SetGateIDT(36, (unsigned)irq4, 0x08, 0x8E);
	SetGateIDT(37, (unsigned)irq5, 0x08, 0x8E);
	SetGateIDT(38, (unsigned)irq6, 0x08, 0x8E);
	SetGateIDT(39, (unsigned)irq7, 0x08, 0x8E);
	SetGateIDT(40, (unsigned)irq8, 0x08, 0x8E);
	SetGateIDT(41, (unsigned)irq9, 0x08, 0x8E);
	SetGateIDT(42, (unsigned)irq10, 0x08, 0x8E);
	SetGateIDT(43, (unsigned)irq11, 0x08, 0x8E);
	SetGateIDT(44, (unsigned)irq12, 0x08, 0x8E);
	SetGateIDT(45, (unsigned)irq13, 0x08, 0x8E);
	SetGateIDT(46, (unsigned)irq14, 0x08, 0x8E);
	SetGateIDT(47, (unsigned)irq15, 0x08, 0x8E);
    outb(0x21, 0xfd);
    outb(0xa1, 0xff);
    asm("sti");
    //irqs
    //SetGateIDT
	
    
    IDT_LOAD(&g_IDTDesc);
}
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}