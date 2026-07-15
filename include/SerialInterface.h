#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#pragma once
#include"MemoryArea.h"
class SerialInterface: public MemoryArea
{
public:

    union{
        struct{
            uint32_t SI_DRAM_ADDR;
            uint32_t SI_PIF_AD_RD64B;
            uint32_t SI_PIF_AD_WR4B;
            uint32_t SI_PIF_AD_WR64B;
            uint32_t SI_PIF_AD_RD4B;
            uint32_t SI_STATUS;
        };
        uint32_t regs[7]{};
    };

    class RCP& rcp;

    SerialInterface(RCP& rcp);

    
    void write_size(uint32_t address, uint64_t value, uint8_t size) override;
    uint64_t read_size(uint32_t address, uint8_t size) override;

    int temp_i = 0; //replace later with real dma process
    uint8_t dma_direction = 0; //0 - from PIF to RAM 1 - from RAM to PIF
    void start_dma();
    void continue_dma();
    void finish_dma();
private:

};

#endif