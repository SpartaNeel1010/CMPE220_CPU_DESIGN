#ifndef ALU_H
#define ALU_H

#include <cstdint>

/**
 * ALU class - Arithmetic Logic Unit
 * Performs arithmetic and logical operations
 * Updates CPU flags based on results
 */
class ALU {
public:
    // Flag bits
    static const uint8_t FLAG_N = 0x80;  // Negative (bit 7)
    static const uint8_t FLAG_Z = 0x40;  // Zero (bit 6)
    static const uint8_t FLAG_C = 0x20;  // Carry (bit 5)
    static const uint8_t FLAG_V = 0x10;  // Overflow (bit 4)
    
    ALU();
    
    // Arithmetic operations
    uint8_t add(uint8_t a, uint8_t b, uint8_t& flags);
    uint8_t subtract(uint8_t a, uint8_t b, uint8_t& flags);
    uint8_t multiply(uint8_t a, uint8_t b, uint8_t& flags);
    uint8_t increment(uint8_t a, uint8_t& flags);
    uint8_t decrement(uint8_t a, uint8_t& flags);
    
    // Logical operations
    uint8_t logicalAnd(uint8_t a, uint8_t b, uint8_t& flags);
    uint8_t logicalOr(uint8_t a, uint8_t b, uint8_t& flags);
    uint8_t logicalXor(uint8_t a, uint8_t b, uint8_t& flags);
    uint8_t logicalNot(uint8_t a, uint8_t& flags);
    
    // Shift operations
    uint8_t shiftLeft(uint8_t a, uint8_t shift, uint8_t& flags);
    uint8_t shiftRight(uint8_t a, uint8_t shift, uint8_t& flags);
    
    // Comparison (updates flags only, returns 0)
    void compare(uint8_t a, uint8_t b, uint8_t& flags);
    
private:
    // Helper functions for flag updates
    void updateZeroFlag(uint8_t result, uint8_t& flags);
    void updateNegativeFlag(uint8_t result, uint8_t& flags);
    void updateCarryFlag(bool carry, uint8_t& flags);
    void updateOverflowFlag(uint8_t a, uint8_t b, uint8_t result, bool isSubtraction, uint8_t& flags);
    void clearFlags(uint8_t& flags, uint8_t mask);
};

#endif // ALU_H

