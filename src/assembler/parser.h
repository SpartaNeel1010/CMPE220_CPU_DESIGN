#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <cstdint>
#include "lexer.h"
#include "symbol_table.h"

/**
 * Instruction structure - Represents a parsed instruction
 */
struct Instruction {
    std::string mnemonic;
    std::vector<std::string> operands;
    int line;
    uint16_t address;
    
    Instruction(const std::string& mn, int ln, uint16_t addr) 
        : mnemonic(mn), line(ln), address(addr) {}
};

/**
 * Parser class - Parses tokens into instructions
 */
class Parser {
private:
    std::vector<Token> tokens;
    size_t position;
    SymbolTable& symbols;
    
public:
    Parser(const std::vector<Token>& toks, SymbolTable& syms);
    
    // Parse tokens into instructions
    std::vector<Instruction> parse();
    
private:
    // Helper functions
    Token current() const;
    Token peek(int offset = 1) const;
    void advance();
    bool isAtEnd() const;
    bool check(TokenType type) const;
    bool match(TokenType type);
    void expect(TokenType type, const std::string& message);
    void skipNewlines();
    
    // Parsing functions
    void parseLabel(uint16_t& address);
    Instruction parseInstruction(uint16_t& address);
    
    // Error reporting
    void error(const std::string& message);
};

#endif // PARSER_H

