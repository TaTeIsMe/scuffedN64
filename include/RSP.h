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



enum GBI: uint32_t{
G_NOOP = 0x00,
G_VTX = 0x01,
G_MODIFYVTX = 0x02,
G_CULLDL = 0x03,
G_BRANCH_Z = 0x04,
G_TRI1 = 0x05,
G_TRI2 = 0x06,
G_QUAD = 0x07,
G_SPECIAL3 = 0xD3,
G_SPECIAL2 = 0xD4,
G_SPECIAL1 = 0xD5,
G_DMA_IO = 0xD6,
G_TEXTURE = 0xD7,
G_POPMTX = 0xD8,
G_GEOMETRYMODE = 0xD9,
G_MTX = 0xDA,
G_MOVEWORD = 0xDB,
G_MOVEMEM = 0xDC,
G_LOAD_UCODE = 0xDD,
G_DL = 0xDE,
G_ENDDL = 0xDF,
G_SPNOP = 0xE0,
G_RDPHALF_1 = 0xE1,
G_SETOTHERMODE_L = 0xE2,
G_SETOTHERMODE_H = 0xE3,
G_TEXRECT = 0xE4,
G_TEXRECTFLIP = 0xE5,
G_RDPLOADSYNC = 0xE6,
G_RDPPIPESYNC = 0xE7,
G_RDPTILESYNC = 0xE8,
G_RDPFULLSYNC = 0xE9,
G_SETKEYGB = 0xEA,
G_SETKEYR = 0xEB,
G_SETCONVERT = 0xEC,
G_SETSCISSOR = 0xED,
G_SETPRIMDEPTH = 0xEE,
G_RDPSETOTHERMODE = 0xEF,
G_LOADTLUT = 0xF0,
G_RDPHALF_2 = 0xF1,
G_SETTILESIZE = 0xF2,
G_LOADBLOCK = 0xF3,
G_LOADTILE = 0xF4,
G_SETTILE = 0xF5,
G_FILLRECT = 0xF6,
G_SETFILLCOLOR = 0xF7,
G_SETFOGCOLOR = 0xF8,
G_SETBLENDCOLOR = 0xF9,
G_SETPRIMCOLOR = 0xFA,
G_SETENVCOLOR = 0xFB,
G_SETCOMBINE = 0xFC,
G_SETTIMG = 0xFD,
G_SETZIMG = 0xFE,
G_SETCIMG = 0xFF
};

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

    static OSTask parse_from_mem(std::vector<uint8_t> mem, uint32_t addr);
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
            uint32_t regs[8]{};
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
        uint32_t pending_dma[4]{};
    };
    bool pending_dma_direction;
    
    class RCP& rcp;
    class GFX& gfx;
    class Rdram& rdram;
    RSP(RCP& rcp,GFX& gfx, Rdram& rdram);

    uint32_t PC = 0;
    Dmem dmem;
    Imem imem;
    RSPRegs regs;


    int16_t len = 0;
    int16_t start_len = 0;
    uint8_t count = 0;
    uint16_t skip = 0;
    uint32_t current_ram_addr = 0;
    uint16_t current_mem_addr = 0;
    int timer = 0;
    bool dma_direction = false; //1 when writing to ram
    void start_dma();
    void continue_dma();
    void finish_dma();

    RSPTaskType current_task_type;
    bool task_in_progress = false;
    uint32_t task_timer = 0;
    void start_task();
    void continue_task();
    void finish_task();

    void process_gfx_task(OSTask task);
};