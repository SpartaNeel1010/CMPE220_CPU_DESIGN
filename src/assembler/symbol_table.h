#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <map>
#include <cstdint>

/**
 * SymbolTable class - Manages labels and their addresses
 */
class SymbolTable {
private:
    std::map<std::string, uint16_t> symbols;
    
public:
    SymbolTable();
    
    // Add a symbol (label) with its address
    void add(const std::string& name, uint16_t address);
    
    // Check if a symbol exists
    bool contains(const std::string& name) const;
    
    // Get the address of a symbol
    uint16_t get(const std::string& name) const;
    
    // Clear all symbols
    void clear();
    
    // Print all symbols (for debugging)
    void print() const;
};

#endif // SYMBOL_TABLE_H

