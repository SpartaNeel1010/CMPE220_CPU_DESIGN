#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <vector>
#include <cstdint>
#include "parser.h"
#include "symbol_table.h"

/**
 * Assembler class - Main assembler that converts assembly to machine code
 */
class Assembler {
private:
    SymbolTable symbols;
    std::vector<uint8_t> machine_code;
    
public:
    Assembler();
    
    // Assemble a source file
    bool assemble(const std::string& source_file, const std::string& output_file);
    
    // Assemble from source string
    bool assembleString(const std::string& source, std::vector<uint8_t>& output);
    
private:
    // Code generation
    void generateCode(const std::vector<Instruction>& instructions);
    void encodeInstruction(const Instruction& instr);
    
    // Helper functions
    uint8_t parseRegister(const std::string& reg);
    uint16_t parseImmediate(const std::string& imm);
    uint16_t parseAddress(const std::string& addr);
    
    // Opcode mapping
    uint8_t getOpcode(const std::string& mnemonic);
    
    // Error reporting
    void error(const std::string& message, int line);
};

#endif // ASSEMBLER_H

