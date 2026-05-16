#include "SerialInterface.h"
#include "RCP.h"
#include "MipsInterface.h"

SerialInterface::SerialInterface(RCP &rcp):rcp(rcp){}

void SerialInterface::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    uint8_t reg_id = address>>2;
    if(address == 0x18)rcp.mi.clear_interrupt(InterruptSource::PI);
}

uint64_t SerialInterface::read_size(uint32_t address, uint8_t size)
{
    return 0;
}
