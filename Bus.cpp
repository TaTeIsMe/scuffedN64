#include "Bus.h"

Bus::Bus(uint8_t* memory)
{
    (*this).memory = memory;
}

Bus::~Bus()
{

}

uint32_t Bus::read_word(uint64_t address)
{
    uint32_t word = ((uint32_t)memory[address] << 24) | ((uint32_t)memory[address + 1] << 16) | ((uint32_t)memory[address + 2] << 8) | (uint32_t)memory[address + 3] ;
    return word;
}

uint32_t Bus::write_word(uint64_t address, uint32_t word)
{
    memory[address] = word >> 24 & 0xFF;
    memory[address+1] = word>>16 & 0xFF;
    memory[address+2] = word>>8 & 0xFF;
    memory[address+3] = word & 0xFF;
    return word;
}

uint32_t Bus::write_byte(uint64_t address, uint8_t byte)
{
    memory[address] = byte;
    return byte;
}
