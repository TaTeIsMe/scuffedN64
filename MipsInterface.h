#ifndef MIPSINTERFACE_H
#define MIPSINTERFACE_H

#pragma once
#include"MemoryArea.h"
class MipsInterface: public MemoryArea
{
public:
    MipsInterface();
    ~MipsInterface();

    union{
        struct{
            uint32_t MI_MODE;
            uint32_t MI_VERSION;
            uint32_t MI_INTERRUPT;
            uint32_t MI_MASK;
        };
        uint32_t regs[4];
    };

    void write_byte(uint32_t address, uint8_t byte) override;
    uint8_t read_byte(uint32_t address) override;
private:

};

#endif