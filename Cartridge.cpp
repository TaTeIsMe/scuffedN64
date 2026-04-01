#include "Cartridge.h"
#include<iostream>
Cartridge::Cartridge(std::vector<uint8_t> rom):mem(rom)
{

}

void Cartridge::write_byte(uint32_t address, uint8_t byte)
{
    mem[address] = byte;
    if(address== 0xB3FF0014- 0xA0000000 - 0x10000000)
    for (uint32_t i = 0xB3FF0020; i < 0xB3FF0220; i++)
    {
        std::cout<<mem[i];
    }
}

uint8_t Cartridge::read_byte(uint32_t address)
{
    return mem[address]; 
}
