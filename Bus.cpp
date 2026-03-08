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
