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
    
    //IPL2
    vr4300.PC = 0xffffffffa4000040;
    for (int i = 0; i < 0xFC0; i++)
    {
        rcp.rsp.dmem.mem[i] = rom[i];
    }
    
    //ram init skip
    rcp.ri.RI_SELECT = 0x14;
    
    while(true){
        vr4300.on_pclock();
        vr4300.on_pclock();
        if(rcp.rsp.regs.SP_DMA_BUSY)rcp.rsp.continue_dma();
    }

}