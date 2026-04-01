#ifndef RDRAM_H
#define RDRAM_H

#pragma once

#include"MemoryArea.h"
#include<vector>
class Rdram : public MemoryArea
{
public:
    Rdram();
    
    struct RdramRegs : public MemoryArea {
        union {
            struct {
                uint32_t DeviceType;
                uint32_t DeviceId;
                uint32_t Delay;
                uint32_t Mode;
                uint32_t RefInterval;
                uint32_t RefRow;
                uint32_t RasInterval;
                uint32_t MinInterval;
                uint32_t AddressSelect;
                uint32_t DeviceManufacturer;
                uint32_t Row;
            };
            uint32_t regs[11];
        };
        void write_byte(uint32_t address, uint8_t byte) override;
        uint8_t read_byte(uint32_t address) override;
    };
    
    RdramRegs regs;
    std::vector<uint8_t> mem;

    void write_byte(uint32_t address, uint8_t byte) override;
    uint8_t read_byte(uint32_t address) override;

private:

};

#endif