#include "VideoInterface.h"

void VideoInterface::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    address = address & 0x3F; // mirroring
    uint16_t reg_id = address >> 2;

    switch (address)
    {
    case 0:
        regs[reg_id] = value & 0x1FBFF;
        break;
    case 0x4:
        regs[reg_id] = value & 0xFFFFFF;
        break;
    case 0x8:
        regs[reg_id] = value & 0xFFF;
        break;
    case 0xC:
    case 0x10:
    case 0x18:
        regs[reg_id] = value & 0x3FF;
        break;
    case 0x14:
        regs[reg_id] = value & 0x3FFFFFFF;
        break;
    case 0x1C:
        regs[reg_id] = value & 0x1F0FFF;
        break;
    case 0x20:
    case 0x30:
    case 0x34:
        regs[reg_id] = value & 0xFFF0FFF;
        break;
    case 0x24:
    case 0x28:
    case 0x2C:
        regs[reg_id] = value & 0x3FF03FF;
        break;
    case 0x38:
        regs[reg_id] = value & 0x7F;
        break;
    case 0x3C:
        regs[reg_id] = value &0xFFFFFFFF;
        break;
    default:
        break;
    }
}

uint64_t VideoInterface::read_size(uint32_t address, uint8_t size)
{
    address = address & 0x3F; //mirroring
    uint16_t reg_id = address >> 2;
    switch (address)
    {
    case 0:     return regs[reg_id] & 0x1FBFF;
    case 0x4:   return regs[reg_id] & 0xFFFFFF;
    case 0x8:
    case 0xC:
    case 0x10:
    case 0x18:  return regs[reg_id] & 0x3FF;

    case 0x14:  return regs[reg_id] & 0x3FFFFFFF;
    case 0x1C:  return regs[reg_id] & 0x1F0FFF;
    case 0x20:
    case 0x30:
    case 0x34:  return regs[reg_id] & 0xFFF0FFF;

    case 0x24:
    case 0x28:
    case 0x2C:  return regs[reg_id] & 0x3FF03FF;

    case 0x38:  return regs[reg_id] & 0x7F;
    case 0x3C:  return regs[reg_id] & 0xFFFFFFFF;

    default:    return 0;
    }

    return 0;
}
