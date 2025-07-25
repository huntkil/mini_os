#include "keyboard.h"

// Global variables
static keyboard_state_t keyboard_state = {0};
static command_line_t command_line = {0};

// Scancode to ASCII conversion table (US layout)
static const char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

// Shifted scancode to ASCII conversion table
static const char scancode_to_ascii_shift[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' '
};

// Initialize keyboard
void keyboard_init(void) {
    // Clear keyboard state
    keyboard_state.shift_pressed = 0;
    keyboard_state.ctrl_pressed = 0;
    keyboard_state.alt_pressed = 0;
    keyboard_state.caps_lock = 0;
    keyboard_state.num_lock = 1;
    keyboard_state.scroll_lock = 0;
    keyboard_state.extended_key = 0;
    
    // Initialize command line
    command_line_init();
    
    terminal_setcolor(VGA_COLOR(VGA_LIGHT_GREEN, VGA_BLACK));
    terminal_println("Keyboard initialized");
}

// Keyboard interrupt handler
void keyboard_handler(void) {
    uint8_t scancode = keyboard_read_scancode();
    keyboard_process_scancode(scancode);
}

// Read scancode from keyboard
uint8_t keyboard_read_scancode(void) {
    uint8_t scancode;
    __asm__ volatile("inb %1, %0" : "=a"(scancode) : "Nd"(KEYBOARD_DATA_PORT));
    return scancode;
}

// Convert scancode to ASCII
char keyboard_scancode_to_ascii(uint8_t scancode) {
    if (scancode >= sizeof(scancode_to_ascii)) {
        return 0;
    }
    
    char c = keyboard_state.shift_pressed ? 
        scancode_to_ascii_shift[scancode] : 
        scancode_to_ascii[scancode];
    
    // Handle caps lock
    if (keyboard_state.caps_lock && c >= 'a' && c <= 'z') {
        c = c - 'a' + 'A';
    }
    
    return c;
}

// Process scancode
void keyboard_process_scancode(uint8_t scancode) {
    // Check if key is released
    if (scancode & KEY_STATE_RELEASED) {
        scancode &= ~KEY_STATE_RELEASED;
        
        // Handle key release
        switch (scancode) {
            case SCANCODE_LEFT_SHIFT:
            case SCANCODE_RIGHT_SHIFT:
                keyboard_state.shift_pressed = 0;
                break;
            case SCANCODE_LEFT_CTRL:
                keyboard_state.ctrl_pressed = 0;
                break;
            case SCANCODE_LEFT_ALT:
                keyboard_state.alt_pressed = 0;
                break;
        }
        return;
    }
    
    // Handle key press
    switch (scancode) {
        case SCANCODE_LEFT_SHIFT:
        case SCANCODE_RIGHT_SHIFT:
            keyboard_state.shift_pressed = 1;
            break;
        case SCANCODE_LEFT_CTRL:
            keyboard_state.ctrl_pressed = 1;
            break;
        case SCANCODE_LEFT_ALT:
            keyboard_state.alt_pressed = 1;
            break;
        case SCANCODE_CAPS_LOCK:
            keyboard_state.caps_lock = !keyboard_state.caps_lock;
            break;
        case SCANCODE_BACKSPACE:
            command_line_handle_backspace();
            break;
        case SCANCODE_ENTER:
            command_line_handle_enter();
            break;
        default:
            // Convert to ASCII and process
            char c = keyboard_scancode_to_ascii(scancode);
            if (c) {
                command_line_process_input(c);
            }
            break;
    }
}

// Echo character to terminal
void keyboard_echo_character(char c) {
    terminal_putchar(c);
}

// Handle special keys
void keyboard_handle_special_key(uint8_t scancode) {
    // For now, just ignore special keys
    (void)scancode;
}

// Command line functions
void command_line_init(void) {
    command_line.position = 0;
    command_line.length = 0;
    command_line.history_count = 0;
    command_line.history_position = 0;
    
    for (int i = 0; i < MAX_COMMAND_LENGTH; i++) {
        command_line.buffer[i] = 0;
    }
    
    for (int i = 0; i < MAX_COMMAND_HISTORY; i++) {
        for (int j = 0; j < MAX_COMMAND_LENGTH; j++) {
            command_line.history[i][j] = 0;
        }
    }
    
    command_line_display_prompt();
}

void command_line_process_input(char c) {
    if (command_line.length < MAX_COMMAND_LENGTH - 1) {
        command_line.buffer[command_line.length] = c;
        command_line.length++;
        command_line.position = command_line.length;
        keyboard_echo_character(c);
    }
}

void command_line_handle_backspace(void) {
    if (command_line.length > 0) {
        command_line.length--;
        command_line.position = command_line.length;
        command_line.buffer[command_line.length] = 0;
        
        // Move cursor back and clear character
        terminal_move_cursor(-1, 0);
        terminal_putchar(' ');
        terminal_move_cursor(-1, 0);
    }
}

void command_line_handle_enter(void) {
    terminal_putchar('\n');
    
    if (command_line.length > 0) {
        command_line_add_to_history(command_line.buffer);
        command_line_execute_command(command_line.buffer);
    }
    
    // Clear buffer
    command_line.length = 0;
    command_line.position = 0;
    for (int i = 0; i < MAX_COMMAND_LENGTH; i++) {
        command_line.buffer[i] = 0;
    }
    
    command_line_display_prompt();
}

void command_line_execute_command(const char* command) {
    if (strlen(command) == 0) return;
    
    // Simple command parsing
    if (strcmp(command, "help") == 0) {
        cmd_help();
    } else if (strcmp(command, "clear") == 0) {
        cmd_clear();
    } else if (strncmp(command, "echo ", 5) == 0) {
        cmd_echo(command + 5);
    } else if (strcmp(command, "reboot") == 0) {
        cmd_reboot();
    } else if (strcmp(command, "version") == 0) {
        cmd_version();
    } else if (strcmp(command, "status") == 0) {
        cmd_status();
    } else {
        terminal_writestring("Unknown command: ");
        terminal_writestring(command);
        terminal_putchar('\n');
    }
}

void command_line_display_prompt(void) {
    terminal_setcolor(VGA_COLOR(VGA_LIGHT_GREEN, VGA_BLACK));
    terminal_writestring("mini-os> ");
    terminal_setcolor(VGA_COLOR(VGA_WHITE, VGA_BLACK));
}

void command_line_add_to_history(const char* command) {
    if (command_line.history_count < MAX_COMMAND_HISTORY) {
        strcpy(command_line.history[command_line.history_count], command);
        command_line.history_count++;
    } else {
        // Shift history up
        for (int i = 0; i < MAX_COMMAND_HISTORY - 1; i++) {
            strcpy(command_line.history[i], command_line.history[i + 1]);
        }
        strcpy(command_line.history[MAX_COMMAND_HISTORY - 1], command);
    }
    command_line.history_position = command_line.history_count;
}

// Built-in commands
void cmd_help(void) {
    terminal_println("Available commands:");
    terminal_println("  help     - Show this help");
    terminal_println("  clear    - Clear screen");
    terminal_println("  echo     - Echo text");
    terminal_println("  reboot   - Reboot system");
    terminal_println("  version  - Show version");
    terminal_println("  status   - Show system status");
}

void cmd_clear(void) {
    terminal_clear();
    command_line_display_prompt();
}

void cmd_echo(const char* args) {
    terminal_writestring(args);
    terminal_putchar('\n');
}

void cmd_reboot(void) {
    terminal_println("Rebooting...");
    // In a real system, you would trigger a reboot
    // For now, just halt
    __asm__ volatile("cli");
    __asm__ volatile("hlt");
}

void cmd_version(void) {
    terminal_println("Mini OS v1.0");
    terminal_println("Phase 3: Keyboard Input System");
    terminal_println("Built with enhanced terminal and interrupt support");
}

void cmd_status(void) {
    terminal_println("System Status:");
    terminal_writestring("  Keyboard: ");
    terminal_println(keyboard_state.shift_pressed ? "SHIFT" : "Normal");
    terminal_writestring("  Caps Lock: ");
    terminal_println(keyboard_state.caps_lock ? "ON" : "OFF");
    terminal_writestring("  Command History: ");
    terminal_print_dec(command_line.history_count);
    terminal_println(" entries");
} 