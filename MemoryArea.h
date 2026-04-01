#ifndef MEMORYAREA_H
#define MEMORYAREA_H

#pragma once

#include<cstdint>

class MemoryArea
{
public:
    virtual void write_size(uint32_t address, uint64_t value, uint8_t size);
    virtual uint64_t read_size(uint32_t address, uint8_t size);

private:

};

#endif