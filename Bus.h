#pragma once
#include<cstdint>

class Bus
{
public:
    Bus(uint8_t* memory);
    ~Bus();
    uint8_t* memory;

    uint32_t read_word(uint64_t address);
    uint32_t write_word(uint64_t address, uint32_t word);

private:
};
