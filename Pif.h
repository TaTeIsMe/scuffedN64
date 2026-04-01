#ifndef PIF_H
#define PIF_H

#pragma once
#include"MemoryArea.h"
class Pif: public MemoryArea
{
public:
    Pif();
    void write_size(uint32_t address, uint64_t value, uint8_t size) override;
    uint64_t read_size(uint32_t address, uint8_t size) override;
private:

};

#endif