#include "Bus.h"

Bus::Bus(uint8_t* memory)
{
    (*this).memory = memory;
}

Bus::~Bus()
{

}

//these might not work depending on endianness
uint32_t Bus::read_word(uint64_t address)
{
    uint32_t word = (read_byte(address)<<24) |
                    (read_byte(address + 1) << 16) | 
                    (read_byte(address + 2) << 8) | 
                    (read_byte(address + 3));
    return word;
}
void Bus::write_word(uint64_t address, uint32_t word)
{
    write_byte(address, (uint8_t)(word >> 24));
    write_byte(address + 1, (uint8_t)(word >> 16));
    write_byte(address + 2, (uint8_t)(word >> 8));
    write_byte(address + 3, (uint8_t)(word));
}

uint16_t Bus::read_halfword(uint64_t address)
{
    uint16_t halfword = (read_byte(address) << 8) | 
                        (read_byte(address + 1));
    return halfword;
}

void Bus::write_halfword(uint64_t address, uint16_t halfword)
{
    write_byte(address, (uint8_t)(halfword >> 8));
    write_byte(address + 1, (uint8_t)(halfword));
}

uint64_t Bus::read_doubleword(uint64_t address)
{
    uint64_t doubleword = (read_byte(address) << 56 ) |
                            (read_byte(address + 1) << 48) | 
                            (read_byte(address + 2) << 40) | 
                            (read_byte(address + 3) << 32) |
                            (read_byte(address + 4) << 24) |
                            (read_byte(address + 5) << 16) | 
                            (read_byte(address + 6) << 8) | 
                            (read_byte(address + 7));
    return doubleword;
}
void Bus::write_doubleword(uint64_t address, uint64_t doubleword)
{
    write_byte(address, (uint8_t)(doubleword >> 56));
    write_byte(address + 1, (uint8_t)(doubleword >> 48));
    write_byte(address + 2, (uint8_t)(doubleword >> 40));
    write_byte(address + 3, (uint8_t)(doubleword >> 32));
    write_byte(address + 4, (uint8_t)(doubleword >> 24));
    write_byte(address + 5, (uint8_t)(doubleword >> 16));
    write_byte(address + 6, (uint8_t)(doubleword >> 8));
    write_byte(address + 7, (uint8_t)(doubleword));
}

uint8_t Bus::read_byte(uint64_t address)
{
    uint8_t byte = memory[address] ;
    return byte;
}

void Bus::write_byte(uint64_t address, uint8_t byte)
{
    memory[address] = byte;
}

uint64_t Bus::read_size(uint64_t address, uint8_t size){
    uint64_t result = 0;
    for (int i = 0; i < size; i++)
    {
        result |= read_byte(address + i) << ((size - 1 - i) * 8);
    }
    return result;
}