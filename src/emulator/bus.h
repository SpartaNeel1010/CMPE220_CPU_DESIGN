#ifndef BUS_H
#define BUS_H

#include <cstdint>

/**
 * Bus class - Simulates the system bus connecting CPU and Memory
 * 
 * The bus provides a simple interface for data transfer between
 * the CPU and memory subsystem.
 */
class Bus {
private:
    uint16_t address_bus;  // 16-bit address bus
    uint8_t data_bus;      // 8-bit data bus
    
public:
    Bus();
    
    // Set address on address bus
    void setAddress(uint16_t address);
    
    // Get current address
    uint16_t getAddress() const;
    
    // Set data on data bus
    void setData(uint8_t data);
    
    // Get data from data bus
    uint8_t getData() const;
    
    // Reset bus
    void reset();
};

#endif // BUS_H

