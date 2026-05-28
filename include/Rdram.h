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
        void write_size(uint32_t address, uint64_t value, uint8_t size) override;
        uint64_t read_size(uint32_t address, uint8_t size) override;
    };
    
    RdramRegs regs;
    std::vector<uint8_t> mem;

    void write_size(uint32_t address, uint64_t value, uint8_t size) override;
    uint64_t read_size(uint32_t address, uint8_t size) override;

private:

};

#endif