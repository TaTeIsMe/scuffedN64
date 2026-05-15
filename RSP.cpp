#include "RSP.h"
#include "RCP.h"

RSP::RSPRegs::RSPRegs(RSP &rsp):rsp(rsp){}

void inline set_clear_reg_bit(uint8_t set, uint8_t clear,uint32_t& reg, uint32_t bit){
    if(set && clear)return;
    if(set) reg |= 1 << bit;
    if(clear) reg &= ~(1 << bit);
};

void RSP::RSPRegs::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    address &= 0x1F; // address mirroring

    uint32_t reg_id = address >> 2;

    switch (address)
    {
    case 0:
        rsp.pending_dma[reg_id] = value & 0x1FF8;
        break;
    case 0x4:
        rsp.pending_dma[reg_id] = value & 0xFFFFF8;
        break;
    case 0x8:
    case 0xC:
        if(!SP_DMA_FULL){
            rsp.pending_dma[2] = value & ~0x00700007;
            rsp.pending_dma[3] = value & ~0x00700007;
            rsp.pending_dma_direction = (reg_id == 3);
            if(SP_DMA_BUSY){
                SP_DMA_FULL = 1; 
                SP_STATUS |= (1 << 3); 
            }
        }
        if(!SP_DMA_BUSY){
            regs[0] = rsp.pending_dma[0];
            regs[1] = rsp.pending_dma[1];
            regs[2] = rsp.pending_dma[2];
            regs[3] = rsp.pending_dma[3];
            rsp.dma_direction = rsp.pending_dma_direction;
            SP_DMA_BUSY = 1;
            SP_STATUS |= (1 << 2);
            rsp.start_dma();
        }
        break;
    case 0x10:{
        uint8_t SET_SIG7 = (value >> 24) & 1;
        uint8_t SET_SIG6 = (value >> 22) & 1;
        uint8_t SET_SIG5 = (value >> 20) & 1;
        uint8_t SET_SIG4 = (value >> 18) & 1;
        uint8_t SET_SIG3 = (value >> 16) & 1;
        uint8_t SET_SIG2 = (value >> 14) & 1;
        uint8_t SET_SIG1 = (value >> 12) & 1;
        uint8_t SET_SIG0 = (value >> 10) & 1;
        uint8_t CLR_SIG7 = (value >> 23) & 1;
        uint8_t CLR_SIG6 = (value >> 21) & 1;
        uint8_t CLR_SIG5 = (value >> 19) & 1;
        uint8_t CLR_SIG4 = (value >> 17) & 1;
        uint8_t CLR_SIG3 = (value >> 15) & 1;
        uint8_t CLR_SIG2 = (value >> 13) & 1;
        uint8_t CLR_SIG1 = (value >> 11) & 1;
        uint8_t CLR_SIG0 = (value >> 9) & 1;
        uint8_t SET_INTBREAK = (value >> 8) & 1;
        uint8_t CLR_INTBREAK = (value >> 7) & 1;
        uint8_t SET_SSTEP = (value >> 6) & 1;
        uint8_t CLR_SSTEP = (value >> 5) & 1;
        uint8_t SET_INTR = (value >> 4) & 1;
        uint8_t CLR_INTR = (value >> 3) & 1;
        uint8_t CLR_BROKE = (value >> 2) & 1;
        uint8_t SET_HALT = (value >> 1) & 1;
        uint8_t CLR_HALT = (value >> 0) & 1;

        set_clear_reg_bit(SET_SIG7, CLR_SIG7, SP_STATUS, 14);
        set_clear_reg_bit(SET_SIG6, CLR_SIG6, SP_STATUS, 13);
        set_clear_reg_bit(SET_SIG5, CLR_SIG5, SP_STATUS, 12);
        set_clear_reg_bit(SET_SIG4, CLR_SIG4, SP_STATUS, 11);
        set_clear_reg_bit(SET_SIG3, CLR_SIG3, SP_STATUS, 10);
        set_clear_reg_bit(SET_SIG2, CLR_SIG2, SP_STATUS, 9);
        set_clear_reg_bit(SET_SIG1, CLR_SIG1, SP_STATUS, 8);
        set_clear_reg_bit(SET_SIG0, CLR_SIG0, SP_STATUS, 7);
        set_clear_reg_bit(SET_INTBREAK, CLR_INTBREAK, SP_STATUS, 6);
        set_clear_reg_bit(SET_SSTEP, CLR_SSTEP, SP_STATUS, 5);
        if(SET_INTR)rsp.rcp.mi.route_interrupt(InterruptSource::SP);
        if(CLR_INTR)rsp.rcp.mi.clear_interrupt(InterruptSource::SP);
        set_clear_reg_bit(0, CLR_BROKE, SP_STATUS, 1);
        set_clear_reg_bit(SET_HALT, CLR_HALT, SP_STATUS, 0);
        break;
    }
    case 0x14:
    case 0x18:
        break;
    case 0x1C:
        SP_SEMAPHORE = value & 1;
        break;
    default:
        break;
    }

}

uint64_t RSP::RSPRegs::read_size(uint32_t address, uint8_t size)
{
    address = address & 0x1F; //mirroring
    uint32_t reg_id = address >> 2;
    return regs[reg_id];
}

void RSP::Imem::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    if((address + size - 1) >= mem.size())return;
    
    for (uint8_t i = 0; i < size; i++)
    {
        mem[address + i] = (value >> ((size - 1 - i) * 8)) & 0xFF;
    }
}

uint64_t RSP::Imem::read_size(uint32_t address, uint8_t size)
{
    if((address + size - 1) >= mem.size()) return 0;
    uint64_t result = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        result |= (uint64_t)mem[(address + i)] << ((size - 1 - i) * 8);
    }
    return result;
}

RSP::Dmem::Dmem():mem(0x1000){}
RSP::Imem::Imem():mem(0x1000){}

void RSP::Dmem::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    if((address + size - 1) >= mem.size())return;
    
    for (uint8_t i = 0; i < size; i++)
    {
        mem[address + i] = (value >> ((size - 1 - i) * 8)) & 0xFF;
    }
}

uint64_t RSP::Dmem::read_size(uint32_t address, uint8_t size)
{
    if((address + size - 1) >= mem.size()) return 0;
    uint64_t result = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        result |= (uint64_t)mem[(address + i)] << ((size - 1 - i) * 8);
    }
    return result;
}

RSP::RSP(RCP& rcp, Rdram& rdram): rcp(rcp), regs(*this), rdram(rdram){}

void RSP::start_dma()
{
    uint32_t ram_start = regs.SP_DMA_RAMADDR & 0xFFFFF8;
    uint16_t mem_start = regs.SP_DMA_SPADDR & 0xFF8;
    len = (regs.SP_DMA_RDLEN & 0xFFF) | 7;
    start_len = len;
    count = ((regs.SP_DMA_RDLEN >> 12) & 0xFF) + 1;
    skip = (regs.SP_DMA_RDLEN >> 20) & 0xFF8;
    current_ram_addr = ram_start;
    current_mem_addr = mem_start;
}

void RSP::continue_dma()
{
    bool mem_bank = regs.SP_DMA_SPADDR & 0x1000;
    //this might end up copying too much memory
    if(count > 0){
        if(len >= 0){
            len -= 8;
            if(dma_direction){
                mem_bank?
                rdram.write_size(current_ram_addr,imem.read_size(current_mem_addr,8),8):
                rdram.write_size(current_ram_addr,dmem.read_size(current_mem_addr,8),8);
            }else{
                mem_bank?
                imem.write_size(current_mem_addr,rdram.read_size(current_ram_addr,8),8):
                dmem.write_size(current_mem_addr,rdram.read_size(current_ram_addr,8),8);
            }
            current_ram_addr += 8;
            current_mem_addr = (current_mem_addr + 8) & 0xFFF;
            regs.SP_DMA_RDLEN = (skip << 20) | (count << 12) | (len & 0xFF8);
            regs.SP_DMA_WRLEN = (skip << 20) | (count << 12) | (len & 0xFF8);
            return;
        }
        count--;
        len = start_len;
        current_ram_addr += skip;
    }
    finish_dma();
}

void RSP::finish_dma()
{
    if(regs.SP_DMA_FULL){
        dma_direction = pending_dma_direction;
        regs.regs[0] = pending_dma[0];
        regs.regs[1] = pending_dma[1];
        regs.regs[2] = pending_dma[2];
        regs.regs[3] = pending_dma[3];
        regs.SP_DMA_BUSY = 1;
        regs.SP_STATUS |= (1 << 2);
        start_dma();
        regs.SP_DMA_FULL = 0;
        regs.SP_STATUS &= ~(1 << 3);
    }else{
        regs.SP_DMA_BUSY = 0;
        regs.SP_STATUS &= ~(1 << 2);
    }
}
