#include "Rdram.h"

Rdram::Rdram():mem(4*1024*1024){}

void Rdram::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    if((address + size - 1) >= mem.size())return;
    
    for (uint8_t i = 0; i < size; i++)
    {
        mem[address + i] = (value >> ((size - 1 - i) * 8)) & 0xFF;
    }
}

uint64_t Rdram::read_size(uint32_t address, uint8_t size)
{
    if((address + size - 1) >= mem.size()) return 0;
    uint64_t result = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        result |= (uint64_t)mem[(address + i)] << ((size - 1 - i) * 8);
    }
    return result;
}


//these two don't take into account broadcast aaddresses
void Rdram::RdramRegs::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    //thsi doesn't handle any special behaviour todo
        //this assumes the access doesn't go accross two regs
    address &= 0x1F; //mirroring
    uint32_t reg_id = address >> 2;
    uint32_t shift = (4 - size - (address & 3)) * 8;
    uint64_t mask = ((1ULL << (size * 8)) - 1) << shift;
    uint32_t prev_val = regs[reg_id];
    regs[reg_id] = (prev_val & ~mask) | ((value << shift) & mask);
}

uint64_t Rdram::RdramRegs::read_size(uint32_t address, uint8_t size)
{
    //thsi doesn't handle any special behaviour todo
        //this assumes the access doesn't go accross two regs
    address = address & 0x1F; //mirroring
    uint32_t reg_id = address >> 2;
    uint32_t shift = (4 - size - (address & 3)) * 8;
    uint64_t mask = ((1ULL << (size * 8)) - 1) << shift;
    return regs[reg_id] & mask;
}