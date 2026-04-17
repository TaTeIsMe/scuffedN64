#include "RCP.h"
#include<iostream>


RCP::RCP(Rdram& rdram, Cartridge& cartridge, Pif& pif):
rsp(*this, rdram), 
pi(rdram, cartridge), 
rdram(rdram), 
cartridge(cartridge), 
pif(pif) {}

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