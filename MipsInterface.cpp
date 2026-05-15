#include "MipsInterface.h"
#include "RCP.h"
#include "VR4300.h"

MipsInterface::MipsInterface(RCP &rcp):rcp(rcp){}

void MipsInterface::route_interrupt(InterruptSource src){
    MI_INTERRUPT |= (uint8_t) src;
    if(MI_INTERRUPT & MI_MASK)
        rcp.vr4300.update_hardware_interrupt(1,1);
}

void MipsInterface::clear_interrupt(InterruptSource src)
{
    MI_INTERRUPT &= (~(uint8_t)src) & 0x3F;
    if(!(MI_INTERRUPT & MI_MASK))
        rcp.vr4300.update_hardware_interrupt(1,0);
}

void inline set_clear_reg_bit(uint8_t set, uint8_t clear,uint32_t& reg, uint32_t bit){
    if(set && clear)return;
    if(set) reg |= 1 << bit;
    if(clear) reg &= ~(1 << bit);
};

void MipsInterface::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    address &= 0xF; //mirroring
    uint16_t reg_id = address >> 2;
    switch (address)
    {
    case 0:{
        uint8_t set_upper = (value >> 13) & 1;
        uint8_t clear_upper = (value >> 12) & 1;
        uint8_t clear_dp = (value >> 11) & 1;
        uint8_t set_ebus = (value >> 10) & 1;
        uint8_t clear_ebus = (value >> 9) & 1;
        uint8_t set_repeat = (value >> 8) & 1;
        uint8_t clear_repeat = (value >> 7) & 1;
        set_clear_reg_bit(set_upper,clear_upper,MI_MODE,9);
        if(clear_dp)
            clear_interrupt(InterruptSource::DP);
        set_clear_reg_bit(set_ebus,clear_ebus,MI_MODE,8);
        set_clear_reg_bit(set_repeat,clear_repeat,MI_MODE,7);
        MI_MODE &= 0x00000380;
        MI_MODE |= value & 0x7F;
        break;
    }
    case 0x4:
    case 0x8:
        break;
    case 0xC:{
        uint8_t set_DP = (value >> 11) & 1;
        uint8_t set_PI = (value >> 9) & 1;
        uint8_t set_VI = (value >> 7) & 1;
        uint8_t set_AI = (value >> 5) & 1;
        uint8_t set_SI = (value >> 3) & 1;
        uint8_t set_SP = (value >> 1) & 1;
        uint8_t clear_DP = (value >> 10) & 1;
        uint8_t clear_PI = (value >> 8) & 1;
        uint8_t clear_VI = (value >> 6) & 1;
        uint8_t clear_AI = (value >> 4) & 1;
        uint8_t clear_SI = (value >> 2) & 1;
        uint8_t clear_SP = (value >> 0) & 1;
        set_clear_reg_bit(set_DP,clear_DP,MI_MASK,5);
        set_clear_reg_bit(set_PI,clear_PI,MI_MASK,4);
        set_clear_reg_bit(set_VI,clear_VI,MI_MASK,3);
        set_clear_reg_bit(set_AI,clear_AI,MI_MASK,2);
        set_clear_reg_bit(set_SI,clear_SI,MI_MASK,1);
        set_clear_reg_bit(set_SP,clear_SP,MI_MASK,0);
        rcp.vr4300.update_hardware_interrupt(1,(MI_INTERRUPT & MI_MASK) != 0);
        break;
    }
    default:
        break;
    }
}

uint64_t MipsInterface::read_size(uint32_t address, uint8_t size)
{
    address &= 0xF; //mirroring
    uint16_t reg_id = address >> 2;
    switch (address)
    {
    case 0:
        return regs[reg_id];
        break;
    case 0x4:
        return 0x02020102;
        break;
    case 0x8:
        return regs[reg_id];
        break;
    case 0xC:
        return regs[reg_id];
        break;
    default:
        break;
    }
    return 0;
}

