#ifndef AUDIOINTERFACE_H
#define AUDIOINTERFACE_H

#pragma once
#include"MemoryArea.h"
class AudioInterface : public MemoryArea
{
public:
    AudioInterface();
    ~AudioInterface();

    union{
        struct{
            uint32_t AI_DRAM_ADDR;
            uint32_t AI_LENGTH;
            uint32_t AI_CONTROL;
            uint32_t AI_STATUS;
            uint32_t AI_DACRATE;
            uint32_t AI_BITRATE;
        };
        uint32_t regs[6];
    };

    void write_byte(uint32_t address, uint8_t byte) override;
    uint8_t read_byte(uint32_t address) override;
private:

};

#endif