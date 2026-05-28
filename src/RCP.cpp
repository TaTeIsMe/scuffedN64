#include "RCP.h"
#include<iostream>

#include"Rdram.h"
#include"Cartridge.h"
#include"Pif.h"
#include"VR4300.h"

RCP::RCP(VR4300& vr4300,Rdram& rdram, Cartridge& cartridge, Pif& pif, GFX& gfx):
rsp(*this,gfx, rdram), 
mi(*this),
vi(*this),
ai(*this),
pi(*this),
ri(*this),
si(*this),
vr4300(vr4300),
rdram(rdram), 
cartridge(cartridge), 
pif(pif),
gfx(gfx) {}

uint64_t RCP::read_size(uint64_t address, uint8_t size){
    uint64_t value = 0;
    for (uint32_t i = 0; i < sizeof(map)/sizeof(MemoryMapping); i++)
    {
        if(address >= map[i].start && address <= map[i].end){
            value = map[i].memory_area.read_size((address - map[i].start)&map[i].access_mask, size);
        }
    }
    return value;
}

void RCP::write_size(uint64_t address, uint64_t value, uint8_t size){

    for (uint32_t i = 0; i < sizeof(map)/sizeof(MemoryMapping); i++)
    {
        if(address >= map[i].start && address <= map[i].end){
            map[i].memory_area.write_size((address - map[i].start)&map[i].access_mask, value, size);
            break;
        }
    }
}