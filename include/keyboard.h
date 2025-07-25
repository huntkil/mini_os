#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "terminal.h"

// Keyboard ports
#define KEYBOARD_DATA_PORT    0x60
#define KEYBOARD_COMMAND_PORT 0x64

// Keyboard commands
#define KEYBOARD_CMD_READ_CONFIG  0x20
#define KEYBOARD_CMD_WRITE_CONFIG 0x60
#define KEYBOARD_CMD_DISABLE      0xAD
#define KEYBOARD_CMD_ENABLE       0xAE
#define KEYBOARD_CMD_RESET        0xFF

// Keyboard configuration bits
#define KEYBOARD_CONFIG_INTERRUPT 0x01
#define KEYBOARD_CONFIG_TRANSLATE 0x40

// Special key scancodes
#define SCANCODE_ESCAPE       0x01
#define SCANCODE_BACKSPACE    0x0E
#define SCANCODE_TAB          0x0F
#define SCANCODE_ENTER        0x1C
#define SCANCODE_LEFT_SHIFT   0x2A
#define SCANCODE_RIGHT_SHIFT  0x36
#define SCANCODE_LEFT_CTRL    0x1D
#define SCANCODE_LEFT_ALT     0x38
#define SCANCODE_CAPS_LOCK    0x3A
#define SCANCODE_F1           0x3B
#define SCANCODE_F2           0x3C
#define SCANCODE_F3           0x3D
#define SCANCODE_F4           0x3E
#define SCANCODE_F5           0x3F
#define SCANCODE_F6           0x40
#define SCANCODE_F7           0x41
#define SCANCODE_F8           0x42
#define SCANCODE_F9           0x43
#define SCANCODE_F10          0x44
#define SCANCODE_F11          0x57
#define SCANCODE_F12          0x58
#define SCANCODE_NUM_LOCK     0x45
#define SCANCODE_SCROLL_LOCK  0x46
#define SCANCODE_HOME         0x47
#define SCANCODE_UP           0x48
#define SCANCODE_PAGE_UP      0x49
#define SCANCODE_LEFT         0x4B
#define SCANCODE_CENTER       0x4C
#define SCANCODE_RIGHT        0x4D
#define SCANCODE_END          0x4F
#define SCANCODE_DOWN         0x50
#define SCANCODE_PAGE_DOWN    0x51
#define SCANCODE_INSERT       0x52
#define SCANCODE_DELETE       0x53

// Extended key prefix
#define SCANCODE_EXTENDED     0xE0

// Key states
#define KEY_STATE_RELEASED    0x80
#define KEY_STATE_PRESSED     0x00

// Keyboard state structure
typedef struct {
    uint8_t shift_pressed;
    uint8_t ctrl_pressed;
    uint8_t alt_pressed;
    uint8_t caps_lock;
    uint8_t num_lock;
    uint8_t scroll_lock;
    uint8_t extended_key;
} keyboard_state_t;

// Function declarations
void keyboard_init(void);
void keyboard_handler(void);
uint8_t keyboard_read_scancode(void);
char keyboard_scancode_to_ascii(uint8_t scancode);
void keyboard_process_scancode(uint8_t scancode);
void keyboard_echo_character(char c);
void keyboard_handle_special_key(uint8_t scancode);

// Command line interface
#define MAX_COMMAND_LENGTH 256
#define MAX_COMMAND_HISTORY 10

typedef struct {
    char buffer[MAX_COMMAND_LENGTH];
    uint16_t position;
    uint16_t length;
    char history[MAX_COMMAND_HISTORY][MAX_COMMAND_LENGTH];
    uint8_t history_count;
    uint8_t history_position;
} command_line_t;

// Command line functions
void command_line_init(void);
void command_line_process_input(char c);
void command_line_handle_backspace(void);
void command_line_handle_enter(void);
void command_line_handle_arrow_keys(uint8_t scancode);
void command_line_execute_command(const char* command);
void command_line_display_prompt(void);
void command_line_add_to_history(const char* command);

// Built-in commands
void cmd_help(void);
void cmd_clear(void);
void cmd_echo(const char* args);
void cmd_reboot(void);
void cmd_version(void);
void cmd_status(void);

#endif // KEYBOARD_H 