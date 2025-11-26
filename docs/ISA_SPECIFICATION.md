# ISA Specification - SC8 (Simple CPU 8-bit)

## Overview

The SC8 is an 8-bit CPU with a 16-bit address space, providing 64KB of addressable memory. It features a load-store architecture with 8 general-purpose registers and a rich instruction set suitable for educational purposes.

## Register Architecture

### General Purpose Registers
- **R0-R6**: 8-bit general-purpose registers
- **R7 (SP)**: Stack Pointer (also usable as general-purpose register)

### Special Registers
- **PC**: 16-bit Program Counter
- **FLAGS**: 8-bit Status Register

### Flag Register (FLAGS)
```
Bit 7: N (Negative) - Set if result is negative (bit 7 = 1)
Bit 6: Z (Zero)     - Set if result is zero
Bit 5: C (Carry)    - Set if operation produces carry/borrow
Bit 4: V (Overflow) - Set if signed overflow occurs
Bit 3-0: Reserved (always 0)
```

## Memory Map

```
0x0000 - 0x00FF: Interrupt vectors and system area
0x0100 - 0xFEFF: General purpose RAM (65024 bytes)
0xFF00 - 0xFFFF: Memory-mapped I/O (256 bytes)
```

### Memory-Mapped I/O
```
0xFF00: TIMER_CTRL  - Timer control register
0xFF01: CONSOLE_OUT - Console output (write character)
0xFF02: CONSOLE_IN  - Console input (read character)
0xFF03: TIMER_VALUE - Timer current value
0xFF04-0xFFFF: Reserved for future I/O
```

## Instruction Format

The SC8 uses three instruction format types:

### Format 1: Register-Register (RR)
```
Byte 0: [OPCODE (5 bits)] [RD (3 bits)]
Byte 1: [RS1 (3 bits)] [RS2 (3 bits)] [unused (2 bits)]
Total: 2 bytes
```

### Format 2: Register-Immediate (RI)
```
Byte 0: [OPCODE (5 bits)] [RD (3 bits)]
Byte 1: [IMMEDIATE (8 bits)]
Total: 2 bytes
```

### Format 3: Memory (MEM)
```
Byte 0: [OPCODE (5 bits)] [RD (3 bits)]
Byte 1: [ADDRESS_LOW (8 bits)]
Byte 2: [ADDRESS_HIGH (8 bits)]
Total: 3 bytes
```

### Format 4: Branch (BR)
```
Byte 0: [OPCODE (5 bits)] [CONDITION (3 bits)]
Byte 1: [OFFSET_LOW (8 bits)]
Byte 2: [OFFSET_HIGH (8 bits)]
Total: 3 bytes
```

### Format 5: Single Operand (SO)
```
Byte 0: [OPCODE (5 bits)] [RD (3 bits)]
Total: 1 byte
```

## Instruction Set

### Arithmetic Instructions (Format RR/RI)

| Mnemonic | Opcode | Format | Description | Flags |
|----------|--------|--------|-------------|-------|
| ADD Rd, Rs1, Rs2 | 0x00 | RR | Rd = Rs1 + Rs2 | N,Z,C,V |
| ADDI Rd, Rs, imm | 0x01 | RI | Rd = Rs + imm | N,Z,C,V |
| SUB Rd, Rs1, Rs2 | 0x02 | RR | Rd = Rs1 - Rs2 | N,Z,C,V |
| SUBI Rd, Rs, imm | 0x03 | RI | Rd = Rs - imm | N,Z,C,V |
| MUL Rd, Rs1, Rs2 | 0x04 | RR | Rd = Rs1 * Rs2 (lower 8 bits) | N,Z |
| INC Rd | 0x05 | SO | Rd = Rd + 1 | N,Z,C,V |
| DEC Rd | 0x06 | SO | Rd = Rd - 1 | N,Z,C,V |

### Logical Instructions (Format RR/RI)

| Mnemonic | Opcode | Format | Description | Flags |
|----------|--------|--------|-------------|-------|
| AND Rd, Rs1, Rs2 | 0x07 | RR | Rd = Rs1 & Rs2 | N,Z |
| ANDI Rd, Rs, imm | 0x08 | RI | Rd = Rs & imm | N,Z |
| OR Rd, Rs1, Rs2 | 0x09 | RR | Rd = Rs1 \| Rs2 | N,Z |
| ORI Rd, Rs, imm | 0x0A | RI | Rd = Rs \| imm | N,Z |
| XOR Rd, Rs1, Rs2 | 0x0B | RR | Rd = Rs1 ^ Rs2 | N,Z |
| NOT Rd, Rs | 0x0C | RR | Rd = ~Rs | N,Z |
| SHL Rd, Rs | 0x0D | RR | Rd = Rd << Rs | N,Z,C |
| SHR Rd, Rs | 0x0E | RR | Rd = Rd >> Rs | N,Z,C |

### Memory Instructions (Format MEM)

| Mnemonic | Opcode | Format | Description | Flags |
|----------|--------|--------|-------------|-------|
| LOAD Rd, [addr] | 0x10 | MEM | Rd = Memory[addr] | - |
| STORE Rs, [addr] | 0x11 | MEM | Memory[addr] = Rs | - |
| LOADI Rd, imm | 0x12 | RI | Rd = imm | - |

### Control Flow Instructions (Format BR)

| Mnemonic | Opcode | Format | Description | Flags |
|----------|--------|--------|-------------|-------|
| JMP addr | 0x18 | BR | PC = addr | - |
| JZ addr | 0x19 | BR | if Z=1 then PC = addr | - |
| JNZ addr | 0x1A | BR | if Z=0 then PC = addr | - |
| JC addr | 0x1B | BR | if C=1 then PC = addr | - |
| JNC addr | 0x1C | BR | if C=0 then PC = addr | - |
| CALL addr | 0x1D | BR | Push PC; PC = addr | - |
| RET | 0x1E | SO | PC = Pop() | - |

### Comparison Instructions (Format RR/RI)

| Mnemonic | Opcode | Format | Description | Flags |
|----------|--------|--------|-------------|-------|
| CMP Rs1, Rs2 | 0x13 | RR | Flags = Rs1 - Rs2 | N,Z,C,V |
| CMPI Rs, imm | 0x14 | RI | Flags = Rs - imm | N,Z,C,V |

### Stack Instructions (Format SO)

| Mnemonic | Opcode | Format | Description | Flags |
|----------|--------|--------|-------------|-------|
| PUSH Rs | 0x15 | SO | [--SP] = Rs | - |
| POP Rd | 0x16 | SO | Rd = [SP++] | - |

### Special Instructions (Format SO)

| Mnemonic | Opcode | Format | Description | Flags |
|----------|--------|--------|-------------|-------|
| NOP | 0xFF | SO | No operation (encoded as 0xFF) | - |
| HALT | 0xF8 | SO | Stop execution (encoded as 0xF8) | - |

**Note on NOP Encoding:** NOP is encoded as 0xFF (all bits set) instead of 0x00 to avoid conflict with ADD R0, R0, R0 which would also encode to 0x00 as its first byte. This ensures unambiguous instruction decoding.

## Addressing Modes

### 1. Immediate Addressing
The operand is specified directly in the instruction.
```
LOADI R0, 42    ; R0 = 42
ADDI R1, R0, 10 ; R1 = R0 + 10
```

### 2. Register Addressing
The operand is in a register.
```
ADD R0, R1, R2  ; R0 = R1 + R2
```

### 3. Direct Addressing
The address of the operand is specified in the instruction.
```
LOAD R0, [0x1000]  ; R0 = Memory[0x1000]
STORE R1, [0x2000] ; Memory[0x2000] = R1
```

### 4. Implied Addressing
The operand is implicit in the instruction.
```
NOP             ; No operands
HALT            ; No operands
RET             ; Return address from stack
```

## Flag Semantics

### Zero Flag (Z)
- Set (1) when the result of an operation is zero
- Cleared (0) when the result is non-zero

### Negative Flag (N)
- Set (1) when bit 7 of the result is 1 (negative in two's complement)
- Cleared (0) when bit 7 of the result is 0

### Carry Flag (C)
- Set (1) when an addition produces a carry out of bit 7
- Set (1) when a subtraction requires a borrow
- Set (1) when a shift operation shifts out a 1
- Cleared (0) otherwise

### Overflow Flag (V)
- Set (1) when a signed arithmetic operation produces a result outside the valid range (-128 to +127)
- Cleared (0) otherwise
- Computed as: V = (Carry into bit 7) XOR (Carry out of bit 7)

## Instruction Encoding Examples

### Example 1: LOADI R0, 42
```
Byte 0: [0x12 << 3 | 0] = 0x90
Byte 1: 42 = 0x2A
Binary: 10010000 00101010
```

### Example 2: ADD R3, R1, R2
```
Byte 0: [0x00 << 3 | 3] = 0x03
Byte 1: [1 << 5 | 2 << 2] = 0x28
Binary: 00000011 00101000
```

### Example 3: LOAD R5, [0xFF01]
```
Byte 0: [0x10 << 3 | 5] = 0x85
Byte 1: 0x01 (low byte)
Byte 2: 0xFF (high byte)
Binary: 10000101 00000001 11111111
```

### Example 4: JZ label (offset = 0x0010)
```
Byte 0: [0x19 << 3 | 0] = 0xC8
Byte 1: 0x10 (low byte)
Byte 2: 0x00 (high byte)
Binary: 11001000 00010000 00000000
```

## Execution Cycle

Each instruction executes in three phases:

### 1. FETCH
- Read instruction from memory at PC
- Increment PC by instruction length

### 2. DECODE
- Determine instruction type from opcode
- Extract operands (registers, immediate values, addresses)

### 3. EXECUTE
- Perform the operation
- Update registers and flags
- Handle memory access or control flow

## Assembly Language Syntax

### Comments
```
; This is a single-line comment
```

### Labels
```
start:          ; Label definition
    LOADI R0, 0
loop:
    INC R0
    JMP loop
```

### Numeric Literals
```
LOADI R0, 42    ; Decimal
LOADI R1, 0x2A  ; Hexadecimal
LOADI R2, 0b00101010  ; Binary
```

### Instructions
```
; Arithmetic
ADD R0, R1, R2
ADDI R3, R0, 10

; Memory
LOAD R0, [0x1000]
STORE R1, [0x2000]

; Control flow
JMP start
JZ zero_label
CALL function
RET

; Stack
PUSH R0
POP R1
```

## Example Programs

### Example 1: Simple Addition
```asm
; Add two numbers and store result
    LOADI R0, 5      ; R0 = 5
    LOADI R1, 10     ; R1 = 10
    ADD R2, R0, R1   ; R2 = R0 + R1 = 15
    STORE R2, [0x1000]  ; Store result
    HALT
```

### Example 2: Loop Counter
```asm
; Count from 0 to 10
    LOADI R0, 0      ; Counter
    LOADI R1, 10     ; Limit
loop:
    INC R0           ; R0++
    CMP R0, R1       ; Compare R0 with R1
    JNZ loop         ; Loop if not equal
    HALT
```

### Example 3: Function Call
```asm
; Main program
    LOADI R0, 5
    CALL multiply_by_2
    HALT

multiply_by_2:
    ADD R0, R0, R0   ; R0 = R0 * 2
    RET
```

## Notes

1. All multi-byte values are stored in little-endian format (low byte first)
2. The stack grows downward (from high to low addresses)
3. SP should be initialized to 0xFEFF before using stack operations
4. Memory-mapped I/O responds immediately to read/write operations
5. HALT instruction stops the CPU; execution cannot resume without reset

