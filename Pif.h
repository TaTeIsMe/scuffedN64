#ifndef PIF_H
#define PIF_H

#pragma once
#include"MemoryArea.h"
class Pif: public MemoryArea
{
public:
    Pif();
    void write_byte(uint32_t address, uint8_t byte) override;
    uint8_t read_byte(uint32_t address) override;
private:

};

#endif