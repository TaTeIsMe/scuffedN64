#include "AudioInterface.h"
#include "RCP.h"

AudioInterface::AudioInterface(RCP &rcp):rcp(rcp){
    AI_STATUS = 0x07190000; //some temporary value
}

void AudioInterface::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    address &= 0x1F; //mirroring
    uint32_t reg_id = address >> 2;
    switch (address)
    {
    case 0x00:
        AI_DRAM_ADDR = value & 0x00FFFFF8;
        break;
    case 0x04:
        AI_LENGTH = value & 0x0007FFF8;
        break;
    case 0x08:
        AI_CONTROL = value & 0x00000001;
        AI_STATUS &= ~ (1<<25);
        AI_STATUS |= AI_CONTROL << 25;
        break;
    case 0x0C:
        rcp.mi.clear_interrupt(InterruptSource::AI);
        break;
    case 0x10:
        AI_DACRATE = value & 0x00007FFF;
        break;
    case 0x14:
        AI_BITRATE = value & 0x0000000F;
        break;
    
    default:
        break;
    }
}

uint64_t AudioInterface::read_size(uint32_t address, uint8_t size)
{
    address &= 0x1F; //mirroring
    uint32_t reg_id = address >> 2;

    switch (address)
    {
    case 0x00:
        return AI_LENGTH; //yes, ai length 
        break;
    case 0x04:
        return AI_LENGTH;
        break;
    case 0x08:
        return AI_LENGTH; //yes, ai length 
        break;
    case 0x0C:
        return AI_STATUS;
        break;
    case 0x10:
        return AI_LENGTH; //yes, ai length 
        break;
    case 0x14:
        return AI_LENGTH; //yes, ai length 
        break;
    
    default:
        break;
    }

    return 0;
}
