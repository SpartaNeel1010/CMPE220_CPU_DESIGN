#include "alu.h"

ALU::ALU() {
}

uint8_t ALU::add(uint8_t a, uint8_t b, uint8_t& flags) {
    uint16_t result = static_cast<uint16_t>(a) + static_cast<uint16_t>(b);
    uint8_t result8 = static_cast<uint8_t>(result);
    
    updateZeroFlag(result8, flags);
    updateNegativeFlag(result8, flags);
    updateCarryFlag(result > 0xFF, flags);
    updateOverflowFlag(a, b, result8, false, flags);
    
    return result8;
}

uint8_t ALU::subtract(uint8_t a, uint8_t b, uint8_t& flags) {
    uint8_t result = a - b;
    
    updateZeroFlag(result, flags);
    updateNegativeFlag(result, flags);
    updateCarryFlag(a < b, flags);  // Carry flag set if borrow needed
    updateOverflowFlag(a, b, result, true, flags);
    
    return result;
}

uint8_t ALU::multiply(uint8_t a, uint8_t b, uint8_t& flags) {
    uint16_t result = static_cast<uint16_t>(a) * static_cast<uint16_t>(b);
    uint8_t result8 = static_cast<uint8_t>(result & 0xFF);  // Lower 8 bits only
    
    updateZeroFlag(result8, flags);
    updateNegativeFlag(result8, flags);
    clearFlags(flags, FLAG_C | FLAG_V);  // Clear carry and overflow for multiply
    
    return result8;
}

uint8_t ALU::increment(uint8_t a, uint8_t& flags) {
    return add(a, 1, flags);
}

uint8_t ALU::decrement(uint8_t a, uint8_t& flags) {
    return subtract(a, 1, flags);
}

uint8_t ALU::logicalAnd(uint8_t a, uint8_t b, uint8_t& flags) {
    uint8_t result = a & b;
    
    updateZeroFlag(result, flags);
    updateNegativeFlag(result, flags);
    clearFlags(flags, FLAG_C | FLAG_V);
    
    return result;
}

uint8_t ALU::logicalOr(uint8_t a, uint8_t b, uint8_t& flags) {
    uint8_t result = a | b;
    
    updateZeroFlag(result, flags);
    updateNegativeFlag(result, flags);
    clearFlags(flags, FLAG_C | FLAG_V);
    
    return result;
}

uint8_t ALU::logicalXor(uint8_t a, uint8_t b, uint8_t& flags) {
    uint8_t result = a ^ b;
    
    updateZeroFlag(result, flags);
    updateNegativeFlag(result, flags);
    clearFlags(flags, FLAG_C | FLAG_V);
    
    return result;
}

uint8_t ALU::logicalNot(uint8_t a, uint8_t& flags) {
    uint8_t result = ~a;
    
    updateZeroFlag(result, flags);
    updateNegativeFlag(result, flags);
    clearFlags(flags, FLAG_C | FLAG_V);
    
    return result;
}

uint8_t ALU::shiftLeft(uint8_t a, uint8_t shift, uint8_t& flags) {
    if (shift == 0) return a;
    
    // Shift left by 'shift' positions
    uint8_t result = a << shift;
    
    // Carry is the last bit shifted out
    bool carry = (shift <= 8) && ((a & (0x80 >> (shift - 1))) != 0);
    
    updateZeroFlag(result, flags);
    updateNegativeFlag(result, flags);
    updateCarryFlag(carry, flags);
    clearFlags(flags, FLAG_V);
    
    return result;
}

uint8_t ALU::shiftRight(uint8_t a, uint8_t shift, uint8_t& flags) {
    if (shift == 0) return a;
    
    // Shift right by 'shift' positions (logical shift)
    uint8_t result = a >> shift;
    
    // Carry is the last bit shifted out
    bool carry = (shift <= 8) && ((a & (1 << (shift - 1))) != 0);
    
    updateZeroFlag(result, flags);
    updateNegativeFlag(result, flags);
    updateCarryFlag(carry, flags);
    clearFlags(flags, FLAG_V);
    
    return result;
}

void ALU::compare(uint8_t a, uint8_t b, uint8_t& flags) {
    // Comparison is just subtraction without storing the result
    subtract(a, b, flags);
}

// Helper functions

void ALU::updateZeroFlag(uint8_t result, uint8_t& flags) {
    if (result == 0) {
        flags |= FLAG_Z;
    } else {
        flags &= ~FLAG_Z;
    }
}

void ALU::updateNegativeFlag(uint8_t result, uint8_t& flags) {
    if (result & 0x80) {
        flags |= FLAG_N;
    } else {
        flags &= ~FLAG_N;
    }
}

void ALU::updateCarryFlag(bool carry, uint8_t& flags) {
    if (carry) {
        flags |= FLAG_C;
    } else {
        flags &= ~FLAG_C;
    }
}

void ALU::updateOverflowFlag(uint8_t a, uint8_t b, uint8_t result, bool isSubtraction, uint8_t& flags) {
    // Overflow occurs when:
    // - Addition: two positive numbers produce negative, or two negative produce positive
    // - Subtraction: positive - negative = negative, or negative - positive = positive
    
    bool a_sign = (a & 0x80) != 0;
    bool b_sign = (b & 0x80) != 0;
    bool result_sign = (result & 0x80) != 0;
    
    bool overflow;
    if (isSubtraction) {
        // For subtraction: overflow if signs of a and b are different and result sign != a sign
        overflow = (a_sign != b_sign) && (a_sign != result_sign);
    } else {
        // For addition: overflow if signs of a and b are same and result sign is different
        overflow = (a_sign == b_sign) && (a_sign != result_sign);
    }
    
    if (overflow) {
        flags |= FLAG_V;
    } else {
        flags &= ~FLAG_V;
    }
}

void ALU::clearFlags(uint8_t& flags, uint8_t mask) {
    flags &= ~mask;
}

