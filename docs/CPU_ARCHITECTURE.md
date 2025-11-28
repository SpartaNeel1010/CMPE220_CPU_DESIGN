# SC8 CPU Architecture

## Overview

The SC8 (Simple CPU 8-bit) is a von Neumann architecture processor with an 8-bit data path and 16-bit address space. It implements a load-store architecture with explicit memory operations and register-based computation.

## CPU Schematic
![SC8 CPU Architecture Diagram](CPU.png)


## Component Descriptions

### 1. Control Unit
The Control Unit orchestrates all CPU operations by:
- Fetching instructions from memory via the Program Counter
- Decoding instructions to determine operations and operands
- Generating control signals for other components
- Managing the instruction execution cycle

**Key Registers:**
- **Instruction Register (IR)**: Holds the current instruction being executed
- **Decoder**: Interprets the opcode and generates appropriate control signals

### 2. Program Counter (PC)
- 16-bit register that holds the address of the next instruction
- Automatically incremented after each instruction fetch
- Can be modified by branch, jump, and call instructions
- Initialized to 0x0100 on reset (start of program memory)

### 3. Register File
Eight 8-bit general-purpose registers (R0-R7):
- **R0-R6**: Available for any operation
- **R7 (SP)**: Designated as Stack Pointer but can be used as general-purpose
- All registers are directly addressable
- Register-to-register operations execute in a single cycle

### 4. Arithmetic Logic Unit (ALU)
Performs all arithmetic and logical operations:

**Arithmetic Operations:**
- Addition (ADD, ADDI, INC)
- Subtraction (SUB, SUBI, DEC)
- Multiplication (MUL) - returns lower 8 bits

**Logical Operations:**
- AND, OR, XOR, NOT
- Shift left (SHL), Shift right (SHR)

**Comparison:**
- CMP, CMPI - updates flags without storing result

**Flag Updates:**
The ALU automatically updates the flags register based on operation results.

### 5. Flags Register
8-bit register containing status flags:
```
Bit 7: N (Negative)  - Result is negative (bit 7 = 1)
Bit 6: Z (Zero)      - Result is zero
Bit 5: C (Carry)     - Carry/borrow occurred
Bit 4: V (Overflow)  - Signed overflow occurred
Bit 3-0: Reserved
```

### 6. System Bus
Connects all components and consists of:
- **Address Bus (16-bit)**: Specifies memory addresses (0x0000-0xFFFF)
- **Data Bus (8-bit)**: Transfers data between components
- **Control Signals**: Read/Write enable, clock, reset, etc.

### 7. Memory Subsystem

**Main Memory (64 KB):**
- 0x0000-0x00FF: System area for interrupt vectors
- 0x0100-0xFEFF: General-purpose RAM (code and data)
- 0xFF00-0xFFFF: Memory-mapped I/O region

**Memory-Mapped I/O:**
Devices mapped into the memory address space:
- **Timer (0xFF00, 0xFF03)**: Hardware timer for delays and timing
- **Console I/O (0xFF01, 0xFF02)**: Character input/output

### 8. Stack Pointer (SP)
- Alias for register R7
- Points to the top of the stack
- Initialized to 0xFEFF (just below I/O region)
- Decrements on PUSH, increments on POP
- Used by CALL/RET for subroutine management

## Data Flow

### Instruction Execution Flow

```
1. FETCH Phase:
   PC ──▶ Address Bus ──▶ Memory ──▶ Data Bus ──▶ IR
   PC = PC + instruction_length

2. DECODE Phase:
   IR ──▶ Decoder ──▶ Control Signals
   Operands extracted from instruction

3. EXECUTE Phase:
   a) Register Operations:
      Register File ──▶ ALU ──▶ Register File
      ALU ──▶ Flags Register

   b) Memory Load:
      Address Bus ──▶ Memory ──▶ Data Bus ──▶ Register File

   c) Memory Store:
      Register File ──▶ Data Bus ──▶ Memory

   d) Branch:
      Flags + Address ──▶ PC
```

### Example: ADD R0, R1, R2

```
Cycle 1 (FETCH):
  - PC (e.g., 0x0100) output to address bus
  - Memory reads instruction bytes at PC
  - Instruction loaded into IR
  - PC incremented by 2

Cycle 2 (DECODE):
  - Control unit decodes opcode (0x00 = ADD)
  - Extracts destination (R0) and sources (R1, R2)
  - Prepares control signals for ALU and register file

Cycle 3 (EXECUTE):
  - R1 and R2 contents read to ALU inputs
  - ALU performs addition
  - Result written to R0
  - Flags updated (Z, N, C, V)
```

### Example: LOAD R0, [0x1000]

```
Cycle 1 (FETCH):
  - PC output to address bus
  - Instruction bytes loaded into IR
  - PC incremented by 3

Cycle 2 (DECODE):
  - Control unit decodes opcode (0x10 = LOAD)
  - Extracts destination register (R0)
  - Extracts memory address (0x1000)

Cycle 3 (EXECUTE):
  - Address 0x1000 output to address bus
  - Memory read control signal activated
  - Data from memory transferred via data bus
  - Data written to R0
```

## Memory-Mapped I/O Operation

### Console Output Example
```assembly
LOADI R0, 'H'           ; Load ASCII 'H' into R0
STORE R0, [0xFF01]      ; Write to console output
```

When storing to 0xFF01:
1. Address bus = 0xFF01
2. Data bus = contents of R0
3. Memory subsystem detects I/O address
4. Character output to console

### Timer Example
```assembly
LOADI R0, 100           ; Load timer value
STORE R0, [0xFF00]      ; Start timer
wait_loop:
LOAD R1, [0xFF03]       ; Read current timer value
CMPI R1, 0              ; Check if timer reached zero
JNZ wait_loop           ; Loop until timer expires
```

## Advantages of This Architecture

1. **Simplicity**: Easy to understand and implement
2. **Flexibility**: 8 general-purpose registers provide good programming flexibility
3. **Memory-Mapped I/O**: Unified approach to I/O using standard memory instructions
4. **Load-Store**: Clean separation between computation and memory access
5. **Flags**: Efficient conditional branching and status checking
6. **Educational Value**: Demonstrates fundamental CPU concepts clearly

## Performance Characteristics

- **Instruction Length**: 1-3 bytes (compact code)
- **Execution Speed**: Most instructions execute in 1-3 cycles
- **Memory Bandwidth**: 8-bit data bus limits to 1 byte per cycle
- **Address Space**: 64KB sufficient for educational programs
- **Register Set**: 8 registers reduce memory traffic

## Implementation Notes

1. **Reset Behavior**: On reset, PC = 0x0100, SP = 0xFEFF, all other registers = 0
2. **Endianness**: Little-endian (least significant byte first)
3. **Stack**: Grows downward (from high to low addresses)
4. **Interrupts**: Not implemented in this version (future enhancement)
5. **Pipeline**: Single-cycle execution (no pipelining)
6. **Clock**: Synchronous design with single clock signal

## Comparison with Other 8-bit CPUs

| Feature | SC8 | 6502 | Z80 | Intel 8080 |
|---------|-----|------|-----|------------|
| Data Width | 8-bit | 8-bit | 8-bit | 8-bit |
| Address Space | 64KB | 64KB | 64KB | 64KB |
| Registers | 8 general | 3 + 2 index | 14 | 7 |
| Stack | Register-based | Hardware | Hardware | Hardware |
| I/O | Memory-mapped | Memory-mapped | Port-mapped | Port-mapped |
| Flags | 4 (NZCV) | 7 | 6 | 5 |

The SC8 is designed for educational clarity rather than performance, making it ideal for learning computer architecture concepts.

