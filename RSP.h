#ifndef RSP_H
#define RSP_H

#pragma once

#include"MemoryArea.h"

class RSP
{
public:
    RSP();
    ~RSP();
    struct Dmem : public MemoryArea{
        uint8_t dmem[0x1000];
        void write_byte(uint32_t address, uint8_t byte) override;
        uint8_t read_byte(uint32_t address) override;
    };
    struct Imem : public MemoryArea{
        uint8_t imem[0x1000];
        void write_byte(uint32_t address, uint8_t byte) override;
        uint8_t read_byte(uint32_t address) override;
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
        void write_byte(uint32_t address, uint8_t byte) override;
        uint8_t read_byte(uint32_t address) override;
    };
    uint32_t PC;
    Dmem dmem;
    Imem imem;
    RSPRegs regs;
};

#endif