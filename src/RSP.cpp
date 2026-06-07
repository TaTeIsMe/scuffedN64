#include "RSP.h"
#include "RCP.h"
#include <stack>
#include <Eigen/Dense>
#include "GFX.h"

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

        if(CLR_HALT)rsp.start_task();

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

RSP::RSP(RCP& rcp, GFX& gfx, Rdram& rdram): rcp(rcp),gfx(gfx), regs(*this), rdram(rdram){}

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
    //timer = len * count / 8 * 3;
    rcp.eventq.enqueue(rcp.cycles + len * count / 8 * 3,EventType::SP_DMA_DONE);
}

void RSP::continue_dma()
{
   //timer-=16;
   //if(timer < 0)
   //finish_dma();
}

void RSP::finish_dma()
{

    bool mem_bank = regs.SP_DMA_SPADDR & 0x1000;
    //this might end up copying too much memory
    while(true){
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
                continue;
            }
            count--;
            len = start_len;
            current_ram_addr += skip;
            continue;
        }
        break;
    }

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

void RSP::start_task()
{
    task_in_progress = true;
    current_task_type = (RSPTaskType)dmem.mem[0xFC3];
    task_timer = 0; //this should break it
    rcp.eventq.enqueue(rcp.cycles + 2000, EventType::SP_TASK_DONE);
}

void RSP::continue_task()
{
    //task_timer+=16;
    //if(task_timer > 160000)
    //    finish_task();
}

inline Eigen::Matrix4f parse_mtx_from_mem(const std::vector<uint8_t>& mem, uint32_t address)
{
    Eigen::Matrix4f mtx;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {

            int16_t intpart = (int16_t)
                ((mem[address + ((i * 4 + j) * 2)] << 8) |
                mem[address + ((i * 4 + j) * 2)+ 1]);

            uint16_t fracpart = (uint16_t)
                ((mem[address + 32 + ((i * 4 + j) * 2)] << 8) | 
                mem[address + 32 + ((i * 4 + j) * 2)+ 1]);
            
            uint32_t raw_combined = ((uint32_t)intpart << 16) | fracpart;
            int32_t combined = (int32_t)raw_combined;
            mtx(j,i) = (float)combined / 65536.0f;
        }
    }
    return mtx;
}

void RSP::process_gfx_task(OSTask task){

    uint32_t segments[16]{};
    std::stack<uint32_t> address_stack;
    std::stack<Eigen::Matrix4f> modelview_mtx_stack;
    modelview_mtx_stack.push(Eigen::Matrix4f::Identity());
    Eigen::Matrix4f projection_mtx(Eigen::Matrix4f::Identity());
    Eigen::Matrix4f modelview_projection_mtx;
    Eigen::Vector4f vertex_buffer[32];
    
    auto translate_address = [&](uint32_t address){
        if(address >> 24 == 0x80)return address & 0xFFFFFF;
        uint32_t seg_id = (address >> 24);
        uint32_t offset = address & 0xFFFFFF;
        uint32_t segment = segments[seg_id];
        return (segment + offset) & 0xFFFFFF;
    };
    
    uint32_t instr_ptr = translate_address(task.data_ptr);
    
    while(true){
        uint64_t instr = rcp.rdram.read_size(instr_ptr, 8);

        switch ((instr >> 56) & 0xFF)
        {
        case G_NOOP: // G_NOOP
            break;
        case G_VTX: // G_VTX
        {
            uint8_t numv = (instr >> 44) & 0xFF;
            uint8_t buf_id = (((instr >> 32) & 0xFF) >> 1) - numv;
            uint32_t vaddr = translate_address(instr & 0xFFFFFFFF);
            for (uint16_t i = 0; i < numv; i++)
            {
                int16_t x = rcp.rdram.read_size(vaddr,2);
                int16_t y = rcp.rdram.read_size(vaddr + 2,2);
                int16_t z = rcp.rdram.read_size(vaddr + 4,2);
                vertex_buffer[buf_id + i] = modelview_projection_mtx * Eigen::Vector4f(x,y,z,1) ;
                vaddr += 16;
            }
            
            break;
        }
        case 0x02: // G_MODIFYVTX
            break;
        case 0x03: // G_CULLDL
            break;
        case 0x04: // G_BRANCH_Z
            break;
        case G_TRI1: // G_TRI1
        {
            uint8_t v0 = ((instr >> 48) & 0xFF) / 2;
            uint8_t v1 = ((instr >> 40) & 0xFF) / 2;
            uint8_t v2 = ((instr >> 32) & 0xFF) / 2;
            gfx.vertices.insert(gfx.vertices.end(),{vertex_buffer[v0], vertex_buffer[v1], vertex_buffer[v2]});
            break;
        }
        case 0x06: // G_TRI2
        {
            uint8_t v00 = ((instr >> 48) & 0xFF) / 2;
            uint8_t v01 = ((instr >> 40) & 0xFF) / 2;
            uint8_t v02 = ((instr >> 32) & 0xFF) / 2;
            gfx.vertices.insert(gfx.vertices.end(),{vertex_buffer[v00], vertex_buffer[v01], vertex_buffer[v02]});

            uint8_t v10 = ((instr >> 16) & 0xFF) / 2;
            uint8_t v11 = ((instr >> 8) & 0xFF) / 2;
            uint8_t v12 = ((instr) & 0xFF) / 2;
            gfx.vertices.insert(gfx.vertices.end(),{vertex_buffer[v10], vertex_buffer[v11], vertex_buffer[v12]});
            break;
        }
        case 0x07: // G_QUAD
            break;
        case 0xD3: // G_SPECIAL_3
            break;
        case 0xD4: // G_SPECIAL_2
            break;
        case 0xD5: // G_SPECIAL_1
            break;
        case 0xD6: // G_DMA_IO
            break;
        case 0xD7: // G_TEXTURE
            break;
        case G_POPMTX: // G_POPMTX
        {
            uint16_t num = (instr & 0xFFFFFFFF) >> 6;
            for (int i = 0; i < num; i++)
                if(!modelview_mtx_stack.empty())
                    modelview_mtx_stack.pop();
            
            break;
        }
        case 0xD9: // G_GEOMETRYMODE
            break;
        case G_MTX: // G_MTX
        {
            uint32_t mtxaddr = translate_address(instr & 0xFFFFFFFF);
            Eigen::Matrix4f mtx = parse_mtx_from_mem(rdram.mem, mtxaddr);
            bool G_MTX_PUSH = !((instr >> 32) & 0x1);
            bool G_MTX_MUL = !((instr >> 32) & 0x2);
            bool G_MTX_PROJECTION = ((instr >> 32) & 0x4);

            Eigen::Matrix4f topmtx;

            if(G_MTX_PROJECTION)
                topmtx = projection_mtx;
            else if(!modelview_mtx_stack.empty())
                topmtx = modelview_mtx_stack.top();
            else
                topmtx = Eigen::Matrix4f::Identity();

            if(G_MTX_MUL)
                mtx = topmtx * mtx;
            if(!G_MTX_PROJECTION){
                if(G_MTX_PUSH)
                    modelview_mtx_stack.push(mtx);
                else if(!G_MTX_PUSH){
                    if(!modelview_mtx_stack.empty())
                        modelview_mtx_stack.top() = mtx;
                    else
                        modelview_mtx_stack.push(mtx);
                }
            }
            else if(G_MTX_PROJECTION){
                projection_mtx = mtx;
            }
            if(!modelview_mtx_stack.empty())
                modelview_projection_mtx = projection_mtx * modelview_mtx_stack.top();
            
            break;
        }
        case G_MOVEWORD: // G_MOVEWORD
        {
            uint8_t index = ((instr >> 48) & 0xFF);
            switch (index)
            {
            case G_MW_SEGMENT:
            {
                uint8_t segment_id = ((instr >> 32) & 0xFFFF) >> 2;
                segments[segment_id] = instr & 0xFFFFFFFF;
                break;
            }
            default:
                break;
            }
            break;
        }
        case 0xDC: // G_MOVEMEM
            break;
        case 0xDD: // G_LOAD_UCODE
            break;
        case G_DL: // G_DL
            if(((instr >> 48) & 0xFF) == 0) address_stack.push(instr_ptr);
            instr_ptr = translate_address(instr);
            continue;
            break;
        case G_ENDDL: // G_ENDDL
            if(address_stack.empty())
                return;

            instr_ptr = address_stack.top();
            address_stack.pop();
            break;
        case 0xE0: // G_SPNOP
            break;
        case 0xE1: // G_RDPHALF_1
            break;
        case 0xE2: // G_SETOTHERMODE_L
            break;
        case 0xE3: // G_SETOTHERMODE_H
            break;
        case 0xE4: // G_TEXRECT
            break;
        case 0xE5: // G_TEXRECTFLIP
            break;
        case 0xE6: // G_RDPLOADSYNC
            break;
        case 0xE7: // G_RDPPIPESYNC
            break;
        case 0xE8: // G_RDPTILESYNC
            break;
        case 0xE9: // G_RDPFULLSYNC
            break;
        case 0xEA: // G_SETKEYGB
            break;
        case 0xEB: // G_SETKEYR
            break;
        case 0xEC: // G_SETCONVERT
            break;
        case 0xED: // G_SETSCISSOR
            break;
        case 0xEE: // G_SETPRIMDEPTH
            break;
        case 0xEF: // G_RDPSETOTHERMODE
            break;
        case 0xF0: // G_LOADTLUT
            break;
        case 0xF1: // G_RDPHALF_2
            break;
        case 0xF2: // G_SETTILESIZE
            break;
        case 0xF3: // G_LOADBLOCK
            break;
        case 0xF4: // G_LOADTILE
            break;
        case 0xF5: // G_SETTILE
            break;
        case 0xF6: // G_FILLRECT
            break;
        case 0xF7: // G_SETFILLCOLOR
            break;
        case 0xF8: // G_SETFOGCOLOR
            break;
        case 0xF9: // G_SETBLENDCOLOR
            break;
        case 0xFA: // G_SETPRIMCOLOR
            break;
        case 0xFB: // G_SETENVCOLOR
            break;
        case 0xFC: // G_SETCOMBINE
            break;
        case 0xFD: // G_SETTIMG
            break;
        case 0xFE: // G_SETZIMG
            break;
        case 0xFF: // G_SETCIMG
            break;
        default:
            break;
        }
    instr_ptr += 8;
    }
};

void RSP::finish_task()
{
    //also set PC in the future
    task_in_progress = false;
    regs.SP_STATUS |= 0x0203;

    OSTask new_task = OSTask::parse_from_mem(dmem.mem,0xFC0);

    if(regs.SP_STATUS & 0x40)
        rcp.mi.route_interrupt(InterruptSource::SP);
    if(current_task_type == RSPTaskType::GFXTASK){
        process_gfx_task(new_task);
        rcp.rsp.gfx.render_cycle();
        rcp.mi.route_interrupt(InterruptSource::DP);
    }
}

OSTask OSTask::parse_from_mem(std::vector<uint8_t> mem, uint32_t addr)
{
    OSTask new_task;
    for(size_t i = 0; i < sizeof(OSTask); i++)
    {
        reinterpret_cast<uint8_t*>(&new_task)[i] =
            mem[(addr + i) ^ 3];
    }

    return new_task;
}
