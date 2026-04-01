#include "RdramInterface.h"


void RdramInterface::write_size(uint32_t address, uint64_t value, uint8_t size)
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

uint64_t RdramInterface::read_size(uint32_t address, uint8_t size)
{
    //thsi doesn't handle any special behaviour todo
    //this assumes the access doesn't go accross two regs
    address = address & 0x1F; //mirroring
    uint32_t reg_id = address >> 2;
    uint32_t shift = (4 - size - (address & 3)) * 8;
    uint64_t mask = ((1ULL << (size * 8)) - 1) << shift;
    return regs[reg_id] & mask;
}
