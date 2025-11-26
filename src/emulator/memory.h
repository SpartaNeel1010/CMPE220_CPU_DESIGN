#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <vector>
#include <iostream>

/**
 * Memory class - Implements 64KB memory with memory-mapped I/O
 * 
 * Memory Map:
 * 0x0000 - 0x00FF: System area
 * 0x0100 - 0xFEFF: General RAM
 * 0xFF00 - 0xFFFF: Memory-mapped I/O
 */
class Memory {
private:
    std::vector<uint8_t> ram;  // 64KB of memory
    
    // Memory-mapped I/O registers
    uint8_t timer_ctrl;        // 0xFF00
    uint8_t console_out;       // 0xFF01
    uint8_t console_in;        // 0xFF02
    uint8_t timer_value;       // 0xFF03
    
    // Timer state
    int timer_counter;
    
public:
    Memory();
    
    // Read and write operations
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
    
    // Memory operations
    void loadProgram(const std::vector<uint8_t>& program, uint16_t start_address = 0x0100);
    void dump(uint16_t start, uint16_t end);
    void reset();
    
    // Timer operations
    void updateTimer();
    
    // Get pointer to raw memory (for debugging)
    const uint8_t* getRawMemory() const { return ram.data(); }
};

#endif // MEMORY_H

