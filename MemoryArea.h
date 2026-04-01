#ifndef MEMORYAREA_H
#define MEMORYAREA_H

#pragma once

#include<cstdint>

class MemoryArea
{
public:
    MemoryArea();
    ~MemoryArea();

    virtual void write_byte(uint32_t address, uint8_t byte);
    virtual uint8_t read_byte(uint32_t address);

private:

};

#endif