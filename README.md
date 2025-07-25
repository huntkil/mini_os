# Mini Linux OS

A minimal Linux-like operating system built from scratch for educational purposes.

## 🎯 Project Overview

This project implements a basic operating system with the following features:
- x86 architecture support (32-bit)
- Custom bootloader in assembly
- Basic kernel in C
- VGA text mode display
- Simple memory management
- And more features to be added...

## 📁 Project Structure

```
mini-os/
├── boot/           # Bootloader code (assembly)
├── kernel/         # Kernel source files (C)
├── drivers/        # Hardware drivers
├── fs/            # File system implementation
├── lib/           # Utility libraries
├── include/       # Header files
├── build/         # Build artifacts
├── scripts/       # Build and utility scripts
└── docs/          # Documentation
```

## 🛠️ Prerequisites

Before building the OS, you need to install the required tools:

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install build-essential nasm qemu-system-x86
```

### macOS
```bash
brew install nasm qemu
```

### Windows (WSL)
```bash
sudo apt-get update
sudo apt-get install build-essential nasm qemu-system-x86
```

## 🚀 Building and Running

### Quick Start
```bash
# Clone the repository
git clone <repository-url>
cd mini-os

# Install dependencies (Ubuntu/Debian)
make install-deps

# Build the OS
make all

# Run in QEMU
make run
```

### Build Commands

```bash
# Build the complete OS image
make all

# Run in QEMU
make run

# Run in QEMU with debug support
make debug

# Clean build files
make clean

# Show help
make help
```

## 🔧 Development

### Current Features (Phase 1)
- ✅ Basic bootloader in assembly
- ✅ Kernel entry point in C
- ✅ VGA text mode display
- ✅ Basic terminal output
- ✅ Build system with Makefile

### Planned Features
- [ ] Keyboard input handling
- [ ] Memory management system
- [ ] Simple file system
- [ ] Basic shell and commands
- [ ] Process management
- [ ] System calls interface

## 🐛 Debugging

### QEMU Debug Mode
```bash
# Start QEMU in debug mode
make debug

# In another terminal, connect with GDB
gdb -ex "target remote localhost:1234" -ex "symbol-file build/kernel.elf"
```

### Common Issues

1. **Build fails with "nasm not found"**
   - Install NASM: `sudo apt-get install nasm`

2. **QEMU not starting**
   - Check if QEMU is installed: `qemu-system-i386 --version`
   - Install QEMU: `sudo apt-get install qemu-system-x86`

3. **Permission denied when creating disk image**
   - Make sure you have write permissions in the build directory

## 📚 Learning Resources

- [OSDev Wiki](https://wiki.osdev.org/)
- [Intel x86 Architecture Manuals](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
- [System V ABI](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf)

## 🤝 Contributing

This is an educational project. Feel free to:
- Report bugs
- Suggest improvements
- Add new features
- Improve documentation

## 📄 License

This project is open source and available under the MIT License.

## 🎓 Educational Purpose

This OS is designed for learning purposes. It demonstrates:
- Low-level programming concepts
- Operating system architecture
- Hardware interaction
- System programming techniques

---

**Note**: This is a work in progress. The OS is not production-ready and should only be used for educational purposes. 