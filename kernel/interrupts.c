#include "interrupts.h"
#include "keyboard.h"

// Global variables
static idt_entry_t idt[256];
static idt_ptr_t idt_ptr;

// Initialize interrupts (simplified version)
void interrupts_init(void) {
    // Set up IDT pointer
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt;
    
    // Clear IDT
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0x08, 0x8E);
    }
    
    // Load IDT
    __asm__ volatile("lidt %0" : : "m"(idt_ptr));
    
    // Initialize PIC (simplified)
    pic_init();
    
    // Enable interrupts
    __asm__ volatile("sti");
}

// Set up an IDT gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[num].offset_low = base & 0xFFFF;
    idt[num].offset_high = (base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

// Initialize PIC (simplified)
void pic_init(void) {
    // For now, just enable interrupts without PIC remapping
    // This is a simplified version that should work for basic keyboard input
}

// Send EOI (End of Interrupt) signal
void pic_send_eoi(uint8_t irq) {
    // Simplified EOI - just acknowledge the interrupt
    (void)irq; // Suppress unused parameter warning
}

// Generic interrupt handler (simplified)
void interrupt_handler(interrupt_frame_t* frame) {
    // Handle different interrupt types
    if (frame->int_no < 32) {
        // Exception occurred
        terminal_setcolor(VGA_COLOR(VGA_LIGHT_RED, VGA_BLACK));
        terminal_writestring("Exception: ");
        terminal_print_dec(frame->int_no);
        terminal_putchar('\n');
        
        // For now, just halt the system on exceptions
        __asm__ volatile("cli");
        __asm__ volatile("hlt");
    } else if (frame->int_no >= 32 && frame->int_no < 48) {
        // IRQ occurred
        uint8_t irq = frame->int_no - 32;
        
        // Handle specific IRQs
        switch (irq) {
            case 0:  // Timer
                // Timer interrupt - could be used for scheduling
                break;
            case 1:  // Keyboard
                // Call keyboard handler
                keyboard_handler();
                break;
            default:
                // Other IRQs
                break;
        }
        
        // Send EOI
        pic_send_eoi(irq);
    }
} 