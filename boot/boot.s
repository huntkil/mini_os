; Mini OS Bootloader
; This bootloader loads the kernel from disk and jumps to it

[org 0x7c00]                    ; BIOS loads bootloader at 0x7c00
[bits 16]                       ; Start in 16-bit real mode

; Bootloader entry point
start:
    ; Initialize segment registers
    cli                         ; Disable interrupts
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00             ; Set stack pointer
    sti                         ; Re-enable interrupts

    ; Save boot drive number
    mov [boot_drive], dl

    ; Print boot message
    mov si, boot_msg
    call print_string

    ; Load kernel from disk
    mov bx, KERNEL_OFFSET      ; Load kernel to this address
    mov dh, 15                 ; Number of sectors to read (adjust as needed)
    mov dl, [boot_drive]       ; Drive number
    call disk_load

    ; Switch to protected mode
    cli                         ; Disable interrupts
    lgdt [gdt_descriptor]      ; Load GDT

    ; Enable A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Set PE bit in CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to flush pipeline and switch to 32-bit
    jmp CODE_SEG:init_pm

[bits 32]
init_pm:
    ; Set up segment registers for 32-bit mode
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Set up stack
    mov ebp, 0x90000
    mov esp, ebp

    ; Jump to kernel
    call KERNEL_OFFSET

    ; Should never reach here
    jmp $

; Data
boot_drive db 0
boot_msg db 'Mini OS Bootloader Starting...', 0x0D, 0x0A, 0

; Constants
KERNEL_OFFSET equ 0x1000
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; GDT
gdt_start:
gdt_null:                       ; Null descriptor
    dd 0
    dd 0

gdt_code:                       ; Code segment descriptor
    dw 0xffff                   ; Limit (bits 0-15)
    dw 0x0000                   ; Base (bits 0-15)
    db 0x00                     ; Base (bits 16-23)
    db 10011010b                ; Access byte
    db 11001111b                ; Flags + Limit (bits 16-19)
    db 0x00                     ; Base (bits 24-31)

gdt_data:                       ; Data segment descriptor
    dw 0xffff                   ; Limit (bits 0-15)
    dw 0x0000                   ; Base (bits 0-15)
    db 0x00                     ; Base (bits 16-23)
    db 10010010b                ; Access byte
    db 11001111b                ; Flags + Limit (bits 16-19)
    db 0x00                     ; Base (bits 24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; GDT size
    dd gdt_start                ; GDT address

; Functions
[bits 16]
print_string:
    pusha
    mov ah, 0x0e                ; BIOS teletype function
.loop:
    lodsb                       ; Load next character
    or al, al                   ; Check if character is null
    jz .done                    ; If null, we're done
    int 0x10                    ; Print character
    jmp .loop                   ; Repeat for next character
.done:
    popa
    ret

disk_load:
    pusha
    push dx                     ; Save number of sectors to read

    mov ah, 0x02                ; BIOS read sector function
    mov al, dh                  ; Number of sectors to read
    mov ch, 0x00                ; Cylinder 0
    mov dh, 0x00                ; Head 0
    mov cl, 0x02                ; Start from sector 2 (sector 1 is bootloader)
    int 0x13                    ; BIOS interrupt

    jc disk_error               ; Jump if error (carry flag set)

    pop dx                      ; Restore DX
    cmp al, dh                  ; Check if we read the expected number of sectors
    jne disk_error

    popa
    ret

disk_error:
    mov si, disk_error_msg
    call print_string
    jmp $

disk_error_msg db 'Disk read error!', 0x0D, 0x0A, 0

; Boot sector padding
times 510-($-$$) db 0           ; Pad to 510 bytes
dw 0xaa55                       ; Boot signature 