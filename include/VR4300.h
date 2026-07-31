#pragma once
#include<cstdint>

class VR4300
{
public:
    uint64_t cycles = 0;
    virtual void on_pclock();
    virtual void update_hardware_interrupt(uint8_t enable, uint8_t value);
};
