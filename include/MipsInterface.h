#ifndef MIPSINTERFACE_H
#define MIPSINTERFACE_H

#pragma once
#include"MemoryArea.h"

enum class InterruptSource: uint8_t{
    SP = 1 << 0,
    SI = 1 << 1,
    AI = 1 << 2,
    VI = 1 << 3,
    PI = 1 << 4,
    DP = 1 << 5
};

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
        uint32_t regs[4]{};
    };

    class RCP& rcp;

    MipsInterface(RCP& rcp);

    void route_interrupt(InterruptSource src);
    void clear_interrupt(InterruptSource src);

    void write_size(uint32_t address, uint64_t value, uint8_t size) override;
    uint64_t read_size(uint32_t address, uint8_t size) override;
private:

};

#endif