#include "keyboard.h"
#include "isr.h"
#include "io.h"
#include "kernel.h"

// Scancode set 1 for US QWERTY layout.
unsigned char scancode_to_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0,
    ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+',
};

// --- simple keyboard buffer ---
#define KBD_BUFFER_SIZE 256
volatile char kbd_buffer[KBD_BUFFER_SIZE];
volatile int kbd_buffer_read_idx = 0;
volatile int kbd_buffer_write_idx = 0;

// C-level handler for keyboard interrupts.
static void keyboard_callback(registers_t *regs) {
    (void)regs;
    unsigned char scancode = inb(0x60);

    if (!(scancode & 0x80)) {
        char c = scancode_to_ascii[scancode];
        if (c) {
            kbd_buffer[kbd_buffer_write_idx] = c;
            kbd_buffer_write_idx = (kbd_buffer_write_idx + 1) % KBD_BUFFER_SIZE;
        }
    }
}


char keyboard_getchar_nonblock() {
    if (kbd_buffer_read_idx == kbd_buffer_write_idx) {
        return '\0'; // Buffer is empty
    }

    asm volatile("cli");
    char c = kbd_buffer[kbd_buffer_read_idx];
    kbd_buffer_read_idx = (kbd_buffer_read_idx + 1) % KBD_BUFFER_SIZE;
    asm volatile("sti");
    
    return c;
}

void keyboard_init() {
    for (int i = 0; i < KBD_BUFFER_SIZE; i++) {
        kbd_buffer[i] = '\0';
    }
    kbd_buffer_read_idx = 0;
    kbd_buffer_write_idx = 0;

    register_interrupt_handler(33, keyboard_callback);
}
