#include "SerialInterface.h"
#include "RCP.h"
#include "MipsInterface.h"

SerialInterface::SerialInterface(RCP &rcp):rcp(rcp){}

void SerialInterface::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    address &= 0x3F; //mirroring
    uint8_t reg_id = address>>2;
    switch (address)
    {
    case 0x00:
        SI_DRAM_ADDR = value & 0x00FFFFFF;
        break;
    case 0x04:
        if(SI_STATUS & 1)return;
        SI_PIF_AD_RD64B = value;
        dma_direction = 0;
        start_dma();
        break;
    case 0x10:
    if(SI_STATUS & 1)return;
        dma_direction = 1;
        start_dma();
        break;
    case 0x14:
        break;
    case 0x18:
        rcp.mi.clear_interrupt(InterruptSource::SI);
        SI_STATUS &= ~0x1000;
        break;

    default:
        break;
    }
}

uint64_t SerialInterface::read_size(uint32_t address, uint8_t size)
{
    if(address == 0x18)return SI_STATUS;
    return 0;
}

void SerialInterface::start_dma()
{
    SI_STATUS |= 1;
    //completely random, change later
    //temp_i = 64 * 3;

    int latency = dma_direction?64*3:1500;

    rcp.eventq.enqueue(rcp.vr4300.cycles + latency, EventType::SI_DMA_DONE);
}

void SerialInterface::continue_dma()
{
    //temp_i-=16;
    //if(temp_i < 0){
    //    finish_dma();
    //}
}

void SerialInterface::finish_dma()
{

    for (int i = 0; i < 64; i++)
        {
            if(dma_direction){
                rcp.pif.ram[i] = rcp.rdram.mem[SI_DRAM_ADDR + i];
            }
            else rcp.rdram.mem[SI_DRAM_ADDR + i] = rcp.pif.ram[i];
        }
        if(rcp.pif.ram[63] && dma_direction)rcp.pif.handle_command();

    SI_STATUS &= ~1;
    SI_STATUS |= 0x1000;
    rcp.mi.route_interrupt(InterruptSource::SI);
}
