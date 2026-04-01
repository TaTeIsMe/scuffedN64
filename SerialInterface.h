#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#pragma once
#include"MemoryArea.h"
class SerialInterface: public MemoryArea
{
public:
    SerialInterface();
    ~SerialInterface();

    union{
        struct{
            uint32_t SI_DRAM_ADDR;
            uint32_t SI_PIF_AD_RD64B;
            uint32_t SI_PIF_AD_WR4B;
            uint32_t SI_PIF_AD_WR64B;
            uint32_t SI_PIF_AD_RD4B;
            uint32_t SI_STATUS;
        };
        uint32_t regs[7];
    };

    void write_byte(uint32_t address, uint8_t byte) override;
    uint8_t read_byte(uint32_t address) override;
private:

};

#endif