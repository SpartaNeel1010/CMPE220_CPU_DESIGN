# SC8 Software CPU Design Project

## CMPE220 - Computer Architecture

### Team Members

**Neel Asheshbhai Shah**

**Vedant Tushar Shah**

**Aarav Pranav Shah**

**Harshavardhan Kuruvella**

---

*Course: CMPE220 - Computer Architecture*

*Semester: Fall 2025*

*Submission Date: November 26, 2025*

---

<div style="page-break-after: always;"></div>

## GitHub Repository

The complete project source code, documentation, and demo video are available on GitHub:

**Repository URL:** [To be filled in with your actual GitHub repository URL]

Example: `https://github.com/your-username/CMPE220_project`

### Repository Contents

The repository includes:

- Complete source code for CPU emulator (C++)
- Complete source code for assembler (C++)
- Sample assembly programs (Timer, Hello World, Fibonacci)
- Comprehensive documentation (ISA specification, architecture details)
- Demo video script and walkthrough
- Build system (Makefile)
- Project report (this document)

### Accessing the Repository

1. Visit the GitHub URL above
2. Clone the repository: `git clone [repository-url]`
3. Follow the instructions in the README.md file

All team members have contributed to the repository with documented commits showing individual contributions.

---

<div style="page-break-after: always;"></div>

## Download, Compile, and Run Instructions

### Prerequisites

Before building the project, ensure you have the following installed:

- **C++ Compiler**: g++ or clang++ with C++11 support
- **Make utility**: For building the project
- **Git**: For cloning the repository
- **Operating System**: Linux, macOS, or WSL (Windows Subsystem for Linux)

### Step 1: Download the Project

#### Option A: Clone from GitHub

```bash
git clone [your-repository-url]
cd CMPE220_project
```

#### Option B: Download ZIP

1. Visit the GitHub repository URL
2. Click "Code" → "Download ZIP"
3. Extract the ZIP file
4. Open terminal and navigate to the extracted folder

### Step 2: Verify Prerequisites

Check that you have the required tools:

```bash
# Check C++ compiler
g++ --version

# Check Make
make --version

# Check Git (optional, for cloning)
git --version
```

If any tools are missing:

**On Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential git
```

**On macOS:**
```bash
# Install Xcode Command Line Tools
xcode-select --install
```

**On Windows:**
- Install WSL (Windows Subsystem for Linux)
- Then follow Ubuntu instructions

### Step 3: Build the Project

Navigate to the project directory and build everything:

```bash
# Build emulator, assembler, and assemble all programs
make all
```

This command will:
1. Compile the CPU emulator from C++ source
2. Compile the assembler from C++ source
3. Assemble all `.asm` programs to `.bin` format

**Expected output:**
```
Building emulator...
✓ Emulator built: bin/emulator
Building assembler...
✓ Assembler built: bin/assembler
Assembling programs/timer.asm...
✓ Assembled: programs/timer.bin
Assembling programs/hello_world.asm...
✓ Assembled: programs/hello_world.bin
Assembling programs/fibonacci.asm...
✓ Assembled: programs/fibonacci.bin
✓ Build complete!
```

#### Troubleshooting Build Issues

**Issue: "g++ command not found"**
- Solution: Install build tools (see Step 2)

**Issue: "C++11 not supported"**
- Solution: Update g++ to version 4.8 or higher

**Issue: "Permission denied"**
- Solution: Make Makefile executable: `chmod +x Makefile`

### Step 4: Run the Programs

After successful build, run the sample programs:

#### Run Hello World

```bash
make run-hello
```

**Expected output:**
```
Running Hello World program...
================================
SC8 CPU Emulator
Program: programs/hello_world.bin
Size: 42 bytes
Start address: 0x0100

Starting CPU execution at PC=0x0100
Hello, World!

CPU halted after 29 cycles

=== Final CPU State ===
...
Execution completed successfully.
```

#### Run Fibonacci Sequence

```bash
make run-fib
```

**Expected output:**
```
Running Fibonacci program...
================================
...
Fib: 0 1 1 2 3 5 8 ## ## ## Done!
...
```

Note: `##` symbols represent numbers ≥10 (our simplified output only shows single digits)

#### Run Timer Example

```bash
make run-timer
```

**Expected output:**
```
Running Timer program...
================================
...
1
2
3
4
5
Done
...
```

### Step 5: Run All Tests

To run all programs as a test suite:

```bash
make test
```

This will execute all three programs and display their output.

### Advanced Usage

#### Debug Mode (Step-by-Step Execution)

Run programs with instruction-level debugging:

```bash
make debug-fib
```

This will:
- Show each instruction as it executes
- Display register and flag states
- Pause between instructions (press Enter to continue)

#### Memory Dump

View memory contents after execution:

```bash
./bin/emulator -m programs/fibonacci.bin
```

#### Custom Programs

To create and run your own assembly program:

1. Create a file: `programs/my_program.asm`
2. Assemble it: `./bin/assembler programs/my_program.asm programs/my_program.bin`
3. Run it: `./bin/emulator programs/my_program.bin`

### Step 6: Clean Build Artifacts

To remove all compiled files:

```bash
make clean
```

This removes:
- Compiled binaries (`bin/` directory)
- Assembled programs (`.bin` files)

### Getting Help

View all available make targets:

```bash
make help
```

For detailed documentation:
- ISA Reference: `docs/ISA_SPECIFICATION.md`
- Architecture Details: `docs/CPU_ARCHITECTURE.md`
- Demo Script: `docs/DEMO_SCRIPT.md`
- Main README: `README.md`

---

<div style="page-break-after: always;"></div>

## Team Member Contributions

### Neel Asheshbhai Shah

**Primary Responsibilities:**

1. **CPU Emulator Core Implementation**
   - Designed and implemented the CPU class with register file and program counter
   - Implemented the fetch-decode-execute cycle
   - Developed the instruction execution logic for all arithmetic and logical operations
   - Implemented flags register and conditional branching logic
   - Created the main emulator program with command-line interface

2. **Memory System**
   - Designed and implemented the 64KB memory subsystem
   - Implemented memory-mapped I/O for console output (0xFF01)
   - Developed the memory dump functionality for debugging
   - Created the timer simulation at memory address 0xFF00

3. **Testing and Integration**
   - Conducted unit testing of ALU operations
   - Performed integration testing of emulator components
   - Debugged instruction encoding/decoding issues
   - Validated program execution correctness

**Files/Modules:**
- `src/emulator/cpu.h` and `cpu.cpp`
- `src/emulator/memory.h` and `memory.cpp`
- `src/emulator/main.cpp`
- Testing and validation of all emulator functionality

### Vedant Tushar Shah

**Primary Responsibilities:**

1. **Assembler Implementation**
   - Designed and implemented the two-pass assembler architecture
   - Developed the lexer for tokenization of assembly source code
   - Implemented the parser for syntax analysis and instruction parsing
   - Created the code generator for binary output
   - Designed the instruction encoding scheme

2. **Symbol Table Management**
   - Implemented the symbol table for label resolution
   - Developed the two-pass algorithm for forward reference resolution
   - Created error reporting with line numbers
   - Handled multiple numeric formats (decimal, hex, binary)

3. **Build System**
   - Created comprehensive Makefile with multiple targets
   - Set up build dependencies and rules
   - Implemented convenient shortcuts for building and running
   - Added debug and test targets

**Files/Modules:**
- `src/assembler/assembler.h` and `assembler.cpp`
- `src/assembler/lexer.h` and `lexer.cpp`
- `src/assembler/parser.h` and `parser.cpp`
- `src/assembler/symbol_table.h` and `symbol_table.cpp`
- `Makefile`

### Aarav Pranav Shah

**Primary Responsibilities:**

1. **ISA Design and Documentation**
   - Designed the complete instruction set architecture (ISA)
   - Created instruction format specifications (RR, RI, MEM, BR, SO)
   - Defined opcode encoding scheme
   - Specified addressing modes and flag semantics
   - Documented memory map layout

2. **ALU and Bus Implementation**
   - Implemented the Arithmetic Logic Unit with all operations
   - Developed flag update logic (N, Z, C, V)
   - Implemented the system bus simulation
   - Created the control unit for instruction orchestration

3. **Architecture Documentation**
   - Created comprehensive ISA specification document
   - Wrote CPU architecture documentation with diagrams
   - Designed CPU schematic showing component interconnections
   - Documented data flow and execution cycles

**Files/Modules:**
- `src/emulator/alu.h` and `alu.cpp`
- `src/emulator/bus.h` and `bus.cpp`
- `docs/ISA_SPECIFICATION.md`
- `docs/CPU_ARCHITECTURE.md`
- CPU architecture diagrams and schematics

### Harshavardhan Kuruvella

**Primary Responsibilities:**

1. **Sample Programs Development**
   - Wrote the Hello World program demonstrating console I/O
   - Developed the Fibonacci sequence program with loops and arithmetic
   - Created the Timer example showing fetch/compute/store cycles
   - Added extensive comments explaining each instruction
   - Validated program correctness through testing

2. **Documentation and Reporting**
   - Created comprehensive README.md with usage instructions
   - Wrote detailed demo video script for Fibonacci program
   - Developed project report with all required sections
   - Created troubleshooting guide and FAQ
   - Documented example programs and use cases

3. **Demo Video Preparation**
   - Scripted the demo video walkthrough
   - Prepared visual aids and diagrams for presentation
   - Created step-by-step instruction explanations
   - Documented fetch-decode-execute cycle examples
   - Prepared Q&A for potential questions

**Files/Modules:**
- `programs/hello_world.asm`
- `programs/fibonacci.asm`
- `programs/timer.asm`
- `README.md`
- `docs/DEMO_SCRIPT.md`
- `report/PROJECT_REPORT.md`

---

### Team Collaboration

All team members participated in:

- **Design meetings**: ISA design, architecture decisions, instruction set selection
- **Code reviews**: Peer review of implementations for quality and correctness
- **Testing**: Collaborative testing and debugging of integrated system
- **Documentation review**: Cross-checking documentation for accuracy
- **Integration**: Ensuring assembler and emulator work together correctly

### Version Control

All contributions were tracked through Git with meaningful commit messages, allowing clear attribution of work and collaborative development.

---

<div style="page-break-after: always;"></div>

## Project Summary

### Overview

This project successfully implements a complete 8-bit CPU (SC8) with:
- ✓ Fully functional emulator with fetch-decode-execute cycle
- ✓ Two-pass assembler with label support
- ✓ Three working sample programs
- ✓ Comprehensive documentation
- ✓ Professional build system

### Technical Achievements

1. **CPU Emulator**: 20+ instructions, memory-mapped I/O, debugging support
2. **Assembler**: Two-pass assembly, multiple numeric formats, error reporting
3. **Programs**: Timer (with cycle annotations), Hello World, Fibonacci
4. **Documentation**: ISA spec, architecture docs, demo script, README

### Educational Value

This project demonstrates:
- Computer architecture fundamentals
- Instruction set design
- Assembly language programming
- Software engineering practices
- Team collaboration and project management

### Future Enhancements

Potential improvements for future iterations:
- Interrupt support
- More memory-mapped devices
- Pipelining simulation
- Macro support in assembler
- Interactive debugger with breakpoints

---

## Conclusion

The SC8 Software CPU Design project successfully delivers a complete, working CPU implementation suitable for educational purposes. All components function correctly, documentation is comprehensive, and the code is well-organized and commented.

The project meets all requirements and demonstrates deep understanding of computer architecture principles.

---

**End of Report**

---

## Formatting Notes for DOCX Conversion

When converting this Markdown document to DOCX format, please ensure:

1. **Page Numbers**: Add page numbers in footer (bottom right or center)
2. **Margins**: Set to 1 inch on all sides
3. **Double Spacing**: Apply double line spacing throughout
4. **Font**: Use readable font (e.g., Times New Roman 12pt or Arial 11pt)
5. **Headings**: Use built-in heading styles for hierarchy
6. **Page Breaks**: Respect `<div style="page-break-after: always;"></div>` markers
7. **Code Blocks**: Use monospace font (Courier New or Consolas)
8. **Title Page**: Format as centered, professional title page

### Recommended DOCX Conversion Tools

- **Pandoc**: `pandoc PROJECT_REPORT.md -o PROJECT_REPORT.docx`
- **Microsoft Word**: File → Import
- **Google Docs**: Import Markdown via add-on
- **Online converters**: markdown-to-word.com

### Additional Formatting

After conversion to DOCX:
1. Add actual GitHub repository URL (currently placeholder)
2. Verify page breaks are correct
3. Check that code formatting is preserved
4. Ensure spacing is consistent
5. Add header/footer with team name or course info (optional)

