ENTRY(_start) /* The entry point of the OS is the _start label in boot.asm */

SECTIONS {
    /* Start at 1MB. The bootloader, BIOS, and VGA memory are below this. */
    . = 1M;

    .text : {
        *(.text) /* Put all the .text (code) sections here. */
    }

    .rodata : {
        *(.rodata) /* Put all the .rodata (read-only data) sections here. */
    }

    .data : {
        *(.data) /* Put all the .data (initialized data) sections here. */
    }

    .bss : {
        *(.bss) /* Put all the .bss (uninitialized data) sections here. */
    }
}
