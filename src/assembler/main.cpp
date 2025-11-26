#include <iostream>
#include <string>
#include "assembler.h"

void printUsage(const char* program) {
    std::cout << "SC8 Assembler" << std::endl;
    std::cout << "Usage: " << program << " <source_file> [output_file]" << std::endl;
    std::cout << "\nArguments:" << std::endl;
    std::cout << "  source_file   - Assembly source file (.asm)" << std::endl;
    std::cout << "  output_file   - Output binary file (.bin) [optional]" << std::endl;
    std::cout << "\nExample:" << std::endl;
    std::cout << "  " << program << " program.asm program.bin" << std::endl;
    std::cout << "  " << program << " program.asm" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string source_file = argv[1];
    std::string output_file;
    
    if (argc >= 3) {
        output_file = argv[2];
    } else {
        // Generate output filename from source filename
        size_t pos = source_file.find_last_of('.');
        if (pos != std::string::npos) {
            output_file = source_file.substr(0, pos) + ".bin";
        } else {
            output_file = source_file + ".bin";
        }
    }
    
    Assembler assembler;
    
    if (!assembler.assemble(source_file, output_file)) {
        std::cerr << "\nAssembly failed!" << std::endl;
        return 1;
    }
    
    return 0;
}

