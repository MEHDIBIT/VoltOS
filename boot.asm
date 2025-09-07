; boot.asm

bits 32

section .text
    ; Multiboot header
    align 4
    dd 0x1BADB002
    dd 0x00
    dd - (0x1BADB002 + 0x00)

global _start
extern kmain

_start:
    cli
    mov esp, stack_space
    call kmain
    hlt

global gdt_flush
gdt_flush:
    mov eax, [esp+4]
    lgdt [eax]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush
.flush:
    ret

%macro ISR_STUB 1
  global isr%1
  isr%1:
    cli
    push    0
    push    %1
    jmp     isr_common_stub
%endmacro

%macro IRQ_STUB 2
  global irq%1
  irq%1:
    cli
    push    0
    push    %2
    jmp     irq_common_stub
%endmacro

extern isr_handler

isr_common_stub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    call isr_handler
    add esp, 4
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8
    iret

irq_common_stub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    call isr_handler
    add esp, 4
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa

    mov edx, [esp+4]
    cmp edx, 40
    jl .master_eoi

    mov al, 0x20
    out 0xA0, al
.master_eoi:
    mov al, 0x20
    out 0x20, al

    add esp, 8
    iret

ISR_STUB 0
ISR_STUB 1
ISR_STUB 2
ISR_STUB 3
ISR_STUB 4
ISR_STUB 5
ISR_STUB 6
ISR_STUB 7
ISR_STUB 8
ISR_STUB 9
ISR_STUB 10
ISR_STUB 11
ISR_STUB 12
ISR_STUB 13
ISR_STUB 14
ISR_STUB 15
ISR_STUB 16
ISR_STUB 17
ISR_STUB 18
ISR_STUB 19
ISR_STUB 20
ISR_STUB 21
ISR_STUB 22
ISR_STUB 23
ISR_STUB 24
ISR_STUB 25
ISR_STUB 26
ISR_STUB 27
ISR_STUB 28
ISR_STUB 29
ISR_STUB 30
ISR_STUB 31

IRQ_STUB 0, 32
IRQ_STUB 1, 33
IRQ_STUB 2, 34
IRQ_STUB 3, 35
IRQ_STUB 4, 36
IRQ_STUB 5, 37
IRQ_STUB 6, 38
IRQ_STUB 7, 39
IRQ_STUB 8, 40
IRQ_STUB 9, 41
IRQ_STUB 10, 42
IRQ_STUB 11, 43
IRQ_STUB 12, 44
IRQ_STUB 13, 45
IRQ_STUB 14, 46
IRQ_STUB 15, 47

section .bss
resb 8192
stack_space:
