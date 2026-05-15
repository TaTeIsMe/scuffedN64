#ifndef RDRAMINTERFACE_H
#define RDRAMINTERFACE_H

#pragma once
#include"MemoryArea.h"
class RdramInterface : public MemoryArea
{
public:

    union{
        struct{
            uint32_t RI_MODE;
            uint32_t RI_CONFIG;
            uint32_t RI_CURRENT_LOAD;
            uint32_t RI_SELECT;
            uint32_t RI_REFRESH;
            uint32_t RI_LATENCY;
            uint32_t RI_ERROR;
            uint32_t RI_BANK_STATUS;
        };
        uint32_t regs[8];
    };
    
    class RCP& rcp;

    RdramInterface(RCP& rcp);

    void write_size(uint32_t address, uint64_t value, uint8_t size) override;
    uint64_t read_size(uint32_t address, uint8_t size) override;
private:

};

#endif