#ifndef TERMINAL_H
#define TERMINAL_H

// Standard integer types
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

// Size type
typedef uint32_t size_t;

// String length function declaration
size_t strlen(const char* str);

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
#define VGA_LIGHT_YELLOW 14  // Same as LIGHT_BROWN in VGA
#define VGA_WHITE 15

// Color utility macros
#define VGA_COLOR(fg, bg) ((bg) << 4 | (fg))
#define VGA_ENTRY(ch, color) ((uint16_t)(ch) | (uint16_t)(color) << 8)

// Terminal position structure
typedef struct {
    uint16_t x;
    uint16_t y;
} terminal_pos_t;

// Terminal state structure
typedef struct {
    uint8_t color;
    terminal_pos_t cursor;
    uint8_t saved_color;
    terminal_pos_t saved_cursor;
} terminal_state_t;

// Basic terminal functions
void terminal_initialize(void);
void terminal_clear(void);
void terminal_clear_line(uint16_t line);
void terminal_scroll(void);

// Color management
void terminal_setcolor(uint8_t color);
uint8_t terminal_getcolor(void);
void terminal_set_foreground(uint8_t color);
void terminal_set_background(uint8_t color);
void terminal_save_color(void);
void terminal_restore_color(void);

// Cursor management
void terminal_set_cursor(uint16_t x, uint16_t y);
void terminal_get_cursor(uint16_t* x, uint16_t* y);
void terminal_move_cursor(int16_t dx, int16_t dy);
void terminal_save_cursor(void);
void terminal_restore_cursor(void);
void terminal_hide_cursor(void);
void terminal_show_cursor(void);

// Character output
void terminal_putchar(char c);
void terminal_putchar_at(char c, uint16_t x, uint16_t y);
void terminal_putchar_color(char c, uint8_t color);

// String output
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_writestring_at(const char* data, uint16_t x, uint16_t y);
void terminal_writestring_color(const char* data, uint8_t color);

// Enhanced output functions
void terminal_printf(const char* format, ...);
void terminal_println(const char* str);
void terminal_print_hex(uint32_t value);
void terminal_print_dec(uint32_t value);
void terminal_print_bin(uint32_t value);

// Screen management
void terminal_fill_screen(char c, uint8_t color);
void terminal_draw_box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, char border_char, uint8_t color);
void terminal_draw_line_horizontal(uint16_t x, uint16_t y, uint16_t length, char c, uint8_t color);
void terminal_draw_line_vertical(uint16_t x, uint16_t y, uint16_t length, char c, uint8_t color);

// Utility functions
uint16_t terminal_get_index(uint16_t x, uint16_t y);
void terminal_update_cursor(void);
void terminal_handle_escape_sequence(const char* sequence);

#endif // TERMINAL_H 