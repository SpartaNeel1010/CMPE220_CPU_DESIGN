#include "lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& src) : source(src), position(0), line(1), column(1) {
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (!isAtEnd()) {
        skipWhitespace();
        
        if (isAtEnd()) break;
        
        // Skip comments
        if (current() == ';') {
            skipComment();
            continue;
        }
        
        Token token = nextToken();
        
        // Only add meaningful tokens
        if (token.type != TokenType::UNKNOWN) {
            tokens.push_back(token);
        }
    }
    
    tokens.push_back(Token(TokenType::END_OF_FILE, "", line, column));
    return tokens;
}

Token Lexer::nextToken() {
    char c = current();
    
    // Handle newlines
    if (c == '\n') {
        Token token(TokenType::NEWLINE, "\\n", line, column);
        advance();
        return token;
    }
    
    // Handle punctuation
    if (c == ',') {
        Token token(TokenType::COMMA, ",", line, column);
        advance();
        return token;
    }
    
    if (c == ':') {
        Token token(TokenType::COLON, ":", line, column);
        advance();
        return token;
    }
    
    if (c == '[') {
        return readAddress();
    }
    
    // Handle numbers (immediate values)
    if (isDigit(c) || (c == '-' && isDigit(peek()))) {
        return readNumber();
    }
    
    // Handle hex numbers
    if (c == '0' && (peek() == 'x' || peek() == 'X')) {
        return readNumber();
    }
    
    // Handle identifiers, instructions, and registers
    if (isAlpha(c) || c == '_') {
        return readIdentifier();
    }
    
    // Unknown character
    Token token(TokenType::UNKNOWN, std::string(1, c), line, column);
    advance();
    return token;
}

Token Lexer::readNumber() {
    int start_line = line;
    int start_column = column;
    std::string number;
    
    // Handle negative sign
    if (current() == '-') {
        number += current();
        advance();
    }
    
    // Handle hexadecimal
    if (current() == '0' && (peek() == 'x' || peek() == 'X')) {
        number += current();
        advance();
        number += current();
        advance();
        
        while (isHexDigit(current())) {
            number += current();
            advance();
        }
    }
    // Handle binary
    else if (current() == '0' && (peek() == 'b' || peek() == 'B')) {
        number += current();
        advance();
        number += current();
        advance();
        
        while (current() == '0' || current() == '1') {
            number += current();
            advance();
        }
    }
    // Handle decimal
    else {
        while (isDigit(current())) {
            number += current();
            advance();
        }
    }
    
    return Token(TokenType::IMMEDIATE, number, start_line, start_column);
}

Token Lexer::readIdentifier() {
    int start_line = line;
    int start_column = column;
    std::string identifier;
    
    while (isAlphaNumeric(current()) || current() == '_') {
        identifier += current();
        advance();
    }
    
    // Check if it's a register (R0-R7)
    if (identifier.length() == 2 && identifier[0] == 'R' && 
        identifier[1] >= '0' && identifier[1] <= '7') {
        return Token(TokenType::REGISTER, identifier, start_line, start_column);
    }
    
    // Check if it's SP (alias for R7)
    if (identifier == "SP") {
        return Token(TokenType::REGISTER, "R7", start_line, start_column);
    }
    
    // List of known instructions
    const char* instructions[] = {
        "ADD", "ADDI", "SUB", "SUBI", "MUL", "INC", "DEC",
        "AND", "ANDI", "OR", "ORI", "XOR", "NOT", "SHL", "SHR",
        "LOAD", "STORE", "LOADI",
        "CMP", "CMPI",
        "PUSH", "POP",
        "JMP", "JZ", "JNZ", "JC", "JNC", "CALL", "RET",
        "NOP", "HALT"
    };
    
    // Check if it's an instruction
    for (const char* instr : instructions) {
        if (identifier == instr) {
            return Token(TokenType::INSTRUCTION, identifier, start_line, start_column);
        }
    }
    
    // Otherwise, it's an identifier (label reference)
    return Token(TokenType::IDENTIFIER, identifier, start_line, start_column);
}

Token Lexer::readAddress() {
    int start_line = line;
    int start_column = column;
    std::string address = "[";
    
    advance(); // Skip '['
    
    skipWhitespace();
    
    // Read the address value (number or identifier)
    while (current() != ']' && !isAtEnd()) {
        address += current();
        advance();
    }
    
    if (current() == ']') {
        address += ']';
        advance();
    }
    
    return Token(TokenType::ADDRESS, address, start_line, start_column);
}

char Lexer::current() const {
    if (isAtEnd()) return '\0';
    return source[position];
}

char Lexer::peek(int offset) const {
    if (position + offset >= source.length()) return '\0';
    return source[position + offset];
}

void Lexer::advance() {
    if (isAtEnd()) return;
    
    if (current() == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    
    position++;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && (current() == ' ' || current() == '\t' || current() == '\r')) {
        advance();
    }
}

void Lexer::skipComment() {
    // Skip until end of line
    while (!isAtEnd() && current() != '\n') {
        advance();
    }
}

bool Lexer::isAtEnd() const {
    return position >= source.length();
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool Lexer::isHexDigit(char c) const {
    return isDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

