#ifndef RDP_H
#define RDP_H

#pragma once
#include"MemoryArea.h"
class RDP
{
public:
    RDP();
    ~RDP();

    struct RDPCommandRegs : public MemoryArea {
        union{
            struct{
                uint32_t DPC_START;
                uint32_t DPC_END;
                uint32_t DPC_CURRENT;
                uint32_t DPC_STATUS;
                uint32_t DPC_CLOCK;
                uint32_t DPC_BUF_BUSY;
                uint32_t DPC_PIPE_BUSY;
                uint32_t DPC_TME_BUSY;
            };
            uint32_t regs[8];
        };
        void write_byte(uint32_t address, uint8_t byte) override;
        uint8_t read_byte(uint32_t address) override;
    };

    struct RDPSpanRegs : public MemoryArea {
        union{
            struct{
                uint32_t DPS_TBIST;
                uint32_t DPS_TEST_MODE;
                uint32_t DPS_BUFTEST_ADDR;
                uint32_t DPS_BUFTEST_DATA;
            };
            uint32_t rdp_span_regs[4];
        };
        void write_byte(uint32_t address, uint8_t byte) override;
        uint8_t read_byte(uint32_t address) override;
    };

    RDPCommandRegs command_regs;
    RDPSpanRegs span_regs;

private:

};

#endif