#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include "cpu.h"
#include "memory.h"

void printUsage(const char* program) {
    std::cout << "SC8 CPU Emulator" << std::endl;
    std::cout << "Usage: " << program << " [options] <binary_file>" << std::endl;
    std::cout << "\nOptions:" << std::endl;
    std::cout << "  -d, --debug       Enable debug mode (step-by-step execution)" << std::endl;
    std::cout << "  -m, --dump-memory Dump memory after execution" << std::endl;
    std::cout << "  -s, --start ADDR  Set program start address (default: 0x0100)" << std::endl;
    std::cout << "  -h, --help        Show this help message" << std::endl;
    std::cout << "\nExample:" << std::endl;
    std::cout << "  " << program << " program.bin" << std::endl;
    std::cout << "  " << program << " -d program.bin" << std::endl;
}

std::vector<uint8_t> loadBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open file '" << filename << "'" << std::endl;
        return {};
    }
    
    // Read entire file
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    
    if (!file) {
        std::cerr << "Error: Failed to read file '" << filename << "'" << std::endl;
        return {};
    }
    
    file.close();
    return buffer;
}

int main(int argc, char* argv[]) {
    bool debug = false;
    bool dump_memory = false;
    uint16_t start_address = 0x0100;
    std::string binary_file;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-d" || arg == "--debug") {
            debug = true;
        } else if (arg == "-m" || arg == "--dump-memory") {
            dump_memory = true;
        } else if (arg == "-s" || arg == "--start") {
            if (i + 1 < argc) {
                start_address = std::stoi(argv[++i], nullptr, 16);
            } else {
                std::cerr << "Error: -s option requires an address" << std::endl;
                return 1;
            }
        } else if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg[0] == '-') {
            std::cerr << "Error: Unknown option '" << arg << "'" << std::endl;
            printUsage(argv[0]);
            return 1;
        } else {
            binary_file = arg;
        }
    }
    
    if (binary_file.empty()) {
        std::cerr << "Error: No binary file specified" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    // Load binary file
    std::vector<uint8_t> program = loadBinaryFile(binary_file);
    if (program.empty()) {
        return 1;
    }
    
    std::cout << "\n=== SC8 CPU Emulator ===" << std::endl;
    std::cout << "Program: " << binary_file << std::endl;
    std::cout << "Size: " << program.size() << " bytes" << std::endl;
    std::cout << "Start address: 0x" << std::hex << std::setw(4) 
              << std::setfill('0') << start_address << std::dec << std::endl;
    std::cout << "Debug mode: " << (debug ? "ON" : "OFF") << std::endl;
    std::cout << std::endl;
    
    // Create memory and CPU
    Memory memory;
    CPU cpu(&memory);
    
    // Load program into memory
    memory.loadProgram(program, start_address);
    
    // Enable debug mode if requested
    if (debug) {
        cpu.enableDebug(true);
        std::cout << "=== Starting Debug Mode ===" << std::endl;
        std::cout << "Press Enter to step through each instruction..." << std::endl;
    }
    
    // Run the CPU
    if (debug) {
        // Step-by-step execution
        cpu.printState();
        while (!cpu.isHalted()) {
            std::cout << "\n--- Press Enter to execute next instruction ---";
            std::cin.get();
            cpu.step();
        }
    } else {
        // Run until halt
        cpu.run();
    }
    
    // Print final state
    std::cout << "\n=== Final CPU State ===" << std::endl;
    cpu.printState();
    
    // Dump memory if requested
    if (dump_memory) {
        std::cout << "\n=== Memory Dump ===" << std::endl;
        std::cout << "\nProgram area:" << std::endl;
        memory.dump(start_address, start_address + std::min(static_cast<int>(program.size()) + 64, 256));
        
        std::cout << "\nData area (0x1000-0x10FF):" << std::endl;
        memory.dump(0x1000, 0x10FF);
        
        std::cout << "\nStack area (0xFE00-0xFEFF):" << std::endl;
        memory.dump(0xFE00, 0xFEFF);
    }
    
    std::cout << "\nExecution completed successfully." << std::endl;
    return 0;
}

