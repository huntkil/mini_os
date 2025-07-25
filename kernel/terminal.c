#include "terminal.h"

// Global variables
static uint16_t* vga_buffer = (uint16_t*)VGA_BUFFER;
static terminal_state_t terminal_state = {0};

// Initialize terminal
void terminal_initialize(void) {
    terminal_state.cursor.x = 0;
    terminal_state.cursor.y = 0;
    terminal_state.color = VGA_COLOR(VGA_LIGHT_GREY, VGA_BLACK);
    terminal_state.saved_color = terminal_state.color;
    terminal_state.saved_cursor = terminal_state.cursor;
    
    terminal_clear();
}

// Clear entire screen
void terminal_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = VGA_ENTRY(' ', terminal_state.color);
        }
    }
    terminal_state.cursor.x = 0;
    terminal_state.cursor.y = 0;
}

// Clear specific line
void terminal_clear_line(uint16_t line) {
    if (line >= VGA_HEIGHT) return;
    
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index = line * VGA_WIDTH + x;
        vga_buffer[index] = VGA_ENTRY(' ', terminal_state.color);
    }
}

// Scroll screen up
void terminal_scroll(void) {
    // Move all lines up by one
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = vga_buffer[(y + 1) * VGA_WIDTH + x];
        }
    }
    
    // Clear the last line
    terminal_clear_line(VGA_HEIGHT - 1);
    
    // Adjust cursor if it was at the bottom
    if (terminal_state.cursor.y > 0) {
        terminal_state.cursor.y--;
    }
}

// Color management functions
void terminal_setcolor(uint8_t color) {
    terminal_state.color = color;
}

uint8_t terminal_getcolor(void) {
    return terminal_state.color;
}

void terminal_set_foreground(uint8_t color) {
    terminal_state.color = (terminal_state.color & 0xF0) | (color & 0x0F);
}

void terminal_set_background(uint8_t color) {
    terminal_state.color = (terminal_state.color & 0x0F) | ((color & 0x0F) << 4);
}

void terminal_save_color(void) {
    terminal_state.saved_color = terminal_state.color;
}

void terminal_restore_color(void) {
    terminal_state.color = terminal_state.saved_color;
}

// Cursor management functions
void terminal_set_cursor(uint16_t x, uint16_t y) {
    if (x < VGA_WIDTH && y < VGA_HEIGHT) {
        terminal_state.cursor.x = x;
        terminal_state.cursor.y = y;
    }
}

void terminal_get_cursor(uint16_t* x, uint16_t* y) {
    if (x) *x = terminal_state.cursor.x;
    if (y) *y = terminal_state.cursor.y;
}

void terminal_move_cursor(int16_t dx, int16_t dy) {
    int32_t new_x = terminal_state.cursor.x + dx;
    int32_t new_y = terminal_state.cursor.y + dy;
    
    if (new_x >= 0 && new_x < VGA_WIDTH && new_y >= 0 && new_y < VGA_HEIGHT) {
        terminal_state.cursor.x = new_x;
        terminal_state.cursor.y = new_y;
    }
}

void terminal_save_cursor(void) {
    terminal_state.saved_cursor = terminal_state.cursor;
}

void terminal_restore_cursor(void) {
    terminal_state.cursor = terminal_state.saved_cursor;
}

void terminal_hide_cursor(void) {
    // In text mode, we can't actually hide the cursor, but we can move it off-screen
    terminal_set_cursor(VGA_WIDTH, VGA_HEIGHT);
}

void terminal_show_cursor(void) {
    // Move cursor back to a visible position
    terminal_set_cursor(terminal_state.cursor.x, terminal_state.cursor.y);
}

// Character output functions
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_state.cursor.x = 0;
        if (++terminal_state.cursor.y == VGA_HEIGHT) {
            terminal_scroll();
        }
    } else if (c == '\r') {
        terminal_state.cursor.x = 0;
    } else if (c == '\t') {
        // Tab: move to next tab stop (every 8 characters)
        terminal_state.cursor.x = (terminal_state.cursor.x + 8) & ~7;
        if (terminal_state.cursor.x >= VGA_WIDTH) {
            terminal_state.cursor.x = 0;
            if (++terminal_state.cursor.y == VGA_HEIGHT) {
                terminal_scroll();
            }
        }
    } else if (c == '\b') {
        // Backspace
        if (terminal_state.cursor.x > 0) {
            terminal_state.cursor.x--;
            terminal_putchar_at(' ', terminal_state.cursor.x, terminal_state.cursor.y);
        }
    } else {
        terminal_putchar_at(c, terminal_state.cursor.x, terminal_state.cursor.y);
        if (++terminal_state.cursor.x == VGA_WIDTH) {
            terminal_state.cursor.x = 0;
            if (++terminal_state.cursor.y == VGA_HEIGHT) {
                terminal_scroll();
            }
        }
    }
}

void terminal_putchar_at(char c, uint16_t x, uint16_t y) {
    if (x < VGA_WIDTH && y < VGA_HEIGHT) {
        const size_t index = y * VGA_WIDTH + x;
        vga_buffer[index] = VGA_ENTRY(c, terminal_state.color);
    }
}

void terminal_putchar_color(char c, uint8_t color) {
    uint8_t old_color = terminal_state.color;
    terminal_state.color = color;
    terminal_putchar(c);
    terminal_state.color = old_color;
}

// String output functions
void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

void terminal_writestring_at(const char* data, uint16_t x, uint16_t y) {
    uint16_t old_x = terminal_state.cursor.x;
    uint16_t old_y = terminal_state.cursor.y;
    
    terminal_set_cursor(x, y);
    terminal_writestring(data);
    
    terminal_set_cursor(old_x, old_y);
}

void terminal_writestring_color(const char* data, uint8_t color) {
    uint8_t old_color = terminal_state.color;
    terminal_state.color = color;
    terminal_writestring(data);
    terminal_state.color = old_color;
}

// Enhanced output functions
void terminal_println(const char* str) {
    terminal_writestring(str);
    terminal_putchar('\n');
}

void terminal_print_hex(uint32_t value) {
    const char hex_chars[] = "0123456789ABCDEF";
    char hex_str[9] = "0x";
    
    for (int i = 0; i < 8; i++) {
        hex_str[i + 2] = hex_chars[(value >> (28 - i * 4)) & 0xF];
    }
    hex_str[10] = '\0';
    
    terminal_writestring(hex_str);
}

void terminal_print_dec(uint32_t value) {
    if (value == 0) {
        terminal_putchar('0');
        return;
    }
    
    char buffer[11];
    int i = 0;
    
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    
    // Print in reverse order
    while (i > 0) {
        terminal_putchar(buffer[--i]);
    }
}

void terminal_print_bin(uint32_t value) {
    terminal_writestring("0b");
    
    if (value == 0) {
        terminal_putchar('0');
        return;
    }
    
    // Find the highest bit set
    uint32_t mask = 0x80000000;
    int started = 0;
    
    for (int i = 0; i < 32; i++) {
        if (value & mask) {
            started = 1;
            terminal_putchar('1');
        } else if (started) {
            terminal_putchar('0');
        }
        mask >>= 1;
    }
}

// Simple printf implementation
void terminal_printf(const char* format, ...) {
    // For now, implement a basic version
    // In a full implementation, you would parse format specifiers
    terminal_writestring(format);
}

// Screen management functions
void terminal_fill_screen(char c, uint8_t color) {
    uint8_t old_color = terminal_state.color;
    terminal_state.color = color;
    
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_putchar_at(c, x, y);
        }
    }
    
    terminal_state.color = old_color;
}

void terminal_draw_box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, char border_char, uint8_t color) {
    if (x1 >= VGA_WIDTH || y1 >= VGA_HEIGHT || x2 >= VGA_WIDTH || y2 >= VGA_HEIGHT) return;
    
    uint8_t old_color = terminal_state.color;
    terminal_state.color = color;
    
    // Draw horizontal lines
    for (uint16_t x = x1; x <= x2; x++) {
        terminal_putchar_at(border_char, x, y1);
        terminal_putchar_at(border_char, x, y2);
    }
    
    // Draw vertical lines
    for (uint16_t y = y1; y <= y2; y++) {
        terminal_putchar_at(border_char, x1, y);
        terminal_putchar_at(border_char, x2, y);
    }
    
    terminal_state.color = old_color;
}

void terminal_draw_line_horizontal(uint16_t x, uint16_t y, uint16_t length, char c, uint8_t color) {
    if (y >= VGA_HEIGHT) return;
    
    uint8_t old_color = terminal_state.color;
    terminal_state.color = color;
    
    for (uint16_t i = 0; i < length && x + i < VGA_WIDTH; i++) {
        terminal_putchar_at(c, x + i, y);
    }
    
    terminal_state.color = old_color;
}

void terminal_draw_line_vertical(uint16_t x, uint16_t y, uint16_t length, char c, uint8_t color) {
    if (x >= VGA_WIDTH) return;
    
    uint8_t old_color = terminal_state.color;
    terminal_state.color = color;
    
    for (uint16_t i = 0; i < length && y + i < VGA_HEIGHT; i++) {
        terminal_putchar_at(c, x, y + i);
    }
    
    terminal_state.color = old_color;
}

// Utility functions
uint16_t terminal_get_index(uint16_t x, uint16_t y) {
    return y * VGA_WIDTH + x;
}

void terminal_update_cursor(void) {
    // In a real implementation, you might update hardware cursor
    // For now, we just ensure our internal state is consistent
}

void terminal_handle_escape_sequence(const char* sequence) {
    // Basic escape sequence handling
    // This would be expanded in a full implementation
    (void)sequence; // Suppress unused parameter warning
}

// String length function
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

// String copy function
char* strcpy(char* dest, const char* src) {
    char* ptr = dest;
    while (*src) {
        *ptr = *src;
        ptr++;
        src++;
    }
    *ptr = '\0';
    return dest;
}

// String compare function
int strcmp(const char* str1, const char* str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return (*str1 < *str2) ? -1 : 1;
        }
        str1++;
        str2++;
    }
    return (*str1 == *str2) ? 0 : (*str1 < *str2) ? -1 : 1;
}

// String compare with length function
int strncmp(const char* str1, const char* str2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (str1[i] != str2[i]) {
            return (str1[i] < str2[i]) ? -1 : 1;
        }
        if (str1[i] == '\0') {
            return 0;
        }
    }
    return 0;
} 