#ifndef IO_H
#define IO_H



// Reads a byte from the specified I/O port.
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    
    asm volatile ("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}


static inline void outb(unsigned short port, unsigned char data) {
    
    asm volatile ("outb %0, %1" : : "a"(data), "dN"(port));
}

#endif // IO_H
