#include "AudioInterface.h"

AudioInterface::AudioInterface(RCP &rcp):rcp(rcp){}

void AudioInterface::write_size(uint32_t address, uint64_t value, uint8_t size)
{
}

uint64_t AudioInterface::read_size(uint32_t address, uint8_t size)
{
    return 0;
}
