#include "Rdram.h"

Rdram::Rdram():mem(4*1025*1024){}

void Rdram::write_byte(uint32_t address, uint8_t byte)
{
        if(address < mem.size())
        mem[address] = byte;
}

uint8_t Rdram::read_byte(uint32_t address)
{
    if(address < mem.size())
        return mem[address]; 
    else
        return 0;
}

//these two don't take into account broadcast aaddresses
void Rdram::RdramRegs::write_byte(uint32_t address, uint8_t byte)
{
    address = address & 0x3F; //mirroring
    uint32_t reg_id = address >> 2;
    uint32_t prev_val = regs[reg_id];
    uint8_t shift = (24 - (address & 3) * 8);
    regs[ reg_id ] = (prev_val & ~(0xFF << shift)) | (byte << shift);
}

uint8_t Rdram::RdramRegs::read_byte(uint32_t address)
{
    address = address & 0x3F; //mirroring
    uint32_t reg_id = address >> 2;
    uint8_t shift = (24 - (address & 3) * 8);
    return regs[reg_id] >> shift;
}
