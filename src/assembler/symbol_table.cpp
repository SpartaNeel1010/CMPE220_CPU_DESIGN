#include "symbol_table.h"
#include <iostream>
#include <iomanip>

SymbolTable::SymbolTable() {
}

void SymbolTable::add(const std::string& name, uint16_t address) {
    symbols[name] = address;
}

bool SymbolTable::contains(const std::string& name) const {
    return symbols.find(name) != symbols.end();
}

uint16_t SymbolTable::get(const std::string& name) const {
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        return it->second;
    }
    return 0;
}

void SymbolTable::clear() {
    symbols.clear();
}

void SymbolTable::print() const {
    std::cout << "\n=== Symbol Table ===" << std::endl;
    std::cout << "Label                Address" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    
    for (const auto& pair : symbols) {
        std::cout << std::left << std::setw(20) << pair.first 
                  << " 0x" << std::hex << std::setw(4) << std::setfill('0') 
                  << pair.second << std::dec << std::endl;
    }
    
    std::cout << std::endl;
}

