#include "RSP.h"

void RSP::RSPRegs::write_size(uint32_t address, uint64_t value, uint8_t size)
{
}

uint64_t RSP::RSPRegs::read_size(uint32_t address, uint8_t size)
{
    return 0;
}

void RSP::Imem::write_size(uint32_t address, uint64_t value, uint8_t size)
{
}

uint64_t RSP::Imem::read_size(uint32_t address, uint8_t size)
{
    return 0;
}

RSP::Dmem::Dmem():mem(0x1000){}
RSP::Imem::Imem():mem(0x1000){}

void RSP::Dmem::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    if((address + size - 1) >= mem.size())return;
    
    for (uint8_t i = 0; i < size; i++)
    {
        mem[address + i] = (value >> ((size - 1 - i) * 8)) & 0xFF;
    }
}

uint64_t RSP::Dmem::read_size(uint32_t address, uint8_t size)
{
    if((address + size - 1) >= mem.size()) return 0;
    uint64_t result = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        result |= (uint64_t)mem[(address + i)] << ((size - 1 - i) * 8);
    }
    return result;
}
