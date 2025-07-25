# Mini OS Makefile

# Compiler and tools
CC = gcc
AS = nasm
LD = ld
OBJCOPY = objcopy
QEMU = qemu-system-i386

# Compiler flags
CFLAGS = -m32 -fno-pie -fno-stack-protector -nostdlib -nostdinc -fno-builtin -fno-pic -mno-red-zone -Wall -Wextra -std=c99 -Iinclude
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T kernel/linker.ld

# Directories
BOOT_DIR = boot
KERNEL_DIR = kernel
BUILD_DIR = build

# Source files
BOOT_SRC = $(BOOT_DIR)/boot.s
KERNEL_SRC = $(KERNEL_DIR)/kernel.c $(KERNEL_DIR)/terminal.c
KERNEL_HEADERS = $(KERNEL_DIR)/kernel.h include/terminal.h

# Object files
BOOT_OBJ = $(BUILD_DIR)/boot.bin
KERNEL_OBJ = $(BUILD_DIR)/kernel.o $(BUILD_DIR)/terminal.o
KERNEL_ELF = $(BUILD_DIR)/kernel.elf
KERNEL_BIN = $(BUILD_DIR)/kernel.bin

# Final output
OS_IMG = $(BUILD_DIR)/mini-os.img

# Default target
all: $(OS_IMG)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile bootloader
$(BOOT_OBJ): $(BOOT_SRC) | $(BUILD_DIR)
	$(AS) -f bin -o $@ $<

# Compile kernel
$(BUILD_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c $(KERNEL_HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile terminal
$(BUILD_DIR)/terminal.o: $(KERNEL_DIR)/terminal.c $(KERNEL_HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Link kernel
$(KERNEL_ELF): $(BUILD_DIR)/kernel.o $(BUILD_DIR)/terminal.o | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $^

# Extract kernel binary
$(KERNEL_BIN): $(KERNEL_ELF) | $(BUILD_DIR)
	$(OBJCOPY) -O binary $< $@

# Create OS image
$(OS_IMG): $(BOOT_OBJ) $(KERNEL_BIN) | $(BUILD_DIR)
	# Create a 1.44MB floppy disk image
	dd if=/dev/zero of=$@ bs=512 count=2880
	# Write bootloader to first sector
	dd if=$(BOOT_OBJ) of=$@ conv=notrunc bs=512 count=1
	# Write kernel starting from second sector
	dd if=$(KERNEL_BIN) of=$@ conv=notrunc bs=512 seek=1

# Run in QEMU
run: $(OS_IMG)
	$(QEMU) -fda $< -display gtk -m 16

# Run in QEMU with debug
debug: $(OS_IMG)
	$(QEMU) -fda $< -display gtk -m 16 -s -S

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential nasm qemu-system-x86

# Show help
help:
	@echo "Mini OS Build System"
	@echo "===================="
	@echo "make all        - Build the complete OS image"
	@echo "make run        - Build and run in QEMU"
	@echo "make debug      - Build and run in QEMU with debug support"
	@echo "make clean      - Clean build files"
	@echo "make install-deps - Install required dependencies"
	@echo "make help       - Show this help message"

.PHONY: all run debug clean install-deps help 