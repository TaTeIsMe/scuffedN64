#ifndef PIF_H
#define PIF_H

#pragma once
#include"MemoryArea.h"
class Pif: public MemoryArea
{
public:
    class RCP* rcp;
    Pif();
    void write_size(uint32_t address, uint64_t value, uint8_t size) override;
    uint64_t read_size(uint32_t address, uint8_t size) override;


    uint8_t ram[64]{};
    void handle_command();
    void config_joy_frame();
    void handle_device_command(uint32_t cmd, uint8_t* result_ptr,uint8_t* tx_ptr, uint8_t* rx_ptr, uint8_t* cmd_ptr, uint8_t current_controller);

private:

};

#endif