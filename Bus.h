#pragma once
#include<cstdint>
#include<vector>

//WIll be renamed later to rcp
class Bus
{
public:
    Bus(uint8_t* rdram, std::vector<uint8_t>& rom);
    ~Bus();


    //RDRAM move to outside later
    uint8_t* rdram;

    union{
        struct{
            uint32_t DeviceType;
            uint32_t DeviceId;
            uint32_t Delay;
            uint32_t Mode;
            uint32_t RefInterval;
            uint32_t RefRow;
            uint32_t RasInterval;
            uint32_t MinInterval;
            uint32_t AddressSelect;
            uint32_t DeviceManufacturer;
            uint32_t Row;
        };
        uint32_t rdram_regs[11];
    };
    void rdram_write_byte(uint32_t address, uint8_t byte);
    uint8_t rdram_read_byte(uint32_t address);
    void rdram_regs_write_byte(uint32_t address, uint8_t byte);
    uint8_t rdram_regs_read_byte(uint32_t address);
    void rdram_broadcast_write_byte(uint32_t address, uint8_t byte);
    uint8_t rdram_broadcast_read_byte(uint32_t address);

    //RSP
    uint8_t rsp_dmem[0x1000];
    uint8_t rsp_imem[0x1000];
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
        uint32_t rsp_regs[8];
    };
    uint32_t SP_PC;
    void rsp_dmem_write_byte(uint32_t address, uint8_t byte);
    uint8_t rsp_dmem_read_byte(uint32_t address);
    void rsp_imem_write_byte(uint32_t address, uint8_t byte);
    uint8_t rsp_imem_read_byte(uint32_t address);
    void rsp_regs_write_byte(uint32_t address, uint8_t byte);
    uint8_t rsp_regs_read_byte(uint32_t address);
    
    //RDP regs
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
        uint32_t rdp_regs[8];
    };
    void rdp_command_write_byte(uint32_t address, uint8_t byte);
    uint8_t rdp_command_read_byte(uint32_t address);

    //RDP span regs
    union{
        struct{
            uint32_t DPS_TBIST;
            uint32_t DPS_TEST_MODE;
            uint32_t DPS_BUFTEST_ADDR;
            uint32_t DPS_BUFTEST_DATA;
        };
        uint32_t rdp_span_regs[4];
    };
    void rdp_span_write_byte(uint32_t address, uint8_t byte);
    uint8_t rdp_span_read_byte(uint32_t address);

    //MI
    union{
        struct{
            uint32_t MI_MODE;
            uint32_t MI_VERSION;
            uint32_t MI_INTERRUPT;
            uint32_t MI_MASK;
        };
        uint32_t mi_regs[4];
    };
    void mi_write_byte(uint32_t address, uint8_t byte);
    uint8_t mi_read_byte(uint32_t address);

    //VI
    union{
        struct{
            uint32_t VI_CTRL;
            uint32_t VI_ORIGIN;
            uint32_t VI_WIDTH;
            uint32_t VI_V_INTR;
            uint32_t VI_V_CURRENT;
            uint32_t VI_BURST;
            uint32_t VI_V_TOTAL;
            uint32_t VI_H_TOTAL;
            uint32_t VI_H_TOTAL_LEAP;
            uint32_t VI_H_VIDEO;
            uint32_t VI_V_VIDEO;
            uint32_t VI_V_BURST;
            uint32_t VI_X_SCALE;
            uint32_t VI_Y_SCALE;
            uint32_t VI_TEST_ADDR;
            uint32_t VI_STAGED_DATA;
        };
        uint32_t vi_regs[16];
    };
    void vi_write_byte(uint32_t address, uint8_t byte);
    uint8_t vi_read_byte(uint32_t address);

    //AI
    union{
        struct{
            uint32_t AI_DRAM_ADDR;
            uint32_t AI_LENGTH;
            uint32_t AI_CONTROL;
            uint32_t AI_STATUS;
            uint32_t AI_DACRATE;
            uint32_t AI_BITRATE;
        };
        uint32_t ai_regs[6];
    };
    void ai_write_byte(uint32_t address, uint8_t byte);
    uint8_t ai_read_byte(uint32_t address);

    //PI
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
        uint32_t pi_regs[9];
    };
    void pi_write_byte(uint32_t address, uint8_t byte);
    uint8_t pi_read_byte(uint32_t address);

    //RI
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
        uint32_t ri_regs[7];
    };
    void ri_write_byte(uint32_t address, uint8_t byte);
    uint8_t ri_read_byte(uint32_t address);

    //SI
    union{
        struct{
            uint32_t SI_DRAM_ADDR;
            uint32_t SI_PIF_AD_RD64B;
            uint32_t SI_PIF_AD_WR4B;
            uint32_t SI_PIF_AD_WR64B;
            uint32_t SI_PIF_AD_RD4B;
            uint32_t SI_STATUS;
        };
        uint32_t si_regs[7];
    };
    void si_write_byte(uint32_t address, uint8_t byte);
    uint8_t si_read_byte(uint32_t address);

    std::vector<uint8_t>& rom; //replace with the interface later?
    void rom_write_byte(uint32_t address, uint8_t byte);
    uint8_t rom_read_byte(uint32_t address);

    uint8_t unmapped_read(uint32_t addr);
    void unmapped_write(uint32_t addr,uint8_t val);
    
    uint32_t read_word(uint64_t address);
    void write_word(uint64_t address, uint32_t word);
    uint64_t read_doubleword(uint64_t address);
    void write_doubleword(uint64_t address, uint64_t doubleword);
    uint8_t read_byte(uint64_t address);
    void write_byte(uint64_t address, uint8_t byte);
    uint16_t read_halfword(uint64_t address);
    void write_halfword(uint64_t address, uint16_t halfword);
    uint64_t read_size(uint64_t address, uint8_t size);
    void write_size(uint64_t address, uint64_t value, uint8_t size);
    
    private:
    
    struct MemoryMapping{
        uint32_t start;
        uint32_t end;
        void (Bus::*device_write_byte)(uint32_t addr, uint8_t byte);
        uint8_t (Bus::*device_read_byte)(uint32_t addr); 
    };
    
    MemoryMapping map[26]={
        {0x00000000, 0x03EFFFFF, &Bus::rdram_write_byte, &Bus::rdram_read_byte}, //RDRAM
        {0x03F00000, 0x03F7FFFF, &Bus::rdram_regs_write_byte, &Bus::rdram_regs_read_byte}, //RDRAM regs 
        {0x03F80000, 0x03FFFFFF, &Bus::rdram_broadcast_write_byte, &Bus::rdram_broadcast_read_byte}, //RDRAM regs broadcast
        {0x04000000, 0x04000FFF, &Bus::rsp_dmem_write_byte, &Bus::rsp_dmem_read_byte}, //RSP DMEMM
        {0x04001000, 0x04001FFF, &Bus::rsp_imem_write_byte, &Bus::rsp_imem_read_byte}, //RSP IMEM
        {0x04040000, 0x040BFFFF, &Bus::rsp_regs_write_byte, &Bus::rsp_regs_read_byte}, //RSP regs
        {0x040C0000, 0x040FFFFF, &Bus::unmapped_write, &Bus::unmapped_read}, //unmapped
        {0x04100000, 0x041FFFFF, &Bus::rdp_command_write_byte, &Bus::rdp_command_read_byte}, //RDP command regs
        {0x04200000, 0x042FFFFF, &Bus::rdp_span_write_byte, &Bus::rdp_span_read_byte}, //RDP span regs
        {0x04300000, 0x043FFFFF, &Bus::mi_write_byte, &Bus::mi_read_byte}, //MIPS interface
        {0x04400000, 0x044FFFFF, &Bus::vi_write_byte, &Bus::vi_read_byte}, //Video interface
        {0x04500000, 0x045FFFFF, &Bus::ai_write_byte, &Bus::ai_read_byte}, //Audio interface
        {0x04600000, 0x046FFFFF, &Bus::pi_write_byte, &Bus::pi_read_byte}, //Peripheral interface
        {0x04700000, 0x047FFFFF, &Bus::ri_write_byte, &Bus::ri_read_byte}, //RDRAM interface
        {0x04800000, 0x048FFFFF, &Bus::si_write_byte, &Bus::si_read_byte}, //Serial interface
        {0x04900000, 0x04FFFFFF, &Bus::unmapped_write, &Bus::unmapped_read}, //unmapped
        {0x05000000, 0x05FFFFFF, &Bus::unmapped_write, &Bus::unmapped_read}, //N64DD regs
        {0x06000000, 0x07FFFFFF, &Bus::unmapped_write, &Bus::unmapped_read}, //N64DD IPL ROM
        {0x08000000, 0x0FFFFFFF, &Bus::unmapped_write, &Bus::unmapped_read}, //Cartridge SRAM/FlashRAM
        {0x10000000, 0x1FBFFFFF, &Bus::rom_write_byte, &Bus::rom_read_byte}, //Cartridge ROM
        {0x1FC00000, 0x1FC007BF, &Bus::unmapped_write, &Bus::unmapped_read}, //PIF ROM
        {0x1FC007C0, 0x1FC007FF, &Bus::unmapped_write, &Bus::unmapped_read}, //PIF RAM
        {0x1FC00800, 0x1FCFFFFF, &Bus::unmapped_write, &Bus::unmapped_read}, //Mystery 0_0
        {0x1FD00000, 0x1FFFFFFF, &Bus::unmapped_write, &Bus::unmapped_read}, //unused
        {0x20000000, 0x7FFFFFFF, &Bus::unmapped_write, &Bus::unmapped_read}, //unused
        {0x80000000, 0xFFFFFFFF, &Bus::unmapped_write, &Bus::unmapped_read}, //unmapped (mapped in the cpu)
    };
};
