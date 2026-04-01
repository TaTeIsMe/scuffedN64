#include "RSP.h"

RSP::RSP()
{

}

RSP::~RSP()
{

}

void RSP::Dmem::write_byte(uint32_t address, uint8_t byte)
{
    dmem[address] = byte;
}

uint8_t RSP::Dmem::read_byte(uint32_t address)
{
    return dmem[address];
}

void RSP::Imem::write_byte(uint32_t address, uint8_t byte)
{
    imem[address] = byte;
}

uint8_t RSP::Imem::read_byte(uint32_t address)
{
    return imem[address];
}

void RSP::RSPRegs::write_byte(uint32_t address, uint8_t byte)
{
    address = address & 0x1F;//mirroring
    
}

uint8_t RSP::RSPRegs::read_byte(uint32_t address)
{
    return 0;
}
