#include "RCP.h"
#include<iostream>


RCP::RCP(Rdram& rdram, Cartridge& cartridge, Pif& pif):rdram(rdram), cartridge(cartridge), pif(pif){}

uint32_t RCP::read_word(uint64_t address){
    uint32_t word = (read_byte(address)<<24) |
                    (read_byte(address + 1) << 16) | 
                    (read_byte(address + 2) << 8) | 
                    (read_byte(address + 3));
    return word;
}
void RCP::write_word(uint64_t address, uint32_t word){
    write_byte(address, (uint8_t)(word >> 24));
    write_byte(address + 1, (uint8_t)(word >> 16));
    write_byte(address + 2, (uint8_t)(word >> 8));
    write_byte(address + 3, (uint8_t)(word));
}

uint16_t RCP::read_halfword(uint64_t address){
    uint16_t halfword = (read_byte(address) << 8) | 
                        (read_byte(address + 1));
    return halfword;
}

void RCP::write_halfword(uint64_t address, uint16_t halfword){
    write_byte(address, (uint8_t)(halfword >> 8));
    write_byte(address + 1, (uint8_t)(halfword));
}

uint64_t RCP::read_doubleword(uint64_t address){
    uint64_t doubleword = ((uint64_t)read_byte(address) << 56 ) |
                            ((uint64_t)read_byte(address + 1) << 48) | 
                            ((uint64_t)read_byte(address + 2) << 40) | 
                            ((uint64_t)read_byte(address + 3) << 32) |
                            ((uint64_t)read_byte(address + 4) << 24) |
                            ((uint64_t)read_byte(address + 5) << 16) | 
                            ((uint64_t)read_byte(address + 6) << 8) | 
                            ((uint64_t)read_byte(address + 7));
    return doubleword;
}
void RCP::write_doubleword(uint64_t address, uint64_t doubleword){
    write_byte(address, (uint8_t)(doubleword >> 56));
    write_byte(address + 1, (uint8_t)(doubleword >> 48));
    write_byte(address + 2, (uint8_t)(doubleword >> 40));
    write_byte(address + 3, (uint8_t)(doubleword >> 32));
    write_byte(address + 4, (uint8_t)(doubleword >> 24));
    write_byte(address + 5, (uint8_t)(doubleword >> 16));
    write_byte(address + 6, (uint8_t)(doubleword >> 8));
    write_byte(address + 7, (uint8_t)(doubleword));
}

uint8_t RCP::read_byte(uint64_t address){
    uint8_t byte = 0;
    for (uint32_t i = 0; i < sizeof(map)/sizeof(MemoryMapping); i++)
    {
        if(address >= map[i].start && address <= map[i].end){
            byte = map[i].memory_area.read_byte(address - map[i].start);
        }
    }
    
    return byte;
}

void RCP::write_byte(uint64_t address, uint8_t byte){

    for (uint32_t i = 0; i < sizeof(map)/sizeof(MemoryMapping); i++)
    {
        if(address >= map[i].start && address <= map[i].end){
            map[i].memory_area.write_byte(address - map[i].start, byte);
        }
    }
}

uint64_t RCP::read_size(uint64_t address, uint8_t size){
    uint64_t result = 0;
    for (int i = 0; i < size; i++)
    {
        result |= read_byte(address + i) << ((size - 1 - i) * 8);
    }
    return result;
}

void RCP::write_size(uint64_t address, uint64_t value, uint8_t size){
    for (int i = 0; i < size; i++)
    {
        write_byte((address + i), (uint8_t)(value >> ((size - 1 - i) * 8)));
    }
}