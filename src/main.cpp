#include <iostream>
#include <fstream>
#include <cstdint>
#include "VR4300.h"
#include "RCP.h"
#include "Cartridge.h"
#include "Rdram.h"
#include "Pif.h"
#include <vector>
#include"GFX.h"
#include <chrono>
#include <iostream>

int main(){
    
    std::ifstream rom_file("./ROMS/ZELOOTD.z64", std::ios::binary);
    //std::ifstream rom_file("n64-systemtest.z64", std::ios::binary);
    std::vector<uint8_t> rom(
        (std::istreambuf_iterator<char>(rom_file)),
        std::istreambuf_iterator<char>()
    );
    rom_file.close();
    GFX gfx;
    Pif pif;
    Rdram rdram;
    Cartridge cartridge(rom);
    VR4300 vr4300;
    RCP rcp(vr4300, rdram, cartridge, pif, gfx);
    vr4300.rcp = &rcp;
    
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
    
    uint32_t cycles = 0;
    
    uint64_t loop_count = 0;
    auto start_time = std::chrono::high_resolution_clock::now();

    //will be 31 MHz hopefully
    while(true){
        //cpu around 93 mhz
        vr4300.on_clock();
        vr4300.on_clock();
        vr4300.on_clock();

        //insert 2 RCP cycles here (62 mhz) (not really, rcp is replaced anyway)

        //insert 60 hz interrupts here
        if (cycles >= 500000){//figure out this number
            rcp.mi.route_interrupt(InterruptSource::VI);
            cycles = 0;
        }cycles++;

        //replace these later with scheduling events
        if(rcp.rsp.regs.SP_DMA_BUSY){
            rcp.rsp.continue_dma();
        }
        if(rcp.pi.dma_busy){
            rcp.pi.continue_dma();
        }
        if(rcp.si.SI_STATUS & 1){
            rcp.si.continue_dma();
        }
        if(rcp.rsp.task_in_progress){
            rcp.rsp.continue_task();
        }

        // --- MEASURE PERFORMANCE ---
        loop_count++;
        if (loop_count >= 10'000'000) [[unlikely]] { 
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end_time - start_time;
            
            double hz = loop_count / elapsed.count();
            double mhz = hz / 1'000'000.0;
            
            std::cout << "Loop Execution Speed: " << hz << " Hz (" << mhz << " MHz)\n";
            
            // Reset tracking window
            loop_count = 0;
            start_time = std::chrono::high_resolution_clock::now();
        }

    }
    return 0;

}