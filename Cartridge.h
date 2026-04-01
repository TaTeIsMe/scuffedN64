#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#pragma once

#include"MemoryArea.h"
#include<vector>
class Cartridge : public MemoryArea
{
public:
    Cartridge(std::vector<uint8_t> rom);

    std::vector<uint8_t> mem;

    void write_byte(uint32_t address, uint8_t byte) override;
    uint8_t read_byte(uint32_t address) override;
private:

};

#endif