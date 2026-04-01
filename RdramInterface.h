#ifndef RDRAMINTERFACE_H
#define RDRAMINTERFACE_H

#pragma once
#include"MemoryArea.h"
class RdramInterface : public MemoryArea
{
public:
    RdramInterface();
    ~RdramInterface();

    union{
        struct{
            uint32_t RI_MODE;
            uint32_t RI_CONFIG;
            uint32_t RI_CURRENT_LOAD;
            uint32_t RI_SELECT;
            uint32_t RI_LATENCY;
            uint32_t RI_ERROr;
            uint32_t RI_BANK_STATUS;
        };
        uint32_t regs[7];
    };

    void write_byte(uint32_t address, uint8_t byte) override;
    uint8_t read_byte(uint32_t address) override;
private:

};

#endif