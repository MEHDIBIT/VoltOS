# **VoltOS: A 32-bit Operating System from Scratch**
### **VoltOS is a custom-built, 32-bit operating system designed from the ground up to demonstrate and explore fundamental concepts in computer architecture and systems programming. It features a monolithic kernel, a complete interrupt handling system, direct hardware control via I/O ports, and an interactive command-line shell.**

The entire project is written in C and x86 Assembly, with no external dependencies beyond the build toolchain.

**Architectural Highlights**
The foundation of VoltOS is built on professional OS design principles to ensure stability and direct control over the virtual machine.

Custom Memory Map (GDT): VoltOS establishes its own Global Descriptor Table, taking full control over the CPU's memory segmentation and defining its own rules for kernel-level code and data access. This is a critical feature for system stability and a major step beyond relying on the initial bootloader's setup.

Robust Interrupt Handling (IDT): The OS implements a full Interrupt Descriptor Table, allowing it to gracefully handle two types of events without crashing:

Hardware Interrupts (IRQs): Signals from hardware like the system timer (PIT) and keyboard.

CPU Exceptions (ISRs): Critical errors detected by the processor itself.
This unified interrupt model ensures system-wide stability.

Hardware Abstraction: VoltOS communicates directly with legacy hardware, including the VGA text-mode buffer, the Programmable Interval Timer (PIT), and the PS/2 keyboard controller.

Shell Interface
The primary user interface is a custom-built, interactive command-line shell that provides a rich user experience.

Interactive Prompt & Parser: The shell reads and parses user input, distinguishing between simple commands and commands with arguments.

**Built-in Commands:**

`help` : Displays a detailed help menu.

`clear` : Clears all text from the screen.

`echo [message]` : Prints any given message back to the terminal.

`time` : Calculates and displays the uptime of the OS in seconds.

`color [bg][fg]` : Changes the background (bg) and foreground (fg) colors for the entire terminal using hexadecimal color codes.

`reboot` : Sends a command directly to the keyboard controller (port 0x64) to trigger a CPU reset.

## **Getting Started**
**Prerequisites**
To build and run VoltOS, a standard Linux-style build environment and the following tools are required:

```
• make

• nasm (The Netwide Assembler)

• gcc (a 32-bit x86 cross-compiler, e.g., i686-elf-gcc)

• qemu-system-i386

• xorriso
```

## **Installation**
### **On Debian / Ubuntu / WSL**

```
sudo apt update
sudo apt install build-essential nasm qemu-system-x86 xorriso

```

(Note: The default gcc on a 64-bit system may require a dedicated cross-compiler for best results, but can often build this simple kernel.)

### **On macOS (using Homebrew)**

```
brew install nasm qemu
# A cross-compiler is highly recommended
brew install i386-elf-gcc

```

### **On Windows**
***The recommended method is to use the Windows Subsystem for Linux (WSL).***

```
Install WSL from the Microsoft Store or PowerShell.

Install a Linux distribution (e.g., Ubuntu).

Open your WSL terminal and follow the Debian/Ubuntu instructions above.

```

## Building and Running the OS
### 1. Clone the repository:

• `git clone [https://github.com/MEHDIBIT/VoltOS.git](https://github.com/MEHDIBIT/VoltOS.git)`

### 2. Navigate to the project directory:

• `cd VoltOS`

### 3. Clean previous builds (optional):

• `make clean`

### 4. Compile the OS:

• `make`

*This will assemble the bootloader, compile the C kernel and drivers, and link them into a single kernel.bin file. It then uses xorriso to create a bootable VoltOS.iso file.*

### 5. Run in QEMU:

• `make run`

*This command will launch QEMU and boot the newly created ISO file.*
