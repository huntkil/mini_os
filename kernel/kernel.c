#include "kernel.h"

// VGA text mode constants
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFFER 0xB8000

// VGA colors
#define VGA_BLACK 0
#define VGA_BLUE 1
#define VGA_GREEN 2
#define VGA_CYAN 3
#define VGA_RED 4
#define VGA_MAGENTA 5
#define VGA_BROWN 6
#define VGA_LIGHT_GREY 7
#define VGA_DARK_GREY 8
#define VGA_LIGHT_BLUE 9
#define VGA_LIGHT_GREEN 10
#define VGA_LIGHT_CYAN 11
#define VGA_LIGHT_RED 12
#define VGA_LIGHT_MAGENTA 13
#define VGA_LIGHT_BROWN 14
#define VGA_WHITE 15

// Global variables
static uint16_t* vga_buffer = (uint16_t*)VGA_BUFFER;
static uint8_t terminal_color = VGA_LIGHT_GREY | (VGA_BLACK << 4);
static uint16_t terminal_column = 0;
static uint16_t terminal_row = 0;

// Function prototypes
void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void kernel_main(void);

// Initialize terminal
void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = VGA_LIGHT_GREY | (VGA_BLACK << 4);
    
    // Clear the screen
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = (uint16_t)' ' | (uint16_t)terminal_color << 8;
        }
    }
}

// Set terminal color
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

// Put character at current position
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            // Scroll the screen
            for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                    vga_buffer[y * VGA_WIDTH + x] = vga_buffer[(y + 1) * VGA_WIDTH + x];
                }
            }
            // Clear the last line
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (uint16_t)' ' | (uint16_t)terminal_color << 8;
            }
            terminal_row = VGA_HEIGHT - 1;
        }
    } else {
        const size_t index = terminal_row * VGA_WIDTH + terminal_column;
        vga_buffer[index] = (uint16_t)c | (uint16_t)terminal_color << 8;
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) {
                // Scroll the screen
                for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
                    for (size_t x = 0; x < VGA_WIDTH; x++) {
                        vga_buffer[y * VGA_WIDTH + x] = vga_buffer[(y + 1) * VGA_WIDTH + x];
                    }
                }
                // Clear the last line
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                    vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (uint16_t)' ' | (uint16_t)terminal_color << 8;
                }
                terminal_row = VGA_HEIGHT - 1;
            }
        }
    }
}

// Write string to terminal
void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

// Write null-terminated string to terminal
void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

// Simple string length function
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

// Main kernel entry point
void kernel_main(void) {
    // Initialize terminal
    terminal_initialize();
    
    // Set color to light green on black
    terminal_setcolor(VGA_LIGHT_GREEN | (VGA_BLACK << 4));
    
    // Print welcome message
    terminal_writestring("Welcome to Mini OS!\n");
    terminal_writestring("Kernel loaded successfully.\n");
    terminal_writestring("System initialized.\n\n");
    
    // Set color to white on black
    terminal_setcolor(VGA_WHITE | (VGA_BLACK << 4));
    terminal_writestring("Mini OS v1.0 - Basic Kernel\n");
    terminal_writestring("Ready for next development phase...\n");
    
    // Infinite loop - kernel should never return
    while (1) {
        // Halt the CPU
        __asm__ volatile("hlt");
    }
} 