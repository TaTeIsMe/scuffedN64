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

    void write_size(uint32_t address, uint64_t value, uint8_t size) override;
    uint64_t read_size(uint32_t address, uint8_t size) override;
private:

};

#endif