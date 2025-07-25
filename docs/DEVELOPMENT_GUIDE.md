# Mini OS Development Guide

## 🎯 Project Overview

This document provides comprehensive guidelines for developing the Mini Linux OS project. It includes current implementation details, coding standards, and development procedures.

## 📋 Current Project Status

### ✅ Completed Features (Phase 1)

1. **Basic Bootloader** (`boot/boot.s`)
   - 512-byte MBR bootloader in x86 assembly
   - Loads kernel from disk sector 2
   - Switches from real mode to protected mode
   - Sets up GDT with code and data segments
   - Jumps to kernel at address 0x1000

2. **Kernel Entry Point** (`kernel/kernel.c`)
   - Main kernel entry point: `kernel_main()`
   - VGA text mode display driver
   - Basic terminal output functions
   - No standard library dependencies

3. **VGA Text Mode Implementation**
   - 80x25 character display
   - 16 foreground and background colors
   - Memory mapped at 0xB8000
   - Screen scrolling support
   - Functions: `terminal_initialize()`, `terminal_putchar()`, `terminal_writestring()`

4. **Build System** (`Makefile`)
   - GNU Make with dependency tracking
   - NASM for assembly compilation
   - GCC with freestanding flags
   - Custom linker script (`kernel/linker.ld`)
   - Creates 1.44MB floppy disk image
   - QEMU integration for testing

5. **Project Structure**
   - Organized directory structure
   - Git repository with GitHub integration
   - Comprehensive README
   - Proper .gitignore for build artifacts

### 🔄 Planned Features

- **Phase 2**: Enhanced output system (printf, colors, cursor positioning)
- **Phase 3**: Keyboard input handling and interrupt management
- **Phase 4**: Memory management system
- **Phase 5**: Simple file system
- **Phase 6**: Basic shell and commands
- **Phase 7**: Process management and multitasking

## 🛠️ Development Environment

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt-get install build-essential nasm qemu-system-x86

# macOS
brew install nasm qemu

# Windows (WSL)
sudo apt-get install build-essential nasm qemu-system-x86
```

### Build Commands
```bash
# Build the complete OS
make all

# Run in QEMU
make run

# Debug mode
make debug

# Clean build files
make clean
```

## 🔧 Technical Specifications

### Memory Layout
- **Bootloader**: 0x7C00 - 0x7DFF (512 bytes)
- **Kernel**: 0x1000 - 0x90000
- **Stack**: 0x90000 - 0x9FFFF
- **VGA Buffer**: 0xB8000 - 0xB8FA0

### VGA Text Mode
- **Resolution**: 80x25 characters
- **Color Depth**: 16 colors (4-bit)
- **Character Format**: 2 bytes (character + color attribute)
- **Color Attribute**: 4-bit background + 4-bit foreground

### Bootloader Specifications
- **Size**: Exactly 512 bytes
- **Signature**: 0xAA55 at offset 510
- **Entry Point**: 0x7C00
- **Disk Loading**: BIOS INT 13h
- **Mode Switch**: Real mode → Protected mode

## 📝 Coding Standards

### C Code Standards
- Use 4-space indentation
- Follow Linux kernel coding style
- Use snake_case for variables and functions
- Use UPPER_CASE for constants and macros
- Add comprehensive comments for hardware-specific code
- Implement proper header guards

### Assembly Code Standards
- Use Intel syntax
- Add detailed comments explaining register usage
- Follow System V ABI calling conventions
- Use meaningful labels and constants
- Properly save/restore registers in interrupt handlers

### File Organization
```
mini-os/
├── boot/           # Bootloader code
├── kernel/         # Kernel source files
├── drivers/        # Hardware drivers
├── fs/            # File system implementation
├── lib/           # Utility libraries
├── include/       # Header files
├── build/         # Build artifacts (generated)
├── scripts/       # Build and utility scripts
└── docs/          # Documentation
```

## 🚨 Safety Guidelines

### Memory Safety
- Always validate memory addresses before access
- Check for null pointers before dereferencing
- Implement proper memory alignment
- Use bounds checking for buffer operations
- Validate VGA buffer access (0xB8000 - 0xB8FA0)

### Error Handling
- Implement graceful degradation for non-critical errors
- Provide meaningful error messages
- Log errors for debugging purposes
- Handle disk read errors in bootloader
- Validate all user input and system call parameters

### Hardware Interaction
- Use volatile qualifiers for hardware registers
- Implement proper synchronization for shared resources
- Add timeout mechanisms for hardware operations
- Handle all possible error conditions
- Document hardware dependencies clearly

## 🔍 Testing and Debugging

### QEMU Testing
```bash
# Normal run
make run

# Debug mode with GDB
make debug

# In another terminal, connect GDB
gdb -ex "target remote localhost:1234" -ex "symbol-file build/kernel.elf"
```

### Common Debugging Commands
```bash
# Check bootloader size
ls -la build/boot.bin

# Verify disk image
file build/mini-os.img

# Check kernel symbols
nm build/kernel.elf

# Disassemble kernel
objdump -d build/kernel.elf
```

### Debugging Tips
- Use QEMU's `-d` flag for detailed logging
- Add debug output to kernel using VGA
- Use GDB to step through bootloader and kernel
- Check memory layout with GDB's `x` command
- Verify register states during mode switches

## 📚 Reference Materials

### Documentation
- [OSDev Wiki](https://wiki.osdev.org/)
- [Intel x86 Architecture Manuals](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
- [System V ABI](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf)
- [VGA Text Mode](https://wiki.osdev.org/Text_Mode_Cursor)

### Tools
- **NASM**: Netwide Assembler for x86 assembly
- **GCC**: GNU Compiler Collection with cross-compilation
- **QEMU**: Machine emulator for testing
- **GDB**: GNU Debugger for kernel debugging
- **objcopy**: Binary manipulation tool

## 🚀 Development Workflow

### 1. Feature Development
1. Create a feature branch: `git checkout -b feature/new-feature`
2. Implement the feature following coding standards
3. Test thoroughly in QEMU
4. Update documentation if needed
5. Commit with descriptive message: `git commit -m "feat: Add new feature"`
6. Push to GitHub: `git push origin feature/new-feature`

### 2. Code Review
- Review all code changes
- Ensure proper error handling
- Verify memory safety
- Check for performance issues
- Validate hardware interactions

### 3. Integration
- Merge feature branch to main
- Run full test suite
- Update version tags if needed
- Update documentation

## 🎯 Next Steps

### Phase 2: Enhanced Output System
- Implement printf-like function with format specifiers
- Add cursor positioning and control
- Implement color management system
- Add screen clearing and scrolling improvements

### Phase 3: Keyboard Input
- Set up Interrupt Descriptor Table (IDT)
- Configure Programmable Interrupt Controller (PIC)
- Implement keyboard interrupt handler (IRQ1)
- Add scancode to ASCII conversion
- Implement basic command line interface

### Phase 4: Memory Management
- Implement physical memory allocator
- Set up virtual memory with paging
- Create kernel heap allocator
- Add memory protection mechanisms
- Implement memory mapping utilities

---

**Note**: This document should be updated as the project evolves. Always refer to the latest version for current development guidelines. 