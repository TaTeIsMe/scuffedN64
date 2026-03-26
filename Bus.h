#pragma once
#include<cstdint>

class Bus
{
public:
    Bus(uint8_t* memory);
    ~Bus();
    uint8_t* memory;

    uint32_t read_word(uint64_t address);
    void write_word(uint64_t address, uint32_t word);
    uint64_t read_doubleword(uint64_t address);
    void write_doubleword(uint64_t address, uint64_t doubleword);
    uint8_t read_byte(uint64_t address);
    void write_byte(uint64_t address, uint8_t byte);
    uint16_t read_halfword(uint64_t address);
    void write_halfword(uint64_t address, uint16_t halfword);
    uint64_t read_size(uint64_t address, uint8_t size);
    void write_size(uint64_t address, uint64_t value, uint8_t size);


private:
};
