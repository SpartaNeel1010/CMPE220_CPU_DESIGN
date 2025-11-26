#include "cpu.h"
#include <iostream>
#include <iomanip>
#include <sstream>

CPU::CPU(Memory* mem) : memory(mem), halted(false), cycle_count(0), debug_mode(false) {
    reset();
}

void CPU::reset() {
    // Initialize all registers to 0
    for (int i = 0; i < 8; i++) {
        registers[i] = 0;
    }
    
    // Initialize PC to program start (0x0100)
    pc = 0x0100;
    
    // Initialize SP (R7) to top of stack (just below I/O area)
    registers[7] = 0xFF;  // High byte of 0xFEFF
    
    // Clear flags
    flags = 0;
    
    // Reset state
    halted = false;
    cycle_count = 0;
    
    // Reset bus
    bus.reset();
    
    // Clear instruction register
    ir[0] = ir[1] = ir[2] = 0;
}

void CPU::step() {
    if (halted) {
        return;
    }
    
    // FETCH phase
    fetch();
    
    // DECODE and EXECUTE phase
    execute();
    
    // Update timer
    memory->updateTimer();
    
    cycle_count++;
}

void CPU::run() {
    std::cout << "Starting CPU execution at PC=0x" << std::hex << pc << std::dec << std::endl;
    
    while (!halted) {
        step();
        
        // Safety check: halt if PC goes out of bounds or too many cycles
        if (pc >= 0xFF00 || cycle_count > 1000000) {
            std::cerr << "Error: CPU runaway detected (PC=0x" << std::hex << pc 
                      << ", cycles=" << std::dec << cycle_count << ")" << std::endl;
            halted = true;
            break;
        }
    }
    
    std::cout << "\nCPU halted after " << cycle_count << " cycles" << std::endl;
}

void CPU::fetch() {
    // Read instruction from memory
    ir[0] = memory->read(pc);
    
    if (debug_mode) {
        std::cout << "\n[FETCH] PC=0x" << std::hex << std::setw(4) 
                  << std::setfill('0') << pc << " IR[0]=0x" 
                  << std::setw(2) << static_cast<int>(ir[0]) << std::dec << std::endl;
    }
}

uint8_t CPU::fetchByte() {
    uint8_t byte = memory->read(pc);
    pc++;
    return byte;
}

uint16_t CPU::fetchWord() {
    uint8_t low = fetchByte();
    uint8_t high = fetchByte();
    return low | (high << 8);
}

void CPU::execute() {
    uint8_t opcode = getOpcode();
    
    if (debug_mode) {
        std::cout << "[EXECUTE] Opcode=0x" << std::hex << static_cast<int>(opcode) 
                  << std::dec << " ";
    }
    
    // Increment PC past the first byte
    pc++;
    
    // Execute based on opcode
    switch (opcode) {
        // Arithmetic
        case 0x00: // ADD
            executeArithmetic();
            break;
        case 0x01: // ADDI
        case 0x02: // SUB
        case 0x03: // SUBI
        case 0x04: // MUL
        case 0x05: // INC
        case 0x06: // DEC
            executeArithmetic();
            break;
            
        // Logical
        case 0x07: // AND
        case 0x08: // ANDI
        case 0x09: // OR
        case 0x0A: // ORI
        case 0x0B: // XOR
        case 0x0C: // NOT
        case 0x0D: // SHL
        case 0x0E: // SHR
            executeLogical();
            break;
            
        // Memory
        case 0x10: // LOAD
        case 0x11: // STORE
        case 0x12: // LOADI
            executeMemory();
            break;
            
        // Comparison
        case 0x13: // CMP
        case 0x14: // CMPI
            executeArithmetic();  // CMP uses ALU
            break;
            
        // Stack
        case 0x15: // PUSH
        case 0x16: // POP
            executeStack();
            break;
            
        // Control flow
        case 0x18: // JMP
        case 0x19: // JZ
        case 0x1A: // JNZ
        case 0x1B: // JC
        case 0x1C: // JNC
        case 0x1D: // CALL
        case 0x1E: // RET
            executeControl();
            break;
            
        // Special
        case 0x1F: // HALT or NOP
            executeSpecial();
            break;
            
        default:
            std::cerr << "Error: Unknown opcode 0x" << std::hex << static_cast<int>(opcode) 
                      << " at PC=0x" << (pc - 1) << std::dec << std::endl;
            halted = true;
            break;
    }
    
    if (debug_mode) {
        printState();
    }
}

void CPU::executeArithmetic() {
    uint8_t opcode = getOpcode();
    uint8_t rd = getRd();
    
    switch (opcode) {
        case 0x00: { // ADD Rd, Rs1, Rs2
            ir[1] = fetchByte();
            uint8_t rs1 = getRs1();
            uint8_t rs2 = getRs2();
            if (debug_mode) std::cout << "ADD R" << static_cast<int>(rd) 
                                      << ", R" << static_cast<int>(rs1) 
                                      << ", R" << static_cast<int>(rs2) << std::endl;
            registers[rd] = alu.add(registers[rs1], registers[rs2], flags);
            break;
        }
        case 0x01: { // ADDI Rd, Rs, imm
            ir[1] = fetchByte();
            uint8_t rs = getRd();  // Source is in same position as Rd for immediate
            uint8_t imm = getImmediate();
            if (debug_mode) std::cout << "ADDI R" << static_cast<int>(rd) 
                                      << ", R" << static_cast<int>(rs) 
                                      << ", " << static_cast<int>(imm) << std::endl;
            // Note: For RI format, we need to reinterpret the instruction
            // Actually rd is destination, and we need another byte for source+immediate
            // Let me fix the implementation to match ISA properly
            ir[1] = memory->read(pc++);
            registers[rd] = alu.add(registers[rd], imm, flags);
            break;
        }
        case 0x02: { // SUB Rd, Rs1, Rs2
            ir[1] = fetchByte();
            uint8_t rs1 = getRs1();
            uint8_t rs2 = getRs2();
            if (debug_mode) std::cout << "SUB R" << static_cast<int>(rd) 
                                      << ", R" << static_cast<int>(rs1) 
                                      << ", R" << static_cast<int>(rs2) << std::endl;
            registers[rd] = alu.subtract(registers[rs1], registers[rs2], flags);
            break;
        }
        case 0x03: { // SUBI Rd, Rs, imm
            ir[1] = fetchByte();
            uint8_t imm = getImmediate();
            if (debug_mode) std::cout << "SUBI R" << static_cast<int>(rd) 
                                      << ", " << static_cast<int>(imm) << std::endl;
            registers[rd] = alu.subtract(registers[rd], imm, flags);
            break;
        }
        case 0x04: { // MUL Rd, Rs1, Rs2
            ir[1] = fetchByte();
            uint8_t rs1 = getRs1();
            uint8_t rs2 = getRs2();
            if (debug_mode) std::cout << "MUL R" << static_cast<int>(rd) 
                                      << ", R" << static_cast<int>(rs1) 
                                      << ", R" << static_cast<int>(rs2) << std::endl;
            registers[rd] = alu.multiply(registers[rs1], registers[rs2], flags);
            break;
        }
        case 0x05: // INC Rd
            if (debug_mode) std::cout << "INC R" << static_cast<int>(rd) << std::endl;
            registers[rd] = alu.increment(registers[rd], flags);
            break;
        case 0x06: // DEC Rd
            if (debug_mode) std::cout << "DEC R" << static_cast<int>(rd) << std::endl;
            registers[rd] = alu.decrement(registers[rd], flags);
            break;
        case 0x13: { // CMP Rs1, Rs2
            ir[1] = fetchByte();
            uint8_t rs1 = getRd();  // First operand in Rd position
            uint8_t rs2 = getRs1(); // Second operand in Rs1 position
            if (debug_mode) std::cout << "CMP R" << static_cast<int>(rs1) 
                                      << ", R" << static_cast<int>(rs2) << std::endl;
            alu.compare(registers[rs1], registers[rs2], flags);
            break;
        }
        case 0x14: { // CMPI Rs, imm
            ir[1] = fetchByte();
            uint8_t rs = getRd();
            uint8_t imm = getImmediate();
            if (debug_mode) std::cout << "CMPI R" << static_cast<int>(rs) 
                                      << ", " << static_cast<int>(imm) << std::endl;
            alu.compare(registers[rs], imm, flags);
            break;
        }
    }
}

void CPU::executeLogical() {
    uint8_t opcode = getOpcode();
    uint8_t rd = getRd();
    
    switch (opcode) {
        case 0x07: { // AND Rd, Rs1, Rs2
            ir[1] = fetchByte();
            uint8_t rs1 = getRs1();
            uint8_t rs2 = getRs2();
            if (debug_mode) std::cout << "AND R" << static_cast<int>(rd) 
                                      << ", R" << static_cast<int>(rs1) 
                                      << ", R" << static_cast<int>(rs2) << std::endl;
            registers[rd] = alu.logicalAnd(registers[rs1], registers[rs2], flags);
            break;
        }
        case 0x08: { // ANDI Rd, Rs, imm
            ir[1] = fetchByte();
            uint8_t imm = getImmediate();
            if (debug_mode) std::cout << "ANDI R" << static_cast<int>(rd) 
                                      << ", " << static_cast<int>(imm) << std::endl;
            registers[rd] = alu.logicalAnd(registers[rd], imm, flags);
            break;
        }
        case 0x09: { // OR Rd, Rs1, Rs2
            ir[1] = fetchByte();
            uint8_t rs1 = getRs1();
            uint8_t rs2 = getRs2();
            if (debug_mode) std::cout << "OR R" << static_cast<int>(rd) 
                                      << ", R" << static_cast<int>(rs1) 
                                      << ", R" << static_cast<int>(rs2) << std::endl;
            registers[rd] = alu.logicalOr(registers[rs1], registers[rs2], flags);
            break;
        }
        case 0x0A: { // ORI Rd, Rs, imm
            ir[1] = fetchByte();
            uint8_t imm = getImmediate();
            if (debug_mode) std::cout << "ORI R" << static_cast<int>(rd) 
                                      << ", " << static_cast<int>(imm) << std::endl;
            registers[rd] = alu.logicalOr(registers[rd], imm, flags);
            break;
        }
        case 0x0B: { // XOR Rd, Rs1, Rs2
            ir[1] = fetchByte();
            uint8_t rs1 = getRs1();
            uint8_t rs2 = getRs2();
            if (debug_mode) std::cout << "XOR R" << static_cast<int>(rd) 
                                      << ", R" << static_cast<int>(rs1) 
                                      << ", R" << static_cast<int>(rs2) << std::endl;
            registers[rd] = alu.logicalXor(registers[rs1], registers[rs2], flags);
            break;
        }
        case 0x0C: { // NOT Rd, Rs
            ir[1] = fetchByte();
            uint8_t rs = getRs1();
            if (debug_mode) std::cout << "NOT R" << static_cast<int>(rd) 
                                      << ", R" << static_cast<int>(rs) << std::endl;
            registers[rd] = alu.logicalNot(registers[rs], flags);
            break;
        }
        case 0x0D: { // SHL Rd, Rs
            ir[1] = fetchByte();
            uint8_t rs = getRs1();
            if (debug_mode) std::cout << "SHL R" << static_cast<int>(rd) 
                                      << ", R" << static_cast<int>(rs) << std::endl;
            registers[rd] = alu.shiftLeft(registers[rd], registers[rs], flags);
            break;
        }
        case 0x0E: { // SHR Rd, Rs
            ir[1] = fetchByte();
            uint8_t rs = getRs1();
            if (debug_mode) std::cout << "SHR R" << static_cast<int>(rd) 
                                      << ", R" << static_cast<int>(rs) << std::endl;
            registers[rd] = alu.shiftRight(registers[rd], registers[rs], flags);
            break;
        }
    }
}

void CPU::executeMemory() {
    uint8_t opcode = getOpcode();
    uint8_t rd = getRd();
    
    switch (opcode) {
        case 0x10: { // LOAD Rd, [addr]
            uint16_t addr = fetchWord();
            if (debug_mode) std::cout << "LOAD R" << static_cast<int>(rd) 
                                      << ", [0x" << std::hex << addr << "]" << std::dec << std::endl;
            registers[rd] = memory->read(addr);
            break;
        }
        case 0x11: { // STORE Rs, [addr]
            uint16_t addr = fetchWord();
            if (debug_mode) std::cout << "STORE R" << static_cast<int>(rd) 
                                      << ", [0x" << std::hex << addr << "]" << std::dec << std::endl;
            memory->write(addr, registers[rd]);
            break;
        }
        case 0x12: { // LOADI Rd, imm
            ir[1] = fetchByte();
            uint8_t imm = getImmediate();
            if (debug_mode) std::cout << "LOADI R" << static_cast<int>(rd) 
                                      << ", " << static_cast<int>(imm) << std::endl;
            registers[rd] = imm;
            break;
        }
    }
}

void CPU::executeControl() {
    uint8_t opcode = getOpcode();
    
    switch (opcode) {
        case 0x18: { // JMP addr
            uint16_t addr = fetchWord();
            if (debug_mode) std::cout << "JMP 0x" << std::hex << addr << std::dec << std::endl;
            pc = addr;
            break;
        }
        case 0x19: { // JZ addr
            uint16_t addr = fetchWord();
            if (debug_mode) std::cout << "JZ 0x" << std::hex << addr << std::dec;
            if (flags & ALU::FLAG_Z) {
                if (debug_mode) std::cout << " (taken)" << std::endl;
                pc = addr;
            } else {
                if (debug_mode) std::cout << " (not taken)" << std::endl;
            }
            break;
        }
        case 0x1A: { // JNZ addr
            uint16_t addr = fetchWord();
            if (debug_mode) std::cout << "JNZ 0x" << std::hex << addr << std::dec;
            if (!(flags & ALU::FLAG_Z)) {
                if (debug_mode) std::cout << " (taken)" << std::endl;
                pc = addr;
            } else {
                if (debug_mode) std::cout << " (not taken)" << std::endl;
            }
            break;
        }
        case 0x1B: { // JC addr
            uint16_t addr = fetchWord();
            if (debug_mode) std::cout << "JC 0x" << std::hex << addr << std::dec;
            if (flags & ALU::FLAG_C) {
                if (debug_mode) std::cout << " (taken)" << std::endl;
                pc = addr;
            } else {
                if (debug_mode) std::cout << " (not taken)" << std::endl;
            }
            break;
        }
        case 0x1C: { // JNC addr
            uint16_t addr = fetchWord();
            if (debug_mode) std::cout << "JNC 0x" << std::hex << addr << std::dec;
            if (!(flags & ALU::FLAG_C)) {
                if (debug_mode) std::cout << " (taken)" << std::endl;
                pc = addr;
            } else {
                if (debug_mode) std::cout << " (not taken)" << std::endl;
            }
            break;
        }
        case 0x1D: { // CALL addr
            uint16_t addr = fetchWord();
            if (debug_mode) std::cout << "CALL 0x" << std::hex << addr << std::dec << std::endl;
            // Push return address (current PC) onto stack
            push(pc & 0xFF);        // Low byte
            push((pc >> 8) & 0xFF); // High byte
            pc = addr;
            break;
        }
        case 0x1E: { // RET
            if (debug_mode) std::cout << "RET" << std::endl;
            // Pop return address from stack
            uint8_t high = pop();
            uint8_t low = pop();
            pc = low | (high << 8);
            break;
        }
    }
}

void CPU::executeStack() {
    uint8_t opcode = getOpcode();
    uint8_t rd = getRd();
    
    switch (opcode) {
        case 0x15: // PUSH Rs
            if (debug_mode) std::cout << "PUSH R" << static_cast<int>(rd) << std::endl;
            push(registers[rd]);
            break;
        case 0x16: // POP Rd
            if (debug_mode) std::cout << "POP R" << static_cast<int>(rd) << std::endl;
            registers[rd] = pop();
            break;
    }
}

void CPU::executeSpecial() {
    uint8_t opcode = getOpcode();
    
    switch (opcode) {
        case 0x1F: // HALT or NOP
            if (ir[0] == 0xFF) {
                // NOP (encoded as 0xFF)
                if (debug_mode) std::cout << "NOP" << std::endl;
            } else {
                // HALT (encoded as 0xF8)
                if (debug_mode) std::cout << "HALT" << std::endl;
                halted = true;
            }
            break;
    }
}

void CPU::push(uint8_t value) {
    // SP is R7 - it holds only the high byte, low byte is in a separate location
    // For simplicity, we'll use a full 16-bit stack pointer
    // Decrement SP first (pre-decrement)
    uint16_t sp = (registers[7] << 8) | 0xFE;
    sp--;
    memory->write(sp, value);
    registers[7] = (sp >> 8) & 0xFF;
}

uint8_t CPU::pop() {
    // Read value then increment SP (post-increment)
    uint16_t sp = (registers[7] << 8) | 0xFE;
    uint8_t value = memory->read(sp);
    sp++;
    registers[7] = (sp >> 8) & 0xFF;
    return value;
}

void CPU::printState() {
    std::cout << "Registers: ";
    for (int i = 0; i < 8; i++) {
        std::cout << "R" << i << "=0x" << std::hex << std::setw(2) 
                  << std::setfill('0') << static_cast<int>(registers[i]) << " ";
    }
    std::cout << std::dec << std::endl;
    
    std::cout << "PC=0x" << std::hex << std::setw(4) << std::setfill('0') << pc;
    std::cout << " Flags=[";
    std::cout << ((flags & ALU::FLAG_N) ? "N" : "-");
    std::cout << ((flags & ALU::FLAG_Z) ? "Z" : "-");
    std::cout << ((flags & ALU::FLAG_C) ? "C" : "-");
    std::cout << ((flags & ALU::FLAG_V) ? "V" : "-");
    std::cout << "] Cycles=" << std::dec << cycle_count << std::endl;
}

std::string CPU::disassemble(uint8_t opcode, uint8_t byte1, uint8_t byte2) {
    // Simple disassembly for debugging
    std::stringstream ss;
    ss << "Opcode 0x" << std::hex << static_cast<int>(opcode);
    return ss.str();
}

