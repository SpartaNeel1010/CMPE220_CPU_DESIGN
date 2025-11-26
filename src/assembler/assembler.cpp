#include "assembler.h"
#include "lexer.h"
#include "parser.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

Assembler::Assembler() {
}

bool Assembler::assemble(const std::string& source_file, const std::string& output_file) {
    // Read source file
    std::ifstream file(source_file);
    if (!file) {
        std::cerr << "Error: Cannot open source file '" << source_file << "'" << std::endl;
        return false;
    }
    
    std::string source((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();
    
    std::cout << "\n=== SC8 Assembler ===" << std::endl;
    std::cout << "Source: " << source_file << std::endl;
    
    // Tokenize
    std::cout << "\n[1] Tokenizing..." << std::endl;
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    std::cout << "Generated " << tokens.size() << " tokens" << std::endl;
    
    // Parse (first pass - collect labels)
    std::cout << "\n[2] Parsing (First Pass - Collecting Labels)..." << std::endl;
    Parser parser(tokens, symbols);
    std::vector<Instruction> instructions = parser.parse();
    std::cout << "Parsed " << instructions.size() << " instructions" << std::endl;
    
    // Print symbol table
    symbols.print();
    
    // Generate code (second pass - resolve labels)
    std::cout << "[3] Generating Machine Code..." << std::endl;
    generateCode(instructions);
    std::cout << "Generated " << machine_code.size() << " bytes of machine code" << std::endl;
    
    // Write output file
    std::ofstream output(output_file, std::ios::binary);
    if (!output) {
        std::cerr << "Error: Cannot create output file '" << output_file << "'" << std::endl;
        return false;
    }
    
    output.write(reinterpret_cast<const char*>(machine_code.data()), machine_code.size());
    output.close();
    
    std::cout << "\n[4] Assembly Complete!" << std::endl;
    std::cout << "Output: " << output_file << " (" << machine_code.size() << " bytes)" << std::endl;
    
    return true;
}

bool Assembler::assembleString(const std::string& source, std::vector<uint8_t>& output) {
    // Tokenize
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    // Parse
    Parser parser(tokens, symbols);
    std::vector<Instruction> instructions = parser.parse();
    
    // Generate code
    generateCode(instructions);
    
    output = machine_code;
    return true;
}

void Assembler::generateCode(const std::vector<Instruction>& instructions) {
    machine_code.clear();
    
    for (const auto& instr : instructions) {
        encodeInstruction(instr);
    }
}

void Assembler::encodeInstruction(const Instruction& instr) {
    uint8_t opcode = getOpcode(instr.mnemonic);
    
    if (instr.mnemonic == "NOP") {
        machine_code.push_back(0xFF);  // Use 0xFF to avoid conflict with ADD R0
    }
    else if (instr.mnemonic == "HALT") {
        machine_code.push_back((0x1F << 3) | 0);
    }
    else if (instr.mnemonic == "RET") {
        machine_code.push_back((0x1E << 3) | 0);
    }
    else if (instr.mnemonic == "INC" || instr.mnemonic == "DEC") {
        uint8_t rd = parseRegister(instr.operands[0]);
        machine_code.push_back((opcode << 3) | rd);
    }
    else if (instr.mnemonic == "PUSH" || instr.mnemonic == "POP") {
        uint8_t rd = parseRegister(instr.operands[0]);
        machine_code.push_back((opcode << 3) | rd);
    }
    else if (instr.mnemonic == "LOADI") {
        uint8_t rd = parseRegister(instr.operands[0]);
        uint8_t imm = parseImmediate(instr.operands[1]);
        machine_code.push_back((opcode << 3) | rd);
        machine_code.push_back(imm);
    }
    else if (instr.mnemonic == "LOAD" || instr.mnemonic == "STORE") {
        uint8_t rd = parseRegister(instr.operands[0]);
        uint16_t addr = parseAddress(instr.operands[1]);
        machine_code.push_back((opcode << 3) | rd);
        machine_code.push_back(addr & 0xFF);
        machine_code.push_back((addr >> 8) & 0xFF);
    }
    else if (instr.mnemonic == "JMP" || instr.mnemonic == "JZ" || instr.mnemonic == "JNZ" ||
             instr.mnemonic == "JC" || instr.mnemonic == "JNC" || instr.mnemonic == "CALL") {
        uint16_t addr;
        // Check if operand is a label
        if (symbols.contains(instr.operands[0])) {
            addr = symbols.get(instr.operands[0]);
        } else {
            addr = parseImmediate(instr.operands[0]);
        }
        machine_code.push_back((opcode << 3) | 0);
        machine_code.push_back(addr & 0xFF);
        machine_code.push_back((addr >> 8) & 0xFF);
    }
    else if (instr.mnemonic == "ADDI" || instr.mnemonic == "SUBI" ||
             instr.mnemonic == "ANDI" || instr.mnemonic == "ORI" ||
             instr.mnemonic == "CMPI") {
        uint8_t rd = parseRegister(instr.operands[0]);
        uint8_t imm = parseImmediate(instr.operands[1]);
        machine_code.push_back((opcode << 3) | rd);
        machine_code.push_back(imm);
    }
    else if (instr.mnemonic == "ADD" || instr.mnemonic == "SUB" || instr.mnemonic == "MUL" ||
             instr.mnemonic == "AND" || instr.mnemonic == "OR" || instr.mnemonic == "XOR" ||
             instr.mnemonic == "SHL" || instr.mnemonic == "SHR" || instr.mnemonic == "CMP") {
        uint8_t rd = parseRegister(instr.operands[0]);
        uint8_t rs1 = parseRegister(instr.operands[1]);
        uint8_t rs2 = (instr.operands.size() > 2) ? parseRegister(instr.operands[2]) : 0;
        
        machine_code.push_back((opcode << 3) | rd);
        machine_code.push_back((rs1 << 5) | (rs2 << 2));
    }
    else if (instr.mnemonic == "NOT") {
        uint8_t rd = parseRegister(instr.operands[0]);
        uint8_t rs = parseRegister(instr.operands[1]);
        machine_code.push_back((opcode << 3) | rd);
        machine_code.push_back((rs << 5));
    }
    else {
        error("Unknown instruction: " + instr.mnemonic, instr.line);
    }
}

uint8_t Assembler::parseRegister(const std::string& reg) {
    if (reg.length() != 2 || reg[0] != 'R') {
        std::cerr << "Error: Invalid register: " << reg << std::endl;
        return 0;
    }
    return reg[1] - '0';
}

uint16_t Assembler::parseImmediate(const std::string& imm) {
    try {
        if (imm.substr(0, 2) == "0x" || imm.substr(0, 2) == "0X") {
            return std::stoi(imm, nullptr, 16);
        } else if (imm.substr(0, 2) == "0b" || imm.substr(0, 2) == "0B") {
            return std::stoi(imm.substr(2), nullptr, 2);
        } else {
            return std::stoi(imm);
        }
    } catch (...) {
        std::cerr << "Error: Invalid immediate value: " << imm << std::endl;
        return 0;
    }
}

uint16_t Assembler::parseAddress(const std::string& addr) {
    // Remove brackets if present
    std::string cleaned = addr;
    if (cleaned.front() == '[' && cleaned.back() == ']') {
        cleaned = cleaned.substr(1, cleaned.length() - 2);
    }
    
    // Trim whitespace
    size_t start = cleaned.find_first_not_of(" \t");
    size_t end = cleaned.find_last_not_of(" \t");
    if (start != std::string::npos) {
        cleaned = cleaned.substr(start, end - start + 1);
    }
    
    return parseImmediate(cleaned);
}

uint8_t Assembler::getOpcode(const std::string& mnemonic) {
    if (mnemonic == "NOP") return 0x00;
    if (mnemonic == "ADD") return 0x00;
    if (mnemonic == "ADDI") return 0x01;
    if (mnemonic == "SUB") return 0x02;
    if (mnemonic == "SUBI") return 0x03;
    if (mnemonic == "MUL") return 0x04;
    if (mnemonic == "INC") return 0x05;
    if (mnemonic == "DEC") return 0x06;
    if (mnemonic == "AND") return 0x07;
    if (mnemonic == "ANDI") return 0x08;
    if (mnemonic == "OR") return 0x09;
    if (mnemonic == "ORI") return 0x0A;
    if (mnemonic == "XOR") return 0x0B;
    if (mnemonic == "NOT") return 0x0C;
    if (mnemonic == "SHL") return 0x0D;
    if (mnemonic == "SHR") return 0x0E;
    if (mnemonic == "LOAD") return 0x10;
    if (mnemonic == "STORE") return 0x11;
    if (mnemonic == "LOADI") return 0x12;
    if (mnemonic == "CMP") return 0x13;
    if (mnemonic == "CMPI") return 0x14;
    if (mnemonic == "PUSH") return 0x15;
    if (mnemonic == "POP") return 0x16;
    if (mnemonic == "JMP") return 0x18;
    if (mnemonic == "JZ") return 0x19;
    if (mnemonic == "JNZ") return 0x1A;
    if (mnemonic == "JC") return 0x1B;
    if (mnemonic == "JNC") return 0x1C;
    if (mnemonic == "CALL") return 0x1D;
    if (mnemonic == "RET") return 0x1E;
    if (mnemonic == "HALT") return 0x1F;
    
    return 0xFF; // Unknown
}

void Assembler::error(const std::string& message, int line) {
    std::cerr << "Error at line " << line << ": " << message << std::endl;
}

