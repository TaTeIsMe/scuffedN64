#ifndef PERIPHERALINTERFACE_H
#define PERIPHERALINTERFACE_H

#pragma once
#include"MemoryArea.h"
class PeripheralInterface : public MemoryArea
{
public:

    union{
        struct{
            uint32_t PI_DRAM_ADDR;
            uint32_t PI_CART_ADDR;
            uint32_t PI_RD_LEN;
            uint32_t PI_WR_LEN;
            uint32_t PI_STATUS;
            uint32_t PI_BSD_DOMn_LAT;
            uint32_t PI_BSD_DOMn_PWD;
            uint32_t PI_BSD_DOMn_PGS;
            uint32_t PI_BSD_DOMn_RLS;
        };
        uint32_t regs[9];
    };
    void write_size(uint32_t address, uint64_t value, uint8_t size) override;
    uint64_t read_size(uint32_t address, uint8_t size) override;
    
private:

};

#endif