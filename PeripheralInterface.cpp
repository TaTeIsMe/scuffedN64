#include "PeripheralInterface.h"
#include "Rdram.h"
#include "Cartridge.h"

void PeripheralInterface::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    address &= 0x3F; //mirroring
    uint16_t reg_id = address >> 2;
    switch (address)
    {
    case 0:
        regs[reg_id] = value & 0xFFFFFE;
        break;
    case 0x4:
        regs[reg_id] = value & 0xFFFFFFFE;
        break;
    case 0x8:
    case 0xC:
        PI_RD_LEN = value & 0xFFFFFF;
        PI_WR_LEN = value & 0xFFFFFF;
        dma_direction = (reg_id == 3);
        start_dma();
        break;
    case 0x10:
        if(value & 1) finish_dma();
        if(value & 2) dma_completed = 0;
        break;
    case 0x14: case 0x24: case 0x18: case 0x28:
        regs[reg_id] = value & 0xFF;
        break;
    case 0x1C: case 0x2C:
        regs[reg_id] = value & 0xF;
        break;
    case 0x20: case 0x30:
        regs[reg_id] = value & 0x3;
        break;
    default:
        break;
    }
}

uint64_t PeripheralInterface::read_size(uint32_t address, uint8_t size)
{
    address &= 0x3F; //mirroring
    uint16_t reg_id = address >> 2;
    switch (address)
    {
    case 0:
    case 0x4:
        return regs[reg_id];
        break;
    case 0x8:
    case 0xC:
        return 0x7f;
        break;
    case 0x10:
        return (dma_completed << 3) | (dma_error << 2) | (io_busy << 1) | dma_busy;
        break;
    case 0x14: case 0x24: case 0x18: case 0x28: case 0x1C: case 0x2C: case 0x20: case 0x30:
        return regs[reg_id];
        break;
    default:
        break;
    }
    return 0;
}

PeripheralInterface::PeripheralInterface(Rdram &rdram, Cartridge &cartridge):rdram(rdram),cartridge(cartridge){}

void PeripheralInterface::start_dma()
{
    dma_busy = true;
    dma_completed = false;
    dma_error = false;
    len = PI_RD_LEN + 1;
    current_ram_addr = PI_DRAM_ADDR;
    //for now this only handles cartridge access
    current_cart_addr = PI_CART_ADDR - 0x10000000;
}

void PeripheralInterface::continue_dma()
{
    //for now this only handles cartridge access
    while(len > 0){
        dma_direction?
        rdram.write_size(current_ram_addr, cartridge.read_size(current_cart_addr, 8), 8):
        cartridge.write_size(current_cart_addr, rdram.read_size(current_ram_addr, 8), 8);
        current_ram_addr += 8;
        current_cart_addr += 8;
        len -= 8;
        return;
    }
    finish_dma();
}

void PeripheralInterface::finish_dma()
{
    PI_CART_ADDR = current_cart_addr + 0x10000000;
    PI_DRAM_ADDR = current_ram_addr;
    dma_busy = false;
    dma_completed = true;
}
