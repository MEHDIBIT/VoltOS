#include "kernel.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "timer.h"
#include "shell.h"
#include "io.h"

// The VGA video memory address.
volatile unsigned short *video_memory = (unsigned short *)0xB8000;
int cursor_x = 0;
int cursor_y = 0;
const int VGA_WIDTH = 80;
const int VGA_HEIGHT = 25;

// ...
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(int x, int y) {
	uint16_t pos = y * VGA_WIDTH + x;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void scroll_screen() {
    if (cursor_y >= VGA_HEIGHT) {
        int i;
        for (i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
            video_memory[i] = video_memory[i + VGA_WIDTH];
        }
        for (i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            video_memory[i] = ' ' | (0x0F << 8);
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}

void print_char_at(int x, int y, char c, unsigned char color) {
    const int index = y * VGA_WIDTH + x;
    video_memory[index] = c | (color << 8);
}

void print_char(char c, unsigned char color) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            print_char_at(cursor_x, cursor_y, ' ', color);
        }
    } else {
        print_char_at(cursor_x, cursor_y, c, color);
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    scroll_screen();
    update_cursor(cursor_x, cursor_y);
}

void print_string(const char* str, unsigned char color) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i], color);
    }
}

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video_memory[i] = ' ' | (0x0F << 8);
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor(cursor_x, cursor_y);
}

// change the color of the entire screen ---
void terminal_set_color(uint8_t color) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            const int index = y * VGA_WIDTH + x;

            video_memory[index] = (video_memory[index] & 0xFF) | (color << 8);
        }
    }
}


void kmain(void) {
    clear_screen();
    enable_cursor(14, 15);
    print_string("Welcome to VoltOS! (v1.1 - Color)\n", 0x0A);
    
    gdt_init();
    idt_init();
    timer_init(100);
    keyboard_init();

    print_string("All systems initialized. Starting shell...\n\n", 0x0F);
    
    asm volatile ("sti");

    shell_init();
    
    print_string("\nFATAL: Shell exited!", 0x0C);
    while(1){}
}
