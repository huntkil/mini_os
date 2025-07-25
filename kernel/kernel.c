#include "kernel.h"
#include "terminal.h"

// Main kernel entry point
void kernel_main(void) {
    // Initialize terminal
    terminal_initialize();
    
    // Welcome message with enhanced colors
    terminal_setcolor(VGA_COLOR(VGA_LIGHT_GREEN, VGA_BLACK));
    terminal_println("Welcome to Mini OS!");
    terminal_println("Kernel loaded successfully.");
    terminal_println("System initialized.");
    terminal_putchar('\n');
    
    // System information with different colors
    terminal_setcolor(VGA_COLOR(VGA_WHITE, VGA_BLACK));
    terminal_println("Mini OS v1.0 - Enhanced Terminal System");
    terminal_println("Phase 2: Enhanced Output System");
    terminal_putchar('\n');
    
    // Demonstrate color management
    terminal_setcolor(VGA_COLOR(VGA_LIGHT_CYAN, VGA_BLACK));
    terminal_println("=== Color Demonstration ===");
    
    terminal_setcolor(VGA_COLOR(VGA_LIGHT_RED, VGA_BLACK));
    terminal_writestring("Red text ");
    
    terminal_setcolor(VGA_COLOR(VGA_LIGHT_GREEN, VGA_BLACK));
    terminal_writestring("Green text ");
    
    terminal_setcolor(VGA_COLOR(VGA_LIGHT_BLUE, VGA_BLACK));
    terminal_writestring("Blue text ");
    
    terminal_setcolor(VGA_COLOR(VGA_LIGHT_MAGENTA, VGA_BLACK));
    terminal_writestring("Magenta text");
    terminal_putchar('\n');
    
    // Demonstrate cursor positioning
    terminal_setcolor(VGA_COLOR(VGA_WHITE, VGA_BLACK));
    terminal_println("=== Cursor Positioning ===");
    
    // Save current cursor position
    terminal_save_cursor();
    
    // Write text at specific positions
    terminal_writestring_at("Top-left corner", 0, 0);
    terminal_writestring_at("Bottom-right", 60, 20);
    terminal_writestring_at("Center message", 30, 12);
    
    // Restore cursor position
    terminal_restore_cursor();
    terminal_putchar('\n');
    
    // Demonstrate number formatting
    terminal_setcolor(VGA_COLOR(VGA_LIGHT_YELLOW, VGA_BLACK));
    terminal_println("=== Number Formatting ===");
    
    uint32_t test_value = 0xDEADBEEF;
    
    terminal_writestring("Decimal: ");
    terminal_print_dec(test_value);
    terminal_putchar('\n');
    
    terminal_writestring("Hexadecimal: ");
    terminal_print_hex(test_value);
    terminal_putchar('\n');
    
    terminal_writestring("Binary: ");
    terminal_print_bin(0x0F);
    terminal_putchar('\n');
    
    // Demonstrate screen drawing
    terminal_setcolor(VGA_COLOR(VGA_LIGHT_GREY, VGA_BLACK));
    terminal_println("=== Screen Drawing ===");
    
    // Draw a box
    terminal_draw_box(5, 15, 25, 18, '#', VGA_COLOR(VGA_LIGHT_BLUE, VGA_BLACK));
    terminal_writestring_at("Drawn Box", 8, 16);
    
    // Draw horizontal line
    terminal_draw_line_horizontal(30, 15, 20, '-', VGA_COLOR(VGA_LIGHT_GREEN, VGA_BLACK));
    
    // Draw vertical line
    terminal_draw_line_vertical(50, 15, 4, '|', VGA_COLOR(VGA_LIGHT_RED, VGA_BLACK));
    
    // Move cursor to bottom for status
    terminal_set_cursor(0, 23);
    terminal_setcolor(VGA_COLOR(VGA_LIGHT_GREY, VGA_BLACK));
    terminal_println("=== System Status ===");
    
    // Show cursor position
    uint16_t cursor_x, cursor_y;
    terminal_get_cursor(&cursor_x, &cursor_y);
    terminal_writestring("Cursor position: (");
    terminal_print_dec(cursor_x);
    terminal_writestring(", ");
    terminal_print_dec(cursor_y);
    terminal_writestring(")");
    
    // Final message
    terminal_set_cursor(0, 24);
    terminal_setcolor(VGA_COLOR(VGA_LIGHT_GREEN, VGA_BLACK));
    terminal_println("Enhanced terminal system ready! Press any key to continue...");
    
    // Infinite loop - kernel should never return
    while (1) {
        // Halt the CPU
        __asm__ volatile("hlt");
    }
} 