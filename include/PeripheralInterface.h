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
            uint32_t PI_BSD_DOM1_LAT;
            uint32_t PI_BSD_DOM1_PWD;
            uint32_t PI_BSD_DOM1_PGS;
            uint32_t PI_BSD_DOM1_RLS;
            uint32_t PI_BSD_DOM2_LAT;
            uint32_t PI_BSD_DOM2_PWD;
            uint32_t PI_BSD_DOM2_PGS;
            uint32_t PI_BSD_DOM2_RLS;
        };
        uint32_t regs[13]{};
    };
    void write_size(uint32_t address, uint64_t value, uint8_t size) override;
    uint64_t read_size(uint32_t address, uint8_t size) override;

    class RCP& rcp;

    PeripheralInterface(RCP& rcp);
    
    uint8_t dma_completed = 0;
    uint8_t dma_busy = 0;
    uint8_t dma_error = 0;
    uint8_t io_busy = 0;
    uint8_t dma_direction = 0; //1 when writing to ram
    uint32_t current_ram_addr = 0;
    uint32_t current_cart_addr = 0;
    int64_t len = 0;
    int timer = 0;
    void start_dma();
    void continue_dma();
    void finish_dma();
private:

};