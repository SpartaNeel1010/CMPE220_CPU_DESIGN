#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <string>
#include "memory.h"
#include "alu.h"
#include "bus.h"

/**
 * CPU class - Main CPU implementation
 * 
 * Implements the SC8 8-bit CPU with:
 * - 8 general-purpose registers (R0-R7)
 * - 16-bit Program Counter (PC)
 * - 8-bit Flags register
 * - Instruction fetch/decode/execute cycle
 */
class CPU {
private:
    // Registers
    uint8_t registers[8];    // R0-R7 (R7 is SP)
    uint16_t pc;             // Program Counter
    uint8_t flags;           // Status flags
    
    // Components
    Memory* memory;
    ALU alu;
    Bus bus;
    
    // State
    bool halted;
    uint64_t cycle_count;
    bool debug_mode;
    
    // Instruction register
    uint8_t ir[3];  // Current instruction (max 3 bytes)
    
public:
    CPU(Memory* mem);
    
    // CPU control
    void reset();
    void step();           // Execute one instruction
    void run();            // Run until HALT
    bool isHalted() const { return halted; }
    
    // Debugging
    void enableDebug(bool enable) { debug_mode = enable; }
    void printState();
    uint64_t getCycleCount() const { return cycle_count; }
    
    // Register access (for debugging)
    uint8_t getRegister(int reg) const { return registers[reg]; }
    uint16_t getPC() const { return pc; }
    uint8_t getFlags() const { return flags; }
    
private:
    // Instruction cycle phases
    void fetch();
    void decode();
    void execute();
    
    // Instruction execution helpers
    void executeArithmetic();
    void executeLogical();
    void executeMemory();
    void executeControl();
    void executeStack();
    void executeSpecial();
    
    // Helper functions
    uint8_t fetchByte();
    uint16_t fetchWord();
    void push(uint8_t value);
    uint8_t pop();
    
    // Opcode extraction
    uint8_t getOpcode() const { return ir[0] >> 3; }
    uint8_t getRd() const { return ir[0] & 0x07; }
    uint8_t getRs1() const { return (ir[1] >> 5) & 0x07; }
    uint8_t getRs2() const { return (ir[1] >> 2) & 0x07; }
    uint8_t getImmediate() const { return ir[1]; }
    uint16_t getAddress() const { return ir[1] | (ir[2] << 8); }
    
    // Disassembly (for debug output)
    std::string disassemble(uint8_t opcode, uint8_t byte1, uint8_t byte2);
};

#endif // CPU_H

