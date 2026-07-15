#include "Pif.h"
#include <cstddef>
#include "RCP.h"
#include"GFX.h"

struct Input{
    uint8_t byte_1 = 0;
    uint8_t byte_2 = 0;
    uint8_t XAxis = 0;
    uint8_t YAxis = 0;

    constexpr bool A()  { return byte_1 & 0x80; }
    constexpr bool B()  { return byte_1 & 0x40; }
    constexpr bool Z()  { return byte_1 & 0x20; }
    constexpr bool S()  { return byte_1 & 0x10; }
    constexpr bool dU() { return byte_1 & 0x08; }
    constexpr bool dD() { return byte_1 & 0x04; }
    constexpr bool dL() { return byte_1 & 0x02; }
    constexpr bool dR() { return byte_1 & 0x01; }

    constexpr bool RST(){ return byte_2 & 0x80; }
    constexpr bool LT() { return byte_2 & 0x20; }
    constexpr bool RT() { return byte_2 & 0x10; }
    constexpr bool cU() { return byte_2 & 0x08; }
    constexpr bool cD() { return byte_2 & 0x04; }
    constexpr bool cL() { return byte_2 & 0x02; }
    constexpr bool cR() { return byte_2 & 0x01; }
    constexpr void A(bool val)   { if (val) byte_1 |= 0x80; else byte_1 &= ~0x80; }
    constexpr void B(bool val)   { if (val) byte_1 |= 0x40; else byte_1 &= ~0x40; }
    constexpr void Z(bool val)   { if (val) byte_1 |= 0x20; else byte_1 &= ~0x20; }
    constexpr void S(bool val)   { if (val) byte_1 |= 0x10; else byte_1 &= ~0x10; }
    constexpr void dU(bool val)  { if (val) byte_1 |= 0x08; else byte_1 &= ~0x08; }
    constexpr void dD(bool val)  { if (val) byte_1 |= 0x04; else byte_1 &= ~0x04; }
    constexpr void dL(bool val)  { if (val) byte_1 |= 0x02; else byte_1 &= ~0x02; }
    constexpr void dR(bool val)  { if (val) byte_1 |= 0x01; else byte_1 &= ~0x01; }

    constexpr void RST(bool val) { if (val) byte_2 |= 0x80; else byte_2 &= ~0x80; }
    constexpr void LT(bool val)  { if (val) byte_2 |= 0x20; else byte_2 &= ~0x20; }
    constexpr void RT(bool val)  { if (val) byte_2 |= 0x10; else byte_2 &= ~0x10; }
    constexpr void cU(bool val)  { if (val) byte_2 |= 0x08; else byte_2 &= ~0x08; }
    constexpr void cD(bool val)  { if (val) byte_2 |= 0x04; else byte_2 &= ~0x04; }
    constexpr void cL(bool val)  { if (val) byte_2 |= 0x02; else byte_2 &= ~0x02; }
    constexpr void cR(bool val)  { if (val) byte_2 |= 0x01; else byte_2 &= ~0x01; }
};

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
            //TODO seperate inputs from pif

            Input input;
            input.A(glfwGetKey(rcp->gfx.window,GLFW_KEY_Q));
            input.B(glfwGetKey(rcp->gfx.window,GLFW_KEY_W));
            input.Z(glfwGetKey(rcp->gfx.window,GLFW_KEY_E));
            input.S(glfwGetKey(rcp->gfx.window,GLFW_KEY_R));
            input.dU(glfwGetKey(rcp->gfx.window,GLFW_KEY_T));
            input.dD(glfwGetKey(rcp->gfx.window,GLFW_KEY_Y));
            input.dL(glfwGetKey(rcp->gfx.window,GLFW_KEY_U));
            input.dR(glfwGetKey(rcp->gfx.window,GLFW_KEY_I));
            input.RST(glfwGetKey(rcp->gfx.window,GLFW_KEY_O));
            input.LT(glfwGetKey(rcp->gfx.window,GLFW_KEY_P));
            input.RT(glfwGetKey(rcp->gfx.window,GLFW_KEY_A));
            input.cU(glfwGetKey(rcp->gfx.window,GLFW_KEY_S));
            input.cD(glfwGetKey(rcp->gfx.window,GLFW_KEY_D));
            input.cL(glfwGetKey(rcp->gfx.window,GLFW_KEY_F));
            input.cR(glfwGetKey(rcp->gfx.window,GLFW_KEY_G));
            int8_t x = 0;
            int8_t y = 0;

            if (glfwGetKey(rcp->gfx.window, GLFW_KEY_LEFT) == GLFW_PRESS)
                x = -80;
            else if (glfwGetKey(rcp->gfx.window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                x = 80;

            if (glfwGetKey(rcp->gfx.window, GLFW_KEY_UP) == GLFW_PRESS)
                y = 80;
            else if (glfwGetKey(rcp->gfx.window, GLFW_KEY_DOWN) == GLFW_PRESS)
                y = -80;

            input.XAxis = static_cast<uint8_t>(x);
            input.YAxis = static_cast<uint8_t>(y);

            *result_ptr = input.byte_1;
            result_ptr ++;
            *result_ptr = input.byte_2;
            result_ptr ++;
            *result_ptr = input.XAxis;
            result_ptr ++;
            *result_ptr = input.YAxis;
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
