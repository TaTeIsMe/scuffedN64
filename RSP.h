#pragma once

#include"MemoryArea.h"
#include<vector>

#define TASK_LENGTH 1000

#define G_MW_MATRIX 0x00
#define G_MW_NUMLIGHT 0x02
#define G_MW_CLIP 0x04
#define G_MW_SEGMENT 0x06
#define G_MW_FOG 0x08
#define G_MW_LIGHTCOL 0x0A
#define G_MW_FORCEMTX 0x0C
#define G_MW_PERSPNORM 0x0E

enum class RSPTaskType : uint32_t{
    NULTASK = 0,
    GFXTASK = 1,
    AUDTASK = 2,
    VIDTASK = 3,
    NJPEGTASK = 4,
    HVQTASK = 6,
    HVQMTASK = 7
};

struct OSTask{
    uint32_t type;
    uint32_t flags;

    uint32_t ucode_boot; //pointer
    uint32_t ucode_boot_size;

    uint32_t ucode; //pointer
    uint32_t ucode_size;

    uint32_t ucode_data; //pointer
    uint32_t ucode_data_size;

    uint32_t dram_stack; //pointer
    uint32_t dram_stack_size;

    uint32_t output_buff; //pointer
    uint32_t output_buff_size; //pointer

    uint32_t data_ptr; //pointer
    uint32_t data_size;

    uint32_t yield_data_ptr; //pointer
    uint32_t yield_data_size;
};

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

    RSPTaskType current_task_type;
    bool task_in_progress;
    uint32_t task_timer;
    void start_task();
    void continue_task();
    void finish_task();

    void process_gfx_task(OSTask task);
};