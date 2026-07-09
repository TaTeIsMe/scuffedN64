#pragma once
#include<cstdint>

class VR4300
{
public:
    virtual void on_pclock();
    virtual void update_hardware_interrupt(uint8_t enable, uint8_t value);
};
