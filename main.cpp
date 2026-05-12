#include <iostream>
#include <fstream>
#include <cstdint>
#include "VR4300.h"
#include "RCP.h"
#include "Cartridge.h"
#include "Rdram.h"
#include "Pif.h"
#include <vector>

//todo:
    //interrupts
    //dmas
    //fpu
    //make rcp address range only output and accept 32 but values
int main(){
    
    std::ifstream rom_file("n64-systemtest.z64", std::ios::binary);
    std::vector<uint8_t> rom(
        (std::istreambuf_iterator<char>(rom_file)),
        std::istreambuf_iterator<char>()
    );
    rom_file.close();
    Pif pif;
    Rdram rdram;
    Cartridge cartridge(rom);
    RCP rcp(rdram, cartridge, pif);
    VR4300 vr4300(rcp);
    
    //IPL2 skip
    vr4300.PC = 0xffffffffa4000040;
    vr4300.cp0.config = 0x7006e463; 
    vr4300.cp0.status = 0x34000000;

    vr4300.GPR[19] = 0;
    vr4300.GPR[20] = 0;
    vr4300.GPR[21] = 0;
    vr4300.GPR[22] = 0x91;
    vr4300.GPR[23] = 0;
    

    for (int i = 0; i < 0xFC0; i++)
    {
        rcp.rsp.dmem.mem[i] = rom[i];
    }

    rcp.rsp.imem.mem[0x0000/4] = 0x3c0dbfc0;
    rcp.rsp.imem.mem[0x0004/4] = 0x8da807fc;
    rcp.rsp.imem.mem[0x0008/4] = 0x25ad07c0;
    rcp.rsp.imem.mem[0x000c/4] = 0x31080080;
    rcp.rsp.imem.mem[0x0010/4] = 0x5500fffc;
    rcp.rsp.imem.mem[0x0014/4] = 0x3c0dbfc0;
    rcp.rsp.imem.mem[0x0018/4] = 0x8da80024;
    rcp.rsp.imem.mem[0x001c/4] = 0x3c0bb000;

    vr4300.GPR[11] = 0xffffffffa4000040ULL;
    vr4300.GPR[29] = 0xffffffffa4001ff0ULL;
    vr4300.GPR[31] = 0xffffffffa4001550ULL;
    
    //ram init skip
    rcp.ri.RI_SELECT = 0x14;
    
    while(true){
        vr4300.on_pclock();
        if(rcp.rsp.regs.SP_DMA_BUSY)rcp.rsp.continue_dma();
        if(rcp.rsp.regs.SP_DMA_BUSY)rcp.rsp.continue_dma();
        if(rcp.rsp.regs.SP_DMA_BUSY)rcp.rsp.continue_dma();
        if(rcp.pi.dma_busy)rcp.pi.continue_dma();
        if(rcp.pi.dma_busy)rcp.pi.continue_dma();
        if(rcp.pi.dma_busy)rcp.pi.continue_dma();
    }

}