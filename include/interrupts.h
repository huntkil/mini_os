#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "terminal.h"

// Interrupt Descriptor Table (IDT) entry structure
typedef struct {
    uint16_t offset_low;    // Lower 16 bits of offset
    uint16_t selector;      // Code segment selector
    uint8_t zero;           // Always 0
    uint8_t flags;          // Type and attributes
    uint16_t offset_high;   // Upper 16 bits of offset
} __attribute__((packed)) idt_entry_t;

// IDT pointer structure for lidt instruction
typedef struct {
    uint16_t limit;         // Size of IDT - 1
    uint32_t base;          // Base address of IDT
} __attribute__((packed)) idt_ptr_t;

// Interrupt frame structure (what gets pushed on stack)
typedef struct {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t int_no;
    uint32_t err_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t user_esp;
    uint32_t ss;
} __attribute__((packed)) interrupt_frame_t;

// PIC ports
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

// PIC initialization
#define ICW1_ICW4    0x01    // ICW4 needed
#define ICW1_SINGLE  0x02    // Single (cascade) mode
#define ICW1_INTERVAL4 0x04  // Call address interval 4 (8)
#define ICW1_LEVEL   0x08    // Level triggered (edge) mode
#define ICW1_INIT    0x10    // Initialization required

#define ICW4_8086    0x01    // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO    0x02    // Auto (normal) EOI
#define ICW4_BUF_SLAVE 0x08  // Buffered mode/slave
#define ICW4_BUF_MASTER 0x0C // Buffered mode/master
#define ICW4_SFNM     0x10   // Special fully nested (not)

// Interrupt numbers
#define IRQ0 32  // Timer
#define IRQ1 33  // Keyboard
#define IRQ2 34  // Cascade
#define IRQ3 35  // COM2
#define IRQ4 36  // COM1
#define IRQ5 37  // LPT2
#define IRQ6 38  // Floppy
#define IRQ7 39  // LPT1
#define IRQ8 40  // CMOS Real-time clock
#define IRQ9 41  // Free for peripherals
#define IRQ10 42 // Free for peripherals
#define IRQ11 43 // Free for peripherals
#define IRQ12 44 // PS2 Mouse
#define IRQ13 45 // FPU
#define IRQ14 46 // Primary ATA
#define IRQ15 47 // Secondary ATA

// Function declarations
void interrupts_init(void);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);
void pic_init(void);
void pic_send_eoi(uint8_t irq);
void interrupt_handler(interrupt_frame_t* frame);

// Interrupt handler declarations
void isr0(void);   // Division by zero
void isr1(void);   // Debug
void isr2(void);   // Non-maskable interrupt
void isr3(void);   // Breakpoint
void isr4(void);   // Overflow
void isr5(void);   // Bound range exceeded
void isr6(void);   // Invalid opcode
void isr7(void);   // Device not available
void isr8(void);   // Double fault
void isr9(void);   // Coprocessor segment overrun
void isr10(void);  // Invalid TSS
void isr11(void);  // Segment not present
void isr12(void);  // Stack segment fault
void isr13(void);  // General protection fault
void isr14(void);  // Page fault
void isr15(void);  // Reserved
void isr16(void);  // x87 FPU error
void isr17(void);  // Alignment check
void isr18(void);  // Machine check
void isr19(void);  // SIMD FPU error
void isr20(void);  // Reserved
void isr21(void);  // Reserved
void isr22(void);  // Reserved
void isr23(void);  // Reserved
void isr24(void);  // Reserved
void isr25(void);  // Reserved
void isr26(void);  // Reserved
void isr27(void);  // Reserved
void isr28(void);  // Reserved
void isr29(void);  // Reserved
void isr30(void);  // Reserved
void isr31(void);  // Reserved

// IRQ handlers
void irq0(void);   // Timer
void irq1(void);   // Keyboard
void irq2(void);   // Cascade
void irq3(void);   // COM2
void irq4(void);   // COM1
void irq5(void);   // LPT2
void irq6(void);   // Floppy
void irq7(void);   // LPT1
void irq8(void);   // CMOS Real-time clock
void irq9(void);   // Free for peripherals
void irq10(void);  // Free for peripherals
void irq11(void);  // Free for peripherals
void irq12(void);  // PS2 Mouse
void irq13(void);  // FPU
void irq14(void);  // Primary ATA
void irq15(void);  // Secondary ATA

#endif // INTERRUPTS_H 