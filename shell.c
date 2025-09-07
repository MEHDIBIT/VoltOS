#include "shell.h"
#include "keyboard.h"
#include "kernel.h"
#include "timer.h"
#include "io.h"

#define MAX_CMD_LEN 100

extern volatile uint32_t tick;

// --- String utility functions ---
int strlen(const char* str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, int n) {
    while (n && *s1 && (*s1 == *s2)) { s1++; s2++; n--; }
    if (n == 0) return 0;
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void reverse(char s[]) {
    int i, j;
    char c;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) { c = s[i]; s[i] = s[j]; s[j] = c; }
}

void itoa(int n, char s[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do { s[i++] = n % 10 + '0'; } while ((n /= 10) > 0);
    if (sign < 0) s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

uint8_t hex_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0xFF;
}


// --- Command Functions ---
void cmd_help() {
    print_string("\nVoltOS Shell v1.4\n", 0x0F);
    print_string(" - help:         Show this help message\n", 0x0F);
    print_string(" - clear:        Clear the screen\n", 0x0F);
    print_string(" - echo [msg]:   Print a message\n", 0x0F);
    print_string(" - time:         Show seconds since boot\n", 0x0F);
    print_string(" - reboot:       Restart the computer\n", 0x0F);
    print_string(" - color [bf]:   Set background (b) and foreground (f) colors\n", 0x0F);
    print_string("   Colors: 0-F (e.g., 'color 1F' for white on blue)\n", 0x07);
}

void cmd_clear() { clear_screen(); }
void cmd_echo(char* message) { print_string("\n", 0x0F); print_string(message, 0x0F); }
void cmd_reboot() { print_string("\nRebooting...", 0x0F); outb(0x64, 0xFE); }

void cmd_time() {
    char time_str[16];
    itoa(tick / 100, time_str);
    print_string("\nSeconds since boot: ", 0x0F);
    print_string(time_str, 0x0F);
}

void cmd_color(char* arg) {
    if (strlen(arg) != 2) {
        print_string("\nInvalid color code. Use two hex digits (e.g., 1F).", 0x0C);
        return;
    }
    uint8_t bg = hex_to_int(arg[0]);
    uint8_t fg = hex_to_int(arg[1]);

    if (bg > 0xF || fg > 0xF) {
        print_string("\nInvalid hex digit. Use 0-9 or A-F.", 0x0C);
        return;
    }
    uint8_t color_attr = (bg << 4) | fg;
    terminal_set_color(color_attr);
}


// --- Shell Logic ---
void process_command(char* buffer) {
    if (strcmp(buffer, "help") == 0) cmd_help();
    else if (strcmp(buffer, "clear") == 0) cmd_clear();
    else if (strcmp(buffer, "time") == 0) cmd_time();
    else if (strcmp(buffer, "reboot") == 0) cmd_reboot();
    else if (strncmp(buffer, "echo ", 5) == 0) cmd_echo(buffer + 5);
    else if (strncmp(buffer, "color ", 6) == 0) cmd_color(buffer + 6);
    else if (buffer[0] != '\0') {
        print_string("\nUnknown command: '", 0x0C);
        print_string(buffer, 0x0C);
        print_string("'", 0x0C);
    }
}

void shell_init() {
    char command_buffer[MAX_CMD_LEN];
    int buffer_index = 0;
    const char heartbeat_chars[] = "|/-\\";
    for(int i=0; i<MAX_CMD_LEN; i++) command_buffer[i] = '\0';
    print_string("> ", 0x0A);

    while (1) {
        char c = keyboard_getchar_nonblock();
        if (c != '\0') {
            if (c == '\n') {
                command_buffer[buffer_index] = '\0';
                process_command(command_buffer);
                buffer_index = 0;
                for(int i=0; i<MAX_CMD_LEN; i++) command_buffer[i] = '\0';
                print_string("\n> ", 0x0A);
            } else if (c == '\b') {
                if (buffer_index > 0) {
                    buffer_index--;
                    command_buffer[buffer_index] = '\0';
                    print_char(c, 0x0F);
                }
            } else {
                if (buffer_index < MAX_CMD_LEN - 1) {
                    command_buffer[buffer_index++] = c;
                    print_char(c, 0x0F);
                }
            }
        }
        print_char_at(79, 0, heartbeat_chars[(tick / 25) % 4], 0x0E);
    }
}
