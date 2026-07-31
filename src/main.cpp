#include <iostream>
#include <fstream>
#include <cstdint>
#include "VR4300Interpreter.h"
#include "RCP.h"
#include "Cartridge.h"
#include "Rdram.h"
#include "Pif.h"
#include <vector>
#include"GFX.h"
#include <chrono>
#include <iostream>
#include"EventQ.h"

int main(){
    
    std::ifstream rom_file("./ROMS/ZELOOTD.z64", std::ios::binary);
    //std::ifstream rom_file("./ROMS/n64-systemtest.z64", std::ios::binary);
    std::vector<uint8_t> rom(
        (std::istreambuf_iterator<char>(rom_file)),
        std::istreambuf_iterator<char>()
    );
    rom_file.close();
    GFX gfx;
    Pif pif;
    Rdram rdram;
    Cartridge cartridge(rom);
    VR4300Interpreter vr4300;
    EventQ eventq;
    RCP rcp(vr4300, rdram, cartridge, pif, gfx, eventq);
    eventq.enqueue(rcp.vr4300.cycles + 1500000,EventType::VI_DONE);
    eventq.rcp = &rcp;
    vr4300.rcp = &rcp;
    pif.rcp = &rcp;
    
    //IPL2 skip
    vr4300.PC = 0xffffffffa4000040;
    vr4300.cp0.config = 0x7006e463;
    vr4300.cp0.status = 0x34000000;
    vr4300.cp0.stash_status();

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

    //will be 93 MHz hopefully
    while(true){
        vr4300.on_pclock();

        eventq.process_queue();

    }
    return 0;

}