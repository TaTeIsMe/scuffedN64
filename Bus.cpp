#include "Bus.h"
#include<iostream>


Bus::Bus(uint8_t* rdram, std::vector<uint8_t>& rom):rom(rom)
{
    (*this).rdram = rdram;
}

Bus::~Bus()
{

}

uint32_t Bus::read_word(uint64_t address)
{
    uint32_t word = (read_byte(address)<<24) |
                    (read_byte(address + 1) << 16) | 
                    (read_byte(address + 2) << 8) | 
                    (read_byte(address + 3));
    return word;
}
void Bus::write_word(uint64_t address, uint32_t word)
{
    write_byte(address, (uint8_t)(word >> 24));
    write_byte(address + 1, (uint8_t)(word >> 16));
    write_byte(address + 2, (uint8_t)(word >> 8));
    write_byte(address + 3, (uint8_t)(word));
}

uint16_t Bus::read_halfword(uint64_t address)
{
    uint16_t halfword = (read_byte(address) << 8) | 
                        (read_byte(address + 1));
    return halfword;
}

void Bus::write_halfword(uint64_t address, uint16_t halfword)
{
    write_byte(address, (uint8_t)(halfword >> 8));
    write_byte(address + 1, (uint8_t)(halfword));
}

uint64_t Bus::read_doubleword(uint64_t address)
{
    uint64_t doubleword = (read_byte(address) << 56 ) |
                            (read_byte(address + 1) << 48) | 
                            (read_byte(address + 2) << 40) | 
                            (read_byte(address + 3) << 32) |
                            (read_byte(address + 4) << 24) |
                            (read_byte(address + 5) << 16) | 
                            (read_byte(address + 6) << 8) | 
                            (read_byte(address + 7));
    return doubleword;
}
void Bus::write_doubleword(uint64_t address, uint64_t doubleword)
{
    write_byte(address, (uint8_t)(doubleword >> 56));
    write_byte(address + 1, (uint8_t)(doubleword >> 48));
    write_byte(address + 2, (uint8_t)(doubleword >> 40));
    write_byte(address + 3, (uint8_t)(doubleword >> 32));
    write_byte(address + 4, (uint8_t)(doubleword >> 24));
    write_byte(address + 5, (uint8_t)(doubleword >> 16));
    write_byte(address + 6, (uint8_t)(doubleword >> 8));
    write_byte(address + 7, (uint8_t)(doubleword));
}

uint8_t Bus::read_byte(uint64_t address)
{
    uint8_t byte = 0;
    for (int i = 0; i < sizeof(map)/sizeof(MemoryMapping); i++)
    {
        if(address >= map[i].start && address <= map[i].end){
            byte = (this->*map[i].device_read_byte)(address - map[i].start);
        }
    }
    
    return byte;
}

void Bus::write_byte(uint64_t address, uint8_t byte)
{

    for (int i = 0; i < sizeof(map)/sizeof(MemoryMapping); i++)
    {
        if(address >= map[i].start && address <= map[i].end){
            (this->*map[i].device_write_byte)(address - map[i].start, byte);
        }
    }
}

uint64_t Bus::read_size(uint64_t address, uint8_t size){
    uint64_t result = 0;
    for (int i = 0; i < size; i++)
    {
        result |= read_byte(address + i) << ((size - 1 - i) * 8);
    }
    return result;
}

void Bus::write_size(uint64_t address, uint64_t value, uint8_t size){
    for (int i = 0; i < size; i++)
    {
        write_byte((address + i), (uint8_t)(value >> ((size - 1 - i) * 8)));
    }
}

void Bus::rdram_write_byte(uint32_t address, uint8_t byte){rdram[address] = byte;}
uint8_t Bus::rdram_read_byte(uint32_t address){ return rdram[address]; }

void Bus::rdram_regs_write_byte(uint32_t address, uint8_t byte){}
uint8_t Bus::rdram_regs_read_byte(uint32_t address){ return 0; }
void Bus::rdram_broadcast_write_byte(uint32_t address, uint8_t byte){}
uint8_t Bus::rdram_broadcast_read_byte(uint32_t address){ return 0; }

void Bus::rsp_dmem_write_byte(uint32_t address, uint8_t byte){rsp_dmem[address] = byte;}
uint8_t Bus::rsp_dmem_read_byte(uint32_t address){ return rsp_dmem[address]; }
void Bus::rsp_imem_write_byte(uint32_t address, uint8_t byte){rsp_imem[address] = byte;}
uint8_t Bus::rsp_imem_read_byte(uint32_t address){ return rsp_imem[address]; }

void Bus::rsp_regs_write_byte(uint32_t address, uint8_t byte){}
uint8_t Bus::rsp_regs_read_byte(uint32_t address){ return 0; }
void Bus::rdp_command_write_byte(uint32_t address, uint8_t byte){}
uint8_t Bus::rdp_command_read_byte(uint32_t address){ return 0; }
void Bus::rdp_span_write_byte(uint32_t address, uint8_t byte){}
uint8_t Bus::rdp_span_read_byte(uint32_t address){ return 0; }
void Bus::mi_write_byte(uint32_t address, uint8_t byte){}
uint8_t Bus::mi_read_byte(uint32_t address){ return 0; }
void Bus::vi_write_byte(uint32_t address, uint8_t byte){}
uint8_t Bus::vi_read_byte(uint32_t address){ return 0; }
void Bus::ai_write_byte(uint32_t address, uint8_t byte){}
uint8_t Bus::ai_read_byte(uint32_t address){ return 0; }
void Bus::pi_write_byte(uint32_t address, uint8_t byte){}
uint8_t Bus::pi_read_byte(uint32_t address){ return 0; }
void Bus::ri_write_byte(uint32_t address, uint8_t byte){}
uint8_t Bus::ri_read_byte(uint32_t address){ return 0; }
void Bus::si_write_byte(uint32_t address, uint8_t byte){}
uint8_t Bus::si_read_byte(uint32_t address){ return 0; }

void Bus::rom_write_byte(uint32_t address, uint8_t byte){
    rom[address] = byte;
    if(address== 0xB3FF0014- 0xA - 0x10000000)
        for (int i = 0xB3FF0020; i < 0xB3FF0220; i++)
        {
            std::cout<<rom[i];
        }
        
}
uint8_t Bus::rom_read_byte(uint32_t address){ return rom[address]; }

void Bus::unmapped_write(uint32_t addr,uint8_t val){}
uint8_t Bus::unmapped_read(uint32_t addr){ return 0; }