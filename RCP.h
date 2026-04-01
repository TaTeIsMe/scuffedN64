#pragma once
#include<cstdint>
#include<vector>

#include"RSP.h"
#include"RDP.h"
#include"MipsInterface.h"
#include"VideoInterface.h"
#include"AudioInterface.h"
#include"PeripheralInterface.h"
#include"RdramInterface.h"
#include"SerialInterface.h"
#include"Rdram.h"
#include"Cartridge.h"
#include"Pif.h"

class RCP
{
public:
    RCP(Rdram& rdram, Cartridge& cartridge, Pif& pif);

    RSP rsp;
    RDP rdp;
    MipsInterface mi;
    VideoInterface vi;
    AudioInterface ai;
    PeripheralInterface pi;
    RdramInterface ri;
    SerialInterface si;
    Rdram& rdram;
    Cartridge& cartridge;
    Pif& pif;
    
    //for all reads i am not sure if unalligned acces is allowed
    uint64_t read_size(uint64_t address, uint8_t size);
    void write_size(uint64_t address, uint64_t value, uint8_t size);
    
    private:
    
    struct MemoryMapping{
        uint32_t start;
        uint32_t end;
        MemoryArea& memory_area;
        uint32_t access_mask;
    };

    MemoryArea unmapped;
    MemoryMapping map[26]={
        {0x00000000, 0x03EFFFFF,rdram           ,~0x0U    }, //RDRAM
        {0x03F00000, 0x03F7FFFF,rdram.regs      ,~0x0U    }, //RDRAM regs 
        {0x03F80000, 0x03FFFFFF,rdram.regs      ,~0x0U    }, //RDRAM regs broadcast
        {0x04000000, 0x04000FFF,rsp.dmem        ,~0x3U    }, //RSP DMEMM
        {0x04001000, 0x04001FFF,rsp.imem        ,~0x3U    }, //RSP IMEM
        {0x04040000, 0x040BFFFF,rsp.regs        ,~0x3U    }, //RSP regs
        {0x040C0000, 0x040FFFFF,unmapped        ,~0x3U    }, //unmapped
        {0x04100000, 0x041FFFFF,rdp.command_regs,~0x3U    }, //RDP command regs
        {0x04200000, 0x042FFFFF,rdp.span_regs   ,~0x3U    }, //RDP span regs
        {0x04300000, 0x043FFFFF,mi              ,~0x3U    }, //MIPS interface
        {0x04400000, 0x044FFFFF,vi              ,~0x3U    }, //Video interface
        {0x04500000, 0x045FFFFF,ai              ,~0x3U    }, //Audio interface
        {0x04600000, 0x046FFFFF,pi              ,~0x3U    }, //Peripheral interface
        {0x04700000, 0x047FFFFF,ri              ,~0x3U    }, //RDRAM interface
        {0x04800000, 0x048FFFFF,si              ,~0x3U    }, //Serial interface
        {0x04900000, 0x04FFFFFF,unmapped        ,~0x0U    }, //unmapped
        {0x05000000, 0x05FFFFFF,unmapped        ,~0x0U    }, //N64DD regs
        {0x06000000, 0x07FFFFFF,unmapped        ,~0x0U    }, //N64DD IPL ROM
        {0x08000000, 0x0FFFFFFF,cartridge       ,~0x0U    }, //Cartridge SRAM/FlashRAM
        {0x10000000, 0x1FBFFFFF,cartridge       ,~0x0U    }, //Cartridge ROM
        {0x1FC00000, 0x1FC007BF,pif             ,~0x0U    }, //PIF ROM
        {0x1FC007C0, 0x1FC007FF,pif             ,~0x0U    }, //PIF RAM
        {0x1FC00800, 0x1FCFFFFF,unmapped        ,~0x0U    }, //Mystery 0_0
        {0x1FD00000, 0x1FFFFFFF,unmapped        ,~0x0U    }, //unused
        {0x20000000, 0x7FFFFFFF,unmapped        ,~0x0U    }, //unused
        {0x80000000, 0xFFFFFFFF,unmapped        ,~0x0U    }, //unmapped (mapped in the cpu)
    };
};
