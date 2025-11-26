#include "memory.h"
#include <iomanip>
#include <cstring>

Memory::Memory() : ram(65536, 0), timer_ctrl(0), console_out(0), 
                   console_in(0), timer_value(0), timer_counter(0) {
}

uint8_t Memory::read(uint16_t address) {
    // Check for memory-mapped I/O
    if (address >= 0xFF00) {
        switch (address) {
            case 0xFF00:  // TIMER_CTRL
                return timer_ctrl;
            case 0xFF01:  // CONSOLE_OUT (write-only, return 0)
                return 0;
            case 0xFF02:  // CONSOLE_IN (read character from stdin)
                // For simplicity, return 0 (no input available)
                return console_in;
            case 0xFF03:  // TIMER_VALUE
                return timer_value;
            default:
                return ram[address];
        }
    }
    
    return ram[address];
}

void Memory::write(uint16_t address, uint8_t value) {
    // Check for memory-mapped I/O
    if (address >= 0xFF00) {
        switch (address) {
            case 0xFF00:  // TIMER_CTRL - start timer
                timer_ctrl = value;
                timer_counter = value;
                timer_value = value;
                break;
            case 0xFF01:  // CONSOLE_OUT - output character
                console_out = value;
                std::cout << static_cast<char>(value);
                std::cout.flush();
                break;
            case 0xFF02:  // CONSOLE_IN - input character (write has no effect)
                break;
            case 0xFF03:  // TIMER_VALUE (read-only, write has no effect)
                break;
            default:
                ram[address] = value;
                break;
        }
    } else {
        ram[address] = value;
    }
}

void Memory::loadProgram(const std::vector<uint8_t>& program, uint16_t start_address) {
    if (start_address + program.size() > 65536) {
        std::cerr << "Error: Program too large to fit in memory" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < program.size(); i++) {
        ram[start_address + i] = program[i];
    }
    
    std::cout << "Loaded " << program.size() << " bytes at address 0x" 
              << std::hex << std::setw(4) << std::setfill('0') 
              << start_address << std::dec << std::endl;
}

void Memory::dump(uint16_t start, uint16_t end) {
    std::cout << "\n=== Memory Dump (0x" << std::hex << std::setw(4) 
              << std::setfill('0') << start << " - 0x" << end << ") ===" 
              << std::dec << std::endl;
    
    for (uint16_t addr = start; addr <= end && addr >= start; addr += 16) {
        std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0') 
                  << addr << ":  ";
        
        // Print hex values
        for (int i = 0; i < 16 && (addr + i) <= end; i++) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                      << static_cast<int>(ram[addr + i]) << " ";
        }
        
        // Print ASCII representation
        std::cout << " | ";
        for (int i = 0; i < 16 && (addr + i) <= end; i++) {
            uint8_t byte = ram[addr + i];
            if (byte >= 32 && byte <= 126) {
                std::cout << static_cast<char>(byte);
            } else {
                std::cout << ".";
            }
        }
        
        std::cout << std::endl;
    }
    std::cout << std::dec << std::endl;
}

void Memory::reset() {
    std::fill(ram.begin(), ram.end(), 0);
    timer_ctrl = 0;
    console_out = 0;
    console_in = 0;
    timer_value = 0;
    timer_counter = 0;
}

void Memory::updateTimer() {
    if (timer_counter > 0) {
        timer_counter--;
        timer_value = timer_counter;
    }
}

