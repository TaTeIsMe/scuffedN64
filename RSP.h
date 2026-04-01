#ifndef RSP_H
#define RSP_H

#pragma once

#include"MemoryArea.h"
#include<vector>
class RSP
{
public:

    struct Dmem : public MemoryArea{
        Dmem();
        std::vector<uint8_t> mem;
        void write_size(uint32_t address, uint64_t value, uint8_t size) override;
        uint64_t read_size(uint32_t address, uint8_t size) override;
    };
    struct Imem : public MemoryArea{
        Imem();
        std::vector<uint8_t> mem;
        void write_size(uint32_t address, uint64_t value, uint8_t size) override;
        uint64_t read_size(uint32_t address, uint8_t size) override;
    };
    struct RSPRegs : public MemoryArea{
        union{
            struct{
                uint32_t SP_DMA_SPADDR;
                uint32_t SP_DMA_RAMADDR;
                uint32_t SP_DMA_RDLEN;
                uint32_t SP_DMA_WRLEN;
                uint32_t SP_STATUS;
                uint32_t SP_DMA_FULL;
                uint32_t SP_DMA_BUSY;
                uint32_t SP_SEMAPHORE;
            };
            uint32_t regs[8];
        };
        void write_size(uint32_t address, uint64_t value, uint8_t size) override;
        uint64_t read_size(uint32_t address, uint8_t size) override;
    };
    uint32_t PC;
    Dmem dmem;
    Imem imem;
    RSPRegs regs;
};

#endif