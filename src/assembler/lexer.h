#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

/**
 * Token types for assembly language
 */
enum class TokenType {
    INSTRUCTION,   // Instruction mnemonic (ADD, SUB, etc.)
    REGISTER,      // Register (R0-R7)
    IMMEDIATE,     // Immediate value (number)
    LABEL,         // Label (identifier:)
    IDENTIFIER,    // Identifier (for labels in instructions)
    ADDRESS,       // Memory address [0x1000]
    COMMA,         // ,
    COLON,         // :
    LBRACKET,      // [
    RBRACKET,      // ]
    NEWLINE,       // End of line
    END_OF_FILE,   // End of file
    UNKNOWN        // Unknown token
};

/**
 * Token structure
 */
struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    
    Token(TokenType t, const std::string& v, int l, int c) 
        : type(t), value(v), line(l), column(c) {}
};

/**
 * Lexer class - Tokenizes assembly source code
 */
class Lexer {
private:
    std::string source;
    size_t position;
    int line;
    int column;
    
public:
    Lexer(const std::string& src);
    
    // Tokenize the entire source
    std::vector<Token> tokenize();
    
private:
    // Helper functions
    char current() const;
    char peek(int offset = 1) const;
    void advance();
    void skipWhitespace();
    void skipComment();
    
    Token nextToken();
    Token readNumber();
    Token readIdentifier();
    Token readAddress();
    
    bool isAtEnd() const;
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isHexDigit(char c) const;
};

#endif // LEXER_H

