#pragma once

#include"MemoryArea.h"
#include<vector>

class RSP
{
public:

    struct Dmem : public MemoryArea{
        Dmem();
        std::vector<uint8_t> mem;
        void write_size(uint32_t address, uint64_t value, uint8_t size) override;
        uint64_t read_size(uint32_t address, uint8_t size) override;
    };
    struct Imem : public MemoryArea{
        Imem();
        std::vector<uint8_t> mem;
        void write_size(uint32_t address, uint64_t value, uint8_t size) override;
        uint64_t read_size(uint32_t address, uint8_t size) override;
    };
    struct RSPRegs : public MemoryArea{
        RSPRegs(RSP& rsp);
        RSP& rsp;
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
        void write_size(uint32_t address, uint64_t value, uint8_t size) override;
        uint64_t read_size(uint32_t address, uint8_t size) override;
    };
    union{
        struct{
            uint32_t pending_dma_spaddr;
            uint32_t pending_dma_ramddr;
            uint32_t pending_dma_rdlen;
            uint32_t pending_dma_wrlen;
        };
        uint32_t pending_dma[4];
    };
    bool pending_dma_direction;
    uint32_t status_input;
    
    class RCP& rcp;
    class Rdram& rdram;
    RSP(RCP& rcp, Rdram& rdram);

    uint32_t PC;
    Dmem dmem;
    Imem imem;
    RSPRegs regs;


    int16_t len;
    int16_t start_len;
    uint8_t count;
    uint16_t skip;
    uint32_t current_ram_addr;
    uint16_t current_mem_addr;
    bool dma_direction; //1 when writing to ram
    void start_dma();
    void continue_dma();
    void finish_dma();

};