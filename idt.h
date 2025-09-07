#ifndef IDT_H
#define IDT_H

#include <stddef.h>
#include <stdint.h>

struct idt_entry_struct {
    uint16_t base_lo;    // The lower 16 bits of the address to jump to.
    uint16_t sel;        // Kernel segment selector.
    uint8_t  always0;    // must always be zero.
    uint8_t  flags;      // Type and attributes of the gate.
    uint16_t base_hi;    // upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

// Structure for the IDT pointer. This is loaded into the IDT register (LIDT).
struct idt_ptr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;


void idt_init();

extern void irq1(); // Interrupt handler for the keyboard

#endif // IDT_H
