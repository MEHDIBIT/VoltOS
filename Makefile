# Makefile for building VoltOS

# Tools
CC = gcc
LD = ld
ASM = nasm

# Compiler flags
CFLAGS = -m32 -ffreestanding -nostdlib -fno-builtin -Wall -Wextra -I.

# Linker flags
LDFLAGS = -T linker.ld -m elf_i386

# Assembler flags
ASMFLAGS = -f elf32

# Source files
# Added shell.o to the list of object files
OBJ_FILES = boot.o kernel.o gdt.o idt.o keyboard.o isr.o timer.o shell.o
ISO_NAME = VoltOS.iso

# Default target
all: $(ISO_NAME)

# Rule to create the final ISO image
$(ISO_NAME): kernel.bin grub.cfg
	@mkdir -p isodir/boot/grub
	@cp kernel.bin isodir/boot/kernel.bin
	@cp grub.cfg isodir/boot/grub/grub.cfg
	@grub-mkrescue -o $(ISO_NAME) isodir
	@rm -rf isodir

# Rule to link the kernel
kernel.bin: $(OBJ_FILES)
	$(LD) $(LDFLAGS) -o $@ $^

# Rule to compile C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to assemble assembly files
%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# Rule to run the OS in QEMU
run: $(ISO_NAME)
	qemu-system-i386 -cdrom $(ISO_NAME)

# Rule to run the OS in debug mode
debug: $(ISO_NAME)
	qemu-system-i386 -cdrom $(ISO_NAME) -d int -no-reboot

# Rule to clean up build files
clean:
	rm -f *.o kernel.bin $(ISO_NAME)
	rm -rf isodir

.PHONY: all clean run debug
