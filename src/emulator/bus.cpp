#include "bus.h"

Bus::Bus() : address_bus(0), data_bus(0) {
}

void Bus::setAddress(uint16_t address) {
    address_bus = address;
}

uint16_t Bus::getAddress() const {
    return address_bus;
}

void Bus::setData(uint8_t data) {
    data_bus = data;
}

uint8_t Bus::getData() const {
    return data_bus;
}

void Bus::reset() {
    address_bus = 0;
    data_bus = 0;
}

