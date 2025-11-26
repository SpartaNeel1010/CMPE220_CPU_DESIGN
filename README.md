# SC8 Software CPU Design

A complete 8-bit CPU implementation in C++ including an emulator, assembler, and sample programs.

## Team Members

- **Neel Asheshbhai Shah**
- **Vedant Tushar Shah**
- **Aarav Pranav Shah**
- **Harshavardhan Kuruvella**

## Project Overview

This project implements SC8 (Simple CPU 8-bit), a complete educational CPU with:

- **8-bit data width** with 16-bit address space (64KB memory)
- **8 general-purpose registers** (R0-R7, with R7 as stack pointer)
- **20+ instructions** covering arithmetic, logic, memory, and control flow
- **Memory-mapped I/O** for console and timer
- **Load-store architecture** with explicit memory operations
- **4 status flags**: Negative (N), Zero (Z), Carry (C), Overflow (V)

### Components

1. **CPU Emulator** - Full CPU simulation with fetch-decode-execute cycle
2. **Assembler** - Two-pass assembler with label support
3. **Sample Programs**:
   - Timer example (demonstrates fetch/compute/store cycles)
   - Hello, World! (console I/O)
   - Fibonacci sequence calculator

## Architecture

### CPU Components

- **ALU**: Arithmetic and logic operations
- **Control Unit**: Instruction decode and execution
- **Register File**: 8 general-purpose 8-bit registers
- **Memory**: 64KB RAM with memory-mapped I/O
- **Bus**: System bus connecting components

### Memory Map

```
0x0000 - 0x00FF: System area (interrupt vectors)
0x0100 - 0xFEFF: General purpose RAM
0xFF00 - 0xFF00: Timer control register
0xFF01 - 0xFF01: Console output
0xFF02 - 0xFF02: Console input
0xFF03 - 0xFF03: Timer value
0xFF04 - 0xFFFF: Reserved I/O
```

### Instruction Set Highlights

- **Arithmetic**: ADD, ADDI, SUB, SUBI, MUL, INC, DEC
- **Logical**: AND, OR, XOR, NOT, SHL, SHR
- **Memory**: LOAD, STORE, LOADI
- **Control Flow**: JMP, JZ, JNZ, JC, JNC, CALL, RET
- **Comparison**: CMP, CMPI
- **Stack**: PUSH, POP
- **Special**: NOP, HALT

## Quick Start

### Prerequisites

- C++ compiler (g++ or clang++) with C++11 support
- Make utility
- Unix-like environment (Linux, macOS, WSL)

### Building

```bash
# Build everything (emulator, assembler, and programs)
make all

# Or build components individually
make emulator    # Build CPU emulator
make assembler   # Build assembler
make programs    # Assemble sample programs
```

### Running Programs

```bash
# Run sample programs
make run-hello    # Run Hello World
make run-fib      # Run Fibonacci
make run-timer    # Run Timer example

# Run all tests
make test
```

### Debug Mode

Step through programs instruction by instruction:

```bash
make debug-fib    # Debug Fibonacci program
make debug-hello  # Debug Hello World
make debug-timer  # Debug Timer example
```

## Usage

### Using the Assembler

```bash
# Assemble a program
./bin/assembler programs/my_program.asm programs/my_program.bin

# The assembler performs:
# 1. Tokenization (lexical analysis)
# 2. Parsing (syntax analysis and label collection)
# 3. Code generation (binary output)
```

### Using the Emulator

```bash
# Run a program
./bin/emulator programs/my_program.bin

# With debug mode (step-by-step)
./bin/emulator -d programs/my_program.bin

# With memory dump
./bin/emulator -m programs/my_program.bin

# Custom start address
./bin/emulator -s 0x0200 programs/my_program.bin
```

## Writing Assembly Programs

### Example: Simple Addition

```asm
; Add two numbers
start:
    LOADI R0, 5      ; R0 = 5
    LOADI R1, 10     ; R1 = 10
    ADD R2, R0, R1   ; R2 = R0 + R1
    STORE R2, [0x1000]  ; Store result
    HALT
```

### Example: Loop

```asm
; Count from 0 to 10
    LOADI R0, 0      ; Counter
    LOADI R1, 10     ; Limit
loop:
    INC R0           ; Increment
    CMP R0, R1       ; Compare
    JNZ loop         ; Loop if not equal
    HALT
```

### Example: Function Call

```asm
main:
    LOADI R0, 5
    CALL double      ; Call function
    HALT

double:
    ADD R0, R0, R0   ; R0 = R0 * 2
    RET
```

## Project Structure

```
CMPE220_project/
├── README.md                   # This file
├── Makefile                    # Build system
├── docs/                       # Documentation
│   ├── ISA_SPECIFICATION.md    # Complete ISA reference
│   ├── CPU_ARCHITECTURE.md     # Architecture details
│   ├── CPU_SCHEMATIC.png       # CPU diagram
│   └── DEMO_SCRIPT.md          # Video demo script
├── src/                        # Source code
│   ├── emulator/               # CPU emulator
│   │   ├── main.cpp            # Emulator entry point
│   │   ├── cpu.h/cpp           # CPU implementation
│   │   ├── alu.h/cpp           # ALU operations
│   │   ├── memory.h/cpp        # Memory system
│   │   ├── bus.h/cpp           # System bus
│   │   └── control_unit.h/cpp  # Control unit
│   └── assembler/              # Assembler
│       ├── main.cpp            # Assembler entry point
│       ├── assembler.h/cpp     # Main assembler
│       ├── lexer.h/cpp         # Tokenizer
│       ├── parser.h/cpp        # Parser
│       └── symbol_table.h/cpp  # Label management
├── programs/                   # Sample programs
│   ├── timer.asm               # Timer demo
│   ├── hello_world.asm         # Hello World
│   └── fibonacci.asm           # Fibonacci sequence
├── report/                     # Project report
│   └── PROJECT_REPORT.docx     # Final report
└── bin/                        # Build output (generated)
    ├── emulator                # Emulator binary
    └── assembler               # Assembler binary
```

## Documentation

- **[ISA Specification](docs/ISA_SPECIFICATION.md)** - Complete instruction set reference
- **[CPU Architecture](docs/CPU_ARCHITECTURE.md)** - Detailed architecture documentation
- **[Demo Script](docs/DEMO_SCRIPT.md)** - Walkthrough for Fibonacci program

## Features

### CPU Emulator

- ✓ Complete fetch-decode-execute cycle
- ✓ 8-bit data path with 16-bit addressing
- ✓ Flag-based conditional branching
- ✓ Stack operations for subroutines
- ✓ Memory-mapped I/O (console, timer)
- ✓ Debug mode with step-by-step execution
- ✓ Memory dump functionality
- ✓ Cycle counting

### Assembler

- ✓ Two-pass assembly (label resolution)
- ✓ Support for decimal, hex, and binary literals
- ✓ Label support for jumps and branches
- ✓ Comments and blank lines
- ✓ Error reporting with line numbers
- ✓ Symbolic register names (R0-R7, SP)

### Sample Programs

- ✓ **Timer**: Demonstrates memory-mapped I/O and fetch/compute/store cycles
- ✓ **Hello World**: Basic console output
- ✓ **Fibonacci**: Loops, arithmetic, and conditional branches

## Technical Details

### Instruction Formats

1. **Register-Register (RR)**: 2 bytes
2. **Register-Immediate (RI)**: 2 bytes
3. **Memory (MEM)**: 3 bytes
4. **Branch (BR)**: 3 bytes
5. **Single Operand (SO)**: 1 byte

### Flag Behavior

- **N (Negative)**: Set when result bit 7 is 1
- **Z (Zero)**: Set when result is zero
- **C (Carry)**: Set on carry/borrow in arithmetic, or shift operations
- **V (Overflow)**: Set on signed arithmetic overflow

### Execution Cycle

Each instruction executes in three phases:

1. **FETCH**: Read instruction from memory at PC, increment PC
2. **DECODE**: Determine operation and extract operands
3. **EXECUTE**: Perform operation, update registers/flags

## Testing

The project includes comprehensive test programs:

```bash
# Run all tests
make test

# This will:
# 1. Build emulator and assembler
# 2. Assemble all programs
# 3. Run each program
# 4. Display output
```

Expected outputs:
- **Hello World**: "Hello, World!"
- **Fibonacci**: "Fib: 0 1 1 2 3 5 8 ## ## ## Done!"
- **Timer**: "1\n2\n3\n4\n5\nDone\n"

## Cleaning Up

```bash
# Remove all build artifacts
make clean
```

## Troubleshooting

### Build Issues

- Ensure g++ is installed: `g++ --version`
- Check C++11 support: The project requires `-std=c++11`
- For macOS users: May need to install Xcode Command Line Tools

### Runtime Issues

- **Program doesn't halt**: Check for infinite loops in assembly
- **Unexpected output**: Use debug mode (`-d` flag) to step through
- **Segmentation fault**: Check memory addresses are within bounds

## Future Enhancements

Potential improvements:
- Interrupt support
- More memory-mapped devices
- Pipelining simulation
- More sophisticated assembler (macros, includes)
- Disassembler
- Interactive debugger with breakpoints

## References

- [ISA Specification](docs/ISA_SPECIFICATION.md)
- [CPU Architecture](docs/CPU_ARCHITECTURE.md)
- [Demo Video Script](docs/DEMO_SCRIPT.md)

## License

Educational project for CMPE220 - Computer Architecture

## Contact

For questions or issues, contact team members through the course portal.

---

**Project completed as part of CMPE220 Computer Architecture Course**

