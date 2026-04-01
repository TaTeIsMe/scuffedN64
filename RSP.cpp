#include "RSP.h"
#include "RCP.h"

RSP::RSPRegs::RSPRegs(RSP &rsp):rsp(rsp){}

void RSP::RSPRegs::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    address &= 0x1F; // address mirroring

    uint32_t reg_id = address >> 2;
    if(reg_id <= 3){
        rsp.pending_dma[reg_id] = value;
        if(SP_DMA_BUSY && (reg_id == 2 || reg_id == 3)){
            SP_DMA_FULL = 1; 
            SP_STATUS |= (1 << 3); 
            rsp.pending_dma_direction = reg_id - 2;}
        if(!SP_DMA_BUSY){
            if(reg_id == 2 || reg_id == 3){
                regs[0] = rsp.pending_dma[0];
                regs[1] = rsp.pending_dma[1];
                regs[2] = rsp.pending_dma[reg_id];
                regs[3] = rsp.pending_dma[reg_id];
                rsp.dma_direction = rsp.pending_dma_direction;
                SP_DMA_BUSY = 1;
                SP_STATUS |= (1 << 2);
                rsp.start_dma();
            }
        }
        rsp.pending_dma[reg_id] &= ~0x00700007;
        regs[reg_id] &= ~0x00700007;
        return;
    }else if(reg_id == 4){
        rsp.status_input = value;
    }else if(reg_id < 7){
        return;
    }else if(reg_id == 8){
        if((value & 1) == 1)SP_SEMAPHORE = 1;
        else SP_SEMAPHORE = 0;
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
    len = regs.SP_DMA_RDLEN & 0xFF8;
    count = regs.SP_DMA_RDLEN >> 12;
    skip = (regs.SP_DMA_RDLEN >> 20) & 0xFF8;
}

void RSP::continue_dma()
{
    uint32_t ram_start = regs.SP_DMA_RAMADDR;
    uint16_t mem_start = regs.SP_DMA_SPADDR & 0xFF8;
    uint8_t mem_bank = regs.SP_DMA_SPADDR & 0x1000;
    uint32_t current_ram_addr = ram_start;
    uint16_t current_mem_addr = mem_start;
    //this might end up copying too much memory
    if(count >= 0){
        if(len >= 0){
            len -= 8;
            if(dma_direction){
                mem_bank?
                rdram.write_size(current_ram_addr,dmem.read_size(current_mem_addr,1),8):
                rdram.write_size(current_ram_addr,imem.read_size(current_mem_addr,1),8);
            }else{
                mem_bank?
                dmem.write_size(current_mem_addr,rdram.read_size(current_ram_addr,1),8):
                imem.write_size(current_mem_addr,rdram.read_size(current_ram_addr,1),8);
            }
            current_ram_addr += 8;
            current_mem_addr = (current_mem_addr + 8) & 0xFFF;
            regs.SP_DMA_RDLEN = ((uint32_t)skip << 20) | (((uint32_t)count << 12) & 0xFF8) | (len & 0xFF8);
            regs.SP_DMA_WRLEN = ((uint32_t)skip << 20) | (((uint32_t)count << 12) & 0xFF8) | (len & 0xFF8);
            return;
        }
        count--;
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
        regs.regs[3] = pending_dma[2];
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
