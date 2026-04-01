#include "RdramInterface.h"

RdramInterface::RdramInterface()
{

}

RdramInterface::~RdramInterface()
{

}

void RdramInterface::write_byte(uint32_t address, uint8_t byte)
{
    //thsi doesn't handle any special behaviour todo
    address = address & 0x1F; //mirroring
    uint32_t reg_id = address >> 2;
    uint32_t prev_val = regs[reg_id];
    uint8_t shift = (24 - (address & 3) * 8);
    regs[ reg_id ] = (prev_val & ~(0xFF << shift)) | (byte << shift);
}

uint8_t RdramInterface::read_byte(uint32_t address)
{
    //thsi doesn't handle any special behaviour todo
    address = address & 0x1F; //mirroring
    uint32_t reg_id = address >> 2;
    uint8_t shift = (24 - (address & 3) * 8);
    return regs[reg_id] >> shift;
}
