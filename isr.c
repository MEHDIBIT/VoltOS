#include "isr.h"
#include "kernel.h" 




isr_t interrupt_handlers[256];


void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}


void isr_handler(registers_t *regs)
{
   
    if (interrupt_handlers[regs->int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs->int_no];
        handler(regs);
    }
   
    else if (regs->int_no < 32)
    {
       
        print_string("\nUnhandled Exception: ", 0x0C); // Light Red
       
        print_string("Halting System.", 0x0C);
        for (;;); 
    }
}
