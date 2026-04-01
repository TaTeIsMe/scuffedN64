#ifndef MIPSINTERFACE_H
#define MIPSINTERFACE_H

#pragma once
#include"MemoryArea.h"
class MipsInterface: public MemoryArea
{
public:

    union{
        struct{
            uint32_t MI_MODE;
            uint32_t MI_VERSION;
            uint32_t MI_INTERRUPT;
            uint32_t MI_MASK;
        };
        uint32_t regs[4];
    };

    void write_size(uint32_t address, uint64_t value, uint8_t size) override;
    uint64_t read_size(uint32_t address, uint8_t size) override;
private:

};

#endif