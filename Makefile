# Makefile for SC8 CPU Project
# Team: Neel Asheshbhai Shah, Vedant Tushar Shah, Aarav Pranav Shah, Harshavardhan Kuruvella

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
INCLUDE = -Isrc/emulator -Isrc/assembler

# Directories
SRC_EMU = src/emulator
SRC_ASM = src/assembler
BIN_DIR = bin
PROG_DIR = programs

# Emulator source files
EMU_SOURCES = $(SRC_EMU)/main.cpp $(SRC_EMU)/cpu.cpp $(SRC_EMU)/alu.cpp \
              $(SRC_EMU)/memory.cpp $(SRC_EMU)/bus.cpp

# Assembler source files
ASM_SOURCES = $(SRC_ASM)/main.cpp $(SRC_ASM)/assembler.cpp $(SRC_ASM)/parser.cpp \
              $(SRC_ASM)/lexer.cpp $(SRC_ASM)/symbol_table.cpp

# Output binaries
EMULATOR = $(BIN_DIR)/emulator
ASSEMBLER = $(BIN_DIR)/assembler

# Assembly programs
ASM_PROGRAMS = $(PROG_DIR)/timer.asm $(PROG_DIR)/hello_world.asm $(PROG_DIR)/fibonacci.asm
BIN_PROGRAMS = $(PROG_DIR)/timer.bin $(PROG_DIR)/hello_world.bin $(PROG_DIR)/fibonacci.bin

# Colors for output
GREEN = \033[0;32m
BLUE = \033[0;34m
NC = \033[0m # No Color

.PHONY: all clean emulator assembler programs test run-hello run-fib run-timer help

# Default target - build everything
all: emulator assembler programs
	@echo "$(GREEN)✓ Build complete!$(NC)"
	@echo "$(BLUE)Run 'make help' for usage information$(NC)"

# Build emulator
emulator: $(EMULATOR)

$(EMULATOR): $(EMU_SOURCES)
	@echo "$(BLUE)Building emulator...$(NC)"
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(EMU_SOURCES) -o $(EMULATOR)
	@echo "$(GREEN)✓ Emulator built: $(EMULATOR)$(NC)"

# Build assembler
assembler: $(ASSEMBLER)

$(ASSEMBLER): $(ASM_SOURCES)
	@echo "$(BLUE)Building assembler...$(NC)"
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(ASM_SOURCES) -o $(ASSEMBLER)
	@echo "$(GREEN)✓ Assembler built: $(ASSEMBLER)$(NC)"

# Assemble programs
programs: $(BIN_PROGRAMS)

$(PROG_DIR)/%.bin: $(PROG_DIR)/%.asm $(ASSEMBLER)
	@echo "$(BLUE)Assembling $<...$(NC)"
	./$(ASSEMBLER) $< $@
	@echo "$(GREEN)✓ Assembled: $@$(NC)"

# Run programs
run-hello: $(EMULATOR) $(PROG_DIR)/hello_world.bin
	@echo "$(BLUE)Running Hello World program...$(NC)"
	@echo "$(BLUE)================================$(NC)"
	./$(EMULATOR) $(PROG_DIR)/hello_world.bin

run-fib: $(EMULATOR) $(PROG_DIR)/fibonacci.bin
	@echo "$(BLUE)Running Fibonacci program...$(NC)"
	@echo "$(BLUE)================================$(NC)"
	./$(EMULATOR) $(PROG_DIR)/fibonacci.bin

run-fibonacci: run-fib

run-timer: $(EMULATOR) $(PROG_DIR)/timer.bin
	@echo "$(BLUE)Running Timer program...$(NC)"
	@echo "$(BLUE)================================$(NC)"
	./$(EMULATOR) $(PROG_DIR)/timer.bin

# Run all programs as a test
test: programs
	@echo "$(BLUE)Testing all programs...$(NC)"
	@echo ""
	@echo "$(BLUE)==== Test 1: Hello World =====$(NC)"
	./$(EMULATOR) $(PROG_DIR)/hello_world.bin
	@echo ""
	@echo "$(BLUE)==== Test 2: Fibonacci =====$(NC)"
	./$(EMULATOR) $(PROG_DIR)/fibonacci.bin
	@echo ""
	@echo "$(BLUE)==== Test 3: Timer =====$(NC)"
	./$(EMULATOR) $(PROG_DIR)/timer.bin
	@echo ""
	@echo "$(GREEN)✓ All tests completed!$(NC)"

# Debug mode (step-by-step execution)
debug-hello: $(EMULATOR) $(PROG_DIR)/hello_world.bin
	@echo "$(BLUE)Running Hello World in debug mode...$(NC)"
	./$(EMULATOR) -d $(PROG_DIR)/hello_world.bin

debug-fib: $(EMULATOR) $(PROG_DIR)/fibonacci.bin
	@echo "$(BLUE)Running Fibonacci in debug mode...$(NC)"
	./$(EMULATOR) -d $(PROG_DIR)/fibonacci.bin

debug-timer: $(EMULATOR) $(PROG_DIR)/timer.bin
	@echo "$(BLUE)Running Timer in debug mode...$(NC)"
	./$(EMULATOR) -d $(PROG_DIR)/timer.bin

# Clean build artifacts
clean:
	@echo "$(BLUE)Cleaning build artifacts...$(NC)"
	rm -rf $(BIN_DIR)
	rm -f $(BIN_PROGRAMS)
	rm -f $(PROG_DIR)/*.bin
	@echo "$(GREEN)✓ Clean complete$(NC)"

# Help message
help:
	@echo "$(BLUE)SC8 CPU Project - Makefile Help$(NC)"
	@echo ""
	@echo "Available targets:"
	@echo "  $(GREEN)make all$(NC)           - Build emulator, assembler, and assemble programs"
	@echo "  $(GREEN)make emulator$(NC)      - Build the CPU emulator"
	@echo "  $(GREEN)make assembler$(NC)     - Build the assembler"
	@echo "  $(GREEN)make programs$(NC)      - Assemble all .asm programs to .bin"
	@echo ""
	@echo "Running programs:"
	@echo "  $(GREEN)make run-hello$(NC)     - Run Hello World program"
	@echo "  $(GREEN)make run-fib$(NC)       - Run Fibonacci program"
	@echo "  $(GREEN)make run-timer$(NC)     - Run Timer program"
	@echo "  $(GREEN)make test$(NC)          - Run all programs (test suite)"
	@echo ""
	@echo "Debug mode:"
	@echo "  $(GREEN)make debug-hello$(NC)   - Run Hello World with step-by-step debugging"
	@echo "  $(GREEN)make debug-fib$(NC)     - Run Fibonacci with step-by-step debugging"
	@echo "  $(GREEN)make debug-timer$(NC)   - Run Timer with step-by-step debugging"
	@echo ""
	@echo "Other:"
	@echo "  $(GREEN)make clean$(NC)         - Remove all build artifacts"
	@echo "  $(GREEN)make help$(NC)          - Show this help message"
	@echo ""
	@echo "Example workflow:"
	@echo "  1. $(GREEN)make all$(NC)         - Build everything"
	@echo "  2. $(GREEN)make run-fib$(NC)     - Run Fibonacci program"
	@echo "  3. $(GREEN)make clean$(NC)       - Clean up when done"

