#include "parser.h"
#include <iostream>

Parser::Parser(const std::vector<Token>& toks, SymbolTable& syms) 
    : tokens(toks), position(0), symbols(syms) {
}

std::vector<Instruction> Parser::parse() {
    std::vector<Instruction> instructions;
    uint16_t address = 0x0100;  // Start address
    
    skipNewlines();
    
    while (!isAtEnd()) {
        // Check for label
        if (current().type == TokenType::IDENTIFIER && 
            position + 1 < tokens.size() && 
            peek().type == TokenType::COLON) {
            parseLabel(address);
            skipNewlines();
            continue;
        }
        
        // Check for instruction
        if (current().type == TokenType::INSTRUCTION) {
            Instruction instr = parseInstruction(address);
            instructions.push_back(instr);
            skipNewlines();
            continue;
        }
        
        // Skip newlines
        if (current().type == TokenType::NEWLINE) {
            advance();
            continue;
        }
        
        // Skip unexpected tokens with warning
        if (current().type != TokenType::END_OF_FILE) {
            error("Unexpected token: " + current().value);
        }
        advance();
    }
    
    return instructions;
}

void Parser::parseLabel(uint16_t& address) {
    std::string label = current().value;
    advance();  // Skip identifier
    advance();  // Skip colon
    
    symbols.add(label, address);
    
    std::cout << "Label '" << label << "' at address 0x" << std::hex << address << std::dec << std::endl;
}

Instruction Parser::parseInstruction(uint16_t& address) {
    std::string mnemonic = current().value;
    int line = current().line;
    Instruction instr(mnemonic, line, address);
    
    advance();  // Skip instruction mnemonic
    
    // Parse operands
    while (!isAtEnd() && current().type != TokenType::NEWLINE && 
           current().type != TokenType::END_OF_FILE) {
        
        if (current().type == TokenType::COMMA) {
            advance();
            continue;
        }
        
        // Add operand
        instr.operands.push_back(current().value);
        advance();
    }
    
    // Calculate instruction size and update address
    if (mnemonic == "NOP") {
        address += 1;
    } else if (mnemonic == "HALT" || mnemonic == "RET" || 
               mnemonic == "INC" || mnemonic == "DEC" ||
               mnemonic == "PUSH" || mnemonic == "POP") {
        address += 1;
    } else if (mnemonic == "LOAD" || mnemonic == "STORE" ||
               mnemonic == "JMP" || mnemonic == "JZ" || mnemonic == "JNZ" ||
               mnemonic == "JC" || mnemonic == "JNC" || mnemonic == "CALL") {
        address += 3;
    } else {
        address += 2;  // Most instructions are 2 bytes
    }
    
    return instr;
}

Token Parser::current() const {
    if (position >= tokens.size()) return tokens.back();
    return tokens[position];
}

Token Parser::peek(int offset) const {
    if (position + offset >= tokens.size()) return tokens.back();
    return tokens[position + offset];
}

void Parser::advance() {
    if (position < tokens.size()) position++;
}

bool Parser::isAtEnd() const {
    return position >= tokens.size() || tokens[position].type == TokenType::END_OF_FILE;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return current().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

void Parser::expect(TokenType type, const std::string& message) {
    if (!check(type)) {
        error(message);
    }
    advance();
}

void Parser::skipNewlines() {
    while (!isAtEnd() && current().type == TokenType::NEWLINE) {
        advance();
    }
}

void Parser::error(const std::string& message) {
    std::cerr << "Parse error at line " << current().line 
              << ", column " << current().column << ": " 
              << message << std::endl;
}

