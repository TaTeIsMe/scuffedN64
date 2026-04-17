#include "Cartridge.h"
#include<iostream>
Cartridge::Cartridge(std::vector<uint8_t> rom):mem(rom){}

void Cartridge::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    static std::vector<uint8_t> buffer(0x200, 0);
    if(address >= 0x03FF0020 && address <= 0x03FF0220){
            buffer[address - 0x03FF0020 + 0] = uint8_t(value >> 24);
            buffer[address - 0x03FF0020 + 1] = uint8_t(value >> 16);
            buffer[address - 0x03FF0020 + 2] = uint8_t(value >> 8);
            buffer[address - 0x03FF0020 + 3] = uint8_t(value >> 0);
    }
    if(address == 0x3FF0014){
        for (uint16_t i = 0; i < buffer.size(); i++)
        {
            std::cout<<(char)buffer[i];
        }
        std::fill(buffer.begin(), buffer.end(), 0);
        
    }
    
    if((address + size - 1) >= mem.size())return;
    
    for (uint8_t i = 0; i < size; i++)
    {
        mem[address + i] = (value >> ((size - 1 - i) * 8)) & 0xFF;
    }
}

uint64_t Cartridge::read_size(uint32_t address, uint8_t size)
{
    if((address + size - 1) >= mem.size()) return 0;
    uint64_t result = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        result |= (uint64_t)mem[(address + i)] << ((size - 1 - i) * 8);
    }
    return result;
}
