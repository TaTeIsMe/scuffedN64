#include <iostream>
#include <fstream>
#include <cstdint>
#include "VR4300.h"
#include "Bus.h"
#include <vector>

int main(){
    
    std::ifstream rom_file("n64-systemtest.z64", std::ios::binary);
    std::vector<uint8_t> rom(
        (std::istreambuf_iterator<char>(rom_file)),
        std::istreambuf_iterator<char>()
    );
    rom_file.close();
    uint8_t rdram[4 * 1024 * 1024];
    Bus bus(rdram, rom);
    VR4300 vr4300(bus);
    vr4300.PC = 0xffffffffA4000040;
    bus.RI_SELECT = 0x14;

    //IPL2
    for (int i = 0; i < 0xFC0; i++)
    {
        bus.rsp_dmem[i] = rom[i];
    }
    
    while(true){
        vr4300.on_clock();
    }

}