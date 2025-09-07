#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>


// VGA text-mode driver functions
void print_char(char c, unsigned char color);
void print_string(const char* str, unsigned char color);
void clear_screen();
void print_char_at(int x, int y, char c, unsigned char color);

void terminal_set_color(uint8_t color);

#endif // KERNEL_H
