#include "RDP.h"

void RDP::RDPCommandRegs::write_size(uint32_t address, uint64_t value, uint8_t size)
{
}

uint64_t RDP::RDPCommandRegs::read_size(uint32_t address, uint8_t size)
{
    return 0;
}

void RDP::RDPSpanRegs::write_size(uint32_t address, uint64_t value, uint8_t size)
{
}

uint64_t RDP::RDPSpanRegs::read_size(uint32_t address, uint8_t size)
{
    return 0;
}
