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
        MemoryArea& memory_area;
    };

    MemoryArea unmapped;
    MemoryMapping map[26]={
        {0x00000000, 0x03EFFFFF,rdram}, //RDRAM
        {0x03F00000, 0x03F7FFFF,rdram.regs}, //RDRAM regs 
        {0x03F80000, 0x03FFFFFF,rdram.regs}, //RDRAM regs broadcast
        {0x04000000, 0x04000FFF,rsp.dmem}, //RSP DMEMM
        {0x04001000, 0x04001FFF,rsp.imem}, //RSP IMEM
        {0x04040000, 0x040BFFFF,rsp.regs}, //RSP regs
        {0x040C0000, 0x040FFFFF,unmapped}, //unmapped
        {0x04100000, 0x041FFFFF,rdp.command_regs}, //RDP command regs
        {0x04200000, 0x042FFFFF,rdp.span_regs}, //RDP span regs
        {0x04300000, 0x043FFFFF,mi}, //MIPS interface
        {0x04400000, 0x044FFFFF,vi}, //Video interface
        {0x04500000, 0x045FFFFF,ai}, //Audio interface
        {0x04600000, 0x046FFFFF,pi}, //Peripheral interface
        {0x04700000, 0x047FFFFF,ri}, //RDRAM interface
        {0x04800000, 0x048FFFFF,si}, //Serial interface
        {0x04900000, 0x04FFFFFF,unmapped}, //unmapped
        {0x05000000, 0x05FFFFFF,unmapped}, //N64DD regs
        {0x06000000, 0x07FFFFFF,unmapped}, //N64DD IPL ROM
        {0x08000000, 0x0FFFFFFF,cartridge}, //Cartridge SRAM/FlashRAM
        {0x10000000, 0x1FBFFFFF,cartridge}, //Cartridge ROM
        {0x1FC00000, 0x1FC007BF,pif}, //PIF ROM
        {0x1FC007C0, 0x1FC007FF,pif}, //PIF RAM
        {0x1FC00800, 0x1FCFFFFF,unmapped}, //Mystery 0_0
        {0x1FD00000, 0x1FFFFFFF,unmapped}, //unused
        {0x20000000, 0x7FFFFFFF,unmapped}, //unused
        {0x80000000, 0xFFFFFFFF,unmapped}, //unmapped (mapped in the cpu)
    };
};
