# SC8 CPU - Fibonacci Program Demo Script

This document provides a detailed walkthrough of the Fibonacci sequence program for the demo video. Use this script to explain how the program works, demonstrating the fetch-decode-execute cycle and CPU operation.

## Video Structure

1. **Introduction** (30 seconds)
2. **Architecture Overview** (1 minute)
3. **Fibonacci Program Walkthrough** (3-4 minutes)
4. **Execution Demo** (2 minutes)
5. **Conclusion** (30 seconds)

---

## 1. Introduction (30 seconds)

**[Show title slide: "SC8 Software CPU Design - Fibonacci Demo"]**

*"Hello! Today we're demonstrating the SC8, an 8-bit educational CPU we designed and implemented in C++. We'll walk through how our Fibonacci sequence program works, showing the fetch-decode-execute cycle in action."*

**[Show team member names]**

*"This project was created by Neel Asheshbhai Shah, Vedant Tushar Shah, Aarav Pranav Shah, and Harshavardhan Kuruvella."*

---

## 2. Architecture Overview (1 minute)

**[Show CPU architecture diagram]**

*"The SC8 is an 8-bit CPU with a 16-bit address space. Let me highlight the key components:"*

**Point to each component:**

- **"8 General-Purpose Registers (R0-R7)"** - *"These store our working data. R7 doubles as the stack pointer."*

- **"ALU (Arithmetic Logic Unit)"** - *"Performs arithmetic and logical operations, updating our status flags."*

- **"Control Unit"** - *"Orchestrates the fetch-decode-execute cycle for each instruction."*

- **"Memory (64KB)"** - *"Stores both our program and data. The top 256 bytes are reserved for memory-mapped I/O."*

- **"Flags Register"** - *"Four flags: Negative, Zero, Carry, and Overflow. These control conditional branches."*

**[Show memory map]**

*"Our memory map shows programs start at address 0x0100. Memory-mapped I/O at 0xFF00 includes a console output register at 0xFF01 for displaying results."*

---

## 3. Fibonacci Program Walkthrough (3-4 minutes)

**[Open fibonacci.asm in editor with line numbers visible]**

### Program Structure

*"Let's walk through our Fibonacci program. The Fibonacci sequence is: 0, 1, 1, 2, 3, 5, 8, 13, and so on. Each number is the sum of the previous two."*

### Initialization Section

**[Highlight lines for initialization]**

```asm
start:
    LOADI R0, 0         ; R0 = fib(n-2) = 0
    LOADI R1, 1         ; R1 = fib(n-1) = 1
    LOADI R2, 0         ; R2 = fib(n) = current value
    LOADI R3, 10        ; R3 = counter (calculate 10 numbers)
    LOADI R4, 0         ; R4 = current count
```

*"First, we initialize our registers:"*
- *"R0 holds fib(n-2), starting at 0"*
- *"R1 holds fib(n-1), starting at 1"*
- *"R2 will hold our current Fibonacci number"*
- *"R3 is our limit - we'll calculate 10 numbers"*
- *"R4 tracks how many numbers we've calculated"*

### Fetch-Decode-Execute Example

*"Let's examine one instruction in detail to see the fetch-decode-execute cycle:"*

**[Highlight: LOADI R0, 0]**

*"For LOADI R0, 0:"*

1. **FETCH Phase:**
   - *"The Program Counter points to address 0x0100"*
   - *"The Control Unit reads the instruction bytes from memory"*
   - *"The instruction is loaded into the Instruction Register"*
   - *"PC increments by 2 (this is a 2-byte instruction)"*

2. **DECODE Phase:**
   - *"The Control Unit decodes the opcode: 0x12 means LOADI"*
   - *"Destination register: R0 (bits 0-2 of first byte)"*
   - *"Immediate value: 0 (second byte)"*

3. **EXECUTE Phase:**
   - *"The value 0 is written to R0"*
   - *"No ALU operation needed, no flags updated"*

**[Show register state diagram: R0=0]**

### Main Loop

**[Highlight the fib_loop section]**

```asm
fib_loop:
    CMP R4, R3          ; Compare count with limit
    JZ done             ; If equal, we're done
    
    ADD R2, R0, R1      ; R2 = R0 + R1
```

*"The main loop has three key parts:"*

#### 1. Loop Condition Check

**[Highlight: CMP R4, R3]**

*"CMP compares our counter (R4) with the limit (R3):"*
- **FETCH**: Read CMP instruction
- **DECODE**: Opcode 0x13, operands R4 and R3
- **EXECUTE**: 
  - *"ALU performs R4 - R3"*
  - *"Result is discarded, but flags are updated"*
  - *"If R4 equals R3, the Zero flag (Z) is set"*

**[Show flags register: Z=0 initially, Z=1 when done]**

**[Highlight: JZ done]**

*"JZ is 'Jump if Zero'. If the Z flag is set, PC jumps to the 'done' label. Otherwise, execution continues."*

#### 2. Fibonacci Calculation

**[Highlight: ADD R2, R0, R1]**

*"This is where we calculate the next Fibonacci number:"*

**Step through with example values (iteration 3: fib(2) = 1):**

*"Assume R0=0, R1=1:"*

- **FETCH**: Read ADD instruction from memory
- **DECODE**: Opcode 0x00, Rd=R2, Rs1=R0, Rs2=R1
- **EXECUTE**:
  - *"ALU receives values: 0 and 1"*
  - *"ALU performs addition: 0 + 1 = 1"*
  - *"Result (1) written to R2"*
  - *"Flags updated: N=0, Z=0, C=0, V=0"*

**[Show register state: R0=0, R1=1, R2=1]**

#### 3. Output Section

**[Highlight the output code]**

```asm
single_digit:
    LOADI R5, 48        ; ASCII '0'
    ADD R5, R5, R2      ; R5 = '0' + number
    STORE R5, [0xFF01]  ; Output digit
```

*"To display the number:"*

1. *"Load ASCII value of '0' (48) into R5"*
2. *"Add our Fibonacci number to convert to ASCII digit"*
3. *"Store to address 0xFF01 - the memory-mapped console output"*

**[Highlight: STORE R5, [0xFF01]]**

*"The STORE instruction to 0xFF01 is special:"*
- **FETCH**: Read STORE instruction
- **DECODE**: Opcode 0x11, Rs=R5, Address=0xFF01
- **EXECUTE**:
  - *"Address 0xFF01 goes on the address bus"*
  - *"R5 contents (ASCII character) goes on data bus"*
  - *"Memory subsystem detects I/O address"*
  - *"Character appears on console output"*

**[Show data flow: R5 → Bus → Memory-Mapped I/O → Console]**

#### 4. State Update

**[Highlight the state update code]**

```asm
    LOADI R0, 0
    ADD R0, R0, R1      ; R0 = R1
    LOADI R1, 0
    ADD R1, R1, R2      ; R1 = R2
    INC R4              ; R4++
    JMP fib_loop
```

*"Before the next iteration, we update our state:"*
- *"R0 ← R1 (shift fib(n-1) to fib(n-2))"*
- *"R1 ← R2 (shift fib(n) to fib(n-1))"*
- *"R4++ (increment counter)"*
- *"JMP back to loop start"*

### Complete Iteration Example

**[Show table with register values through iterations]**

```
Iteration | R0  | R1  | R2  | R4  | Output
----------|-----|-----|-----|-----|--------
0         | 0   | 1   | 0   | 0   | '0'
1         | 1   | 0   | 1   | 1   | '1'
2         | 0   | 1   | 1   | 2   | '1'
3         | 1   | 1   | 2   | 3   | '2'
4         | 1   | 2   | 3   | 4   | '3'
5         | 2   | 3   | 5   | 5   | '5'
```

*"Notice how each row shows the state after one iteration. R0 and R1 shift forward, and R2 contains their sum."*

### Program Termination

**[Highlight the done section]**

```asm
done:
    LOADI R5, 10        ; newline
    STORE R5, [0xFF01]
    ; ... output "Done!" message ...
    HALT
```

*"When R4 reaches 10, we:"*
1. *"Output a newline"*
2. *"Display 'Done!'"*
3. *"Execute HALT, which stops the CPU"*

---

## 4. Execution Demo (2 minutes)

**[Switch to terminal]**

### Build Everything

```bash
make all
```

*"First, we build the emulator and assembler, and assemble our programs."*

**[Show build output]**

### Run Fibonacci Program

```bash
make run-fib
```

*"Now let's run the Fibonacci program:"*

**[Show output]**

```
SC8 CPU Emulator
Program: programs/fibonacci.bin
Size: XX bytes
Start address: 0x0100
Debug mode: OFF

Starting CPU execution at PC=0x0100
Fib: 0 1 1 2 3 5 8 ## ## ## Done!

CPU halted after XXX cycles

=== Final CPU State ===
Registers: R0=0xXX R1=0xXX R2=0xXX R3=0x0A R4=0x0A ...
PC=0xXXXX Flags=[----] Cycles=XXX

Execution completed successfully.
```

*"Success! Our program calculated the first 10 Fibonacci numbers. Notice the ## symbols - those represent numbers 10 and above that don't fit in a single digit."*

### Optional: Debug Mode Demo

**[If time permits]**

```bash
make debug-fib
```

*"We can also run in debug mode to step through each instruction:"*

**[Show first few steps]**

```
[FETCH] PC=0x0100 IR[0]=0x90
[EXECUTE] Opcode=0x12 LOADI R0, 0
Registers: R0=0x00 R1=0x00 ...
PC=0x0102 Flags=[--Z-] Cycles=1

--- Press Enter to execute next instruction ---
```

*"This shows the fetch-decode-execute cycle in action. We can see registers and flags update after each instruction."*

### Memory Dump

**[If time permits]**

```bash
./bin/emulator -m programs/fibonacci.bin
```

*"With the memory dump option, we can see the program in memory and any data it produced."*

---

## 5. Conclusion (30 seconds)

**[Back to slides or face camera]**

*"In summary, we've built a complete CPU from scratch:"*

- ✓ *"Designed a custom ISA with 20+ instructions"*
- ✓ *"Implemented a working emulator with all CPU components"*
- ✓ *"Created a two-pass assembler"*
- ✓ *"Wrote and tested multiple programs"*

*"The Fibonacci program demonstrates:"*
- *"Fetch-decode-execute cycles"*
- *"Register operations and ALU usage"*
- *"Conditional branching with flags"*
- *"Memory-mapped I/O"*
- *"Complete program flow from start to halt"*

**[Show final slide with GitHub repo URL]**

*"All source code, documentation, and this demo are available in our GitHub repository. Thank you for watching!"*

---

## Tips for Recording

### Visual Aids to Prepare

1. **CPU Architecture Diagram** - Use the ASCII art from CPU_ARCHITECTURE.md or create a visual diagram
2. **Fetch-Decode-Execute Flowchart** - Show the cycle graphically
3. **Register State Tables** - Show register contents changing through iterations
4. **Memory Map Diagram** - Visual representation of memory layout
5. **Instruction Format Diagrams** - Show how instructions are encoded

### Screen Recording Tips

1. **Use large fonts** - Ensure code is readable (16-18pt minimum)
2. **Zoom in on relevant sections** - Don't show entire screen if not needed
3. **Highlight as you talk** - Use cursor or highlighting to point to current topic
4. **Pause between sections** - Allow viewers to absorb information
5. **Keep terminal colors clear** - Use high-contrast themes

### Narration Tips

1. **Speak clearly and at moderate pace** - Technical content needs clear enunciation
2. **Define acronyms first** - PC (Program Counter), ALU (Arithmetic Logic Unit), etc.
3. **Use analogies** - Compare to familiar concepts where possible
4. **Show enthusiasm** - This is cool stuff!
5. **Preview and review** - "We're going to look at..." then "We just saw..."

### Timing Guidelines

- **Total video**: 7-8 minutes
- **Don't rush** - Better to cut some detail than rush through
- **Practice beforehand** - Do a dry run to catch issues
- **Edit if needed** - It's okay to cut and splice for clarity

### Common Pitfalls to Avoid

1. ❌ Reading code line-by-line without explanation
2. ❌ Going too deep into implementation details
3. ❌ Assuming viewers know assembly language
4. ❌ Forgetting to show output
5. ❌ Not explaining the "why" behind design decisions

### What Makes a Great Demo

✓ Clear narrative structure
✓ Visual aids and diagrams
✓ Working demonstrations
✓ Explanation of key concepts
✓ Enthusiasm and engagement
✓ Professional presentation

---

## Alternative: Shorter Version (4-5 minutes)

If time is limited, focus on:

1. **Quick architecture overview** (30 sec)
2. **Fibonacci program structure** (1 min)
3. **One complete fetch-decode-execute example** (1.5 min)
4. **Running demonstration** (1 min)
5. **Conclusion** (30 sec)

Skip the detailed iteration-by-iteration walkthrough and memory-mapped I/O details.

---

## Questions You Might Be Asked

**Q: Why did you choose this architecture?**
A: Educational clarity. The 8-bit design is simple enough to understand completely while being powerful enough to run real programs.

**Q: How does memory-mapped I/O work?**
A: Certain memory addresses (0xFF00-0xFFFF) trigger hardware actions instead of accessing RAM. Writing to 0xFF01 outputs a character.

**Q: What was the hardest part?**
A: Coordinating the instruction encoding between the assembler and emulator, and debugging the two-pass assembly process.

**Q: Could this run on real hardware?**
A: With modifications, yes! The design could be implemented in Verilog/VHDL for FPGA implementation.

**Q: How long did this take?**
A: [Answer honestly based on your team's experience]

---

Good luck with your demo video! 🎬

