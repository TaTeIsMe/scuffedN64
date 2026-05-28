#include "Pif.h"
#include <cstddef>

uint8_t pak_crc(uint8_t *data, size_t len)
{
    uint8_t crc = 0x00;
    size_t i, j;

    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if (crc & 0x80)
                crc = (uint8_t)((crc << 1) ^ 0x85);
            else
                crc <<= 1;
        }
    }
    return crc;
}


Pif::Pif()
{

}

void Pif::write_size(uint32_t address, uint64_t value, uint8_t size)
{
}

uint64_t Pif::read_size(uint32_t address, uint8_t size)
{
    return 0;
}

void Pif::handle_command()
{
    if(ram[63] == 0x01) config_joy_frame();
    ram[63] = 0;

}

void Pif::config_joy_frame()
{
    uint8_t ptr = 0;
    uint8_t tx;
    uint8_t rx;
    uint32_t cmd;

    uint8_t current_controller = 0;

    while(ptr < 64)
    {
        uint8_t* tx_ptr = &ram[ptr];
        tx = ram[ptr++];
        
        if(tx == 0xFE) //end
            break;

        if(tx == 0xFF) //nop
            continue;
        
        if(tx == 0x00){
            current_controller++;
            continue;
        }

        uint8_t* rx_ptr = &ram[ptr];
        rx = ram[ptr++];

        uint8_t* cmd_ptr = &ram[ptr];
        cmd = ram[ptr];

        ptr += tx;
        uint8_t* response = &ram[ptr];
        handle_device_command(cmd, response, tx_ptr, rx_ptr, cmd_ptr, current_controller);

        ptr += rx;
        current_controller++;
    }
}

void Pif::handle_device_command(uint32_t cmd, uint8_t *result_ptr,uint8_t* tx_ptr , uint8_t* rx_ptr, uint8_t* cmd_ptr, uint8_t current_controller)
{
    if(current_controller != 0){
        *rx_ptr |= 0x80;
        return;
    }
    switch (cmd)
    {
    case 0x00:
        if(current_controller == 0){
            *result_ptr = 0x05;
            result_ptr ++;
            *result_ptr = 0x00;
            result_ptr ++;
            *result_ptr = 0x01;
        } 
        break;
    case 0x01:
        if(current_controller == 0){
            *result_ptr = 0x00;
            result_ptr ++;
            *result_ptr = 0x00;
            result_ptr ++;
            *result_ptr = 0x00;
            result_ptr ++;
            *result_ptr = 0x00;
        }
        break;
    case 0x02:
        if(current_controller == 0){
            for (int i = 0; i < *rx_ptr - 1; i++)
            {
                *result_ptr = 0x00;
                result_ptr ++;
            }
            *result_ptr = pak_crc(cmd_ptr + 3,32);
        }
        
        break;
    case 0x03:
        if(current_controller == 0)
            *result_ptr = pak_crc(cmd_ptr + 3,32);
    default:
        break;
    }
}
