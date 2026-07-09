#include "VR4300Cycle.h"
#include "Operations.h"
#include <iostream>
#include <cstring>
#include <iomanip>
#include<inttypes.h>
#include"RCP.h"
#include <limits>
#include"CP0.h"

VR4300Cycle::VR4300Cycle() : cp0(), rcp(rcp), fpu(cp0){
    discard_bd = true;
    WB_in.op = op_storage;
    DC_in.op = op_storage + 1;
    EX_in.op = op_storage + 2;
    RF_in.op = op_storage + 3;
}

// todo
// fix asid
// add exceptions.
//  overflow exception in dc
//  interrupts and their exceptions
//  all exception caused by operations will be added with them
//  fpu exceptions will be added with it
//  reset exceptions
// fpu
void VR4300Cycle::on_pclock()
{
    if(cp0.count == cp0.compare )//>= cp0.compare)
        cp0.cause = cp0.set_bits(cp0.cause, CAUSE_IP_TIMER_MASK, 1 << CAUSE_IP_TIMER_SHIFT);
    cp0.count = (uint32_t)(cp0.count + 1);
    
    //on interlock the ENTIRE pipeline is stalled
    if(stall){
        stall--;
        return;
    }
    
    if (WB() || DC() || EX() || RF() || IC()) return; 
    submit_pipeline();

}

//Pipeline writeback stage
inline bool VR4300Cycle::WB()
{
    if(stall_depth > 0)return false;

    auto& in = WB_in;
    
    //what WB does is:
    // write back. just as the name suggests really


    //if (in.op->flags & CAUSED_EXCEPTION && DC_in.op->flags & READS_CP0 && !in.CP0I_triggered)
    //{
    //    //CP0I
    //    //stall = 1;
    //    stall = 0;
    //    stall_depth = 0;
    //    in.CP0I_triggered = true; // This won't work as of right now since WB gets deleted when stalling
    //    return true;
    //}

    if(in.op->tmplt->is_store){
        if(in.op->tmplt->left_access || in.op->tmplt->right_access) in.op->data_addr_p = in.op->data_addr_p & ~(in.op->tmplt->access_size - 1);
        if(in.op->cacheable){
            if((in.op->tmplt->atomic) && !LLBit){
                GPR[in.op->dest_reg] = 0;
                return false;
            }else if((in.op->tmplt->atomic) && LLBit) GPR[in.op->dest_reg] = 1;
            uint8_t offset = in.op->data_addr_p & 0xF;
            Dcache_line &line = Dcache[in.op->dcache_index];
            dcache_write_size(line, offset, in.op->result, in.op->tmplt->access_size);
        }else{
            rcp->write_size(in.op->data_addr_p, in.op->result, in.op->tmplt->access_size);
        }
    }
    if(in.op->tmplt->writes_reg){
        if(in.op->tmplt->writes_cp){
            if(in.op->tmplt->CPz == 0){
                cp0.regs[in.op->dest_reg] = (cp0.regs[in.op->dest_reg] & ~cp0.write_masks[in.op->dest_reg]) | (in.op->result & cp0.write_masks[in.op->dest_reg]);
                if(in.op->dest_reg == 11)
                    cp0.cause = cp0.set_bits(cp0.cause, CAUSE_IP_TIMER_MASK, 0 << CAUSE_IP_TIMER_SHIFT);
                if(in.op->dest_reg == 6)
                    cp0.random = (cp0.wired > 31)?63:31;
                if(in.op->dest_reg == 12)
                    cp0.stash_status();
            }
            if(in.op->tmplt->CPz== 1 && (in.op->tmplt->cp_control))
                fpu.set_control(in.op->rd, in.op->result);
            else if(in.op->tmplt->CPz == 1) 
                fpu.write_fpr(in.op->dest_reg,in.op->result,in.op->tmplt->access_size);
        }else
        if(in.op->dest_reg != 0)
            GPR[in.op->dest_reg] = in.op->result;
    }
    if(in.op->tmplt->writes_lo) LO = in.op->result_LO;
    if(in.op->tmplt->writes_hi) HI = in.op->result_HI;
    if(in.op->tmplt->instruction_type == OpType::TLBR){
        cp0.entryHi = in.op->result_entryHI;
        cp0.entryLo0 = in.op->result_entryLO0;
        cp0.entryLo1 = in.op->result_entryLO1;
        cp0.pageMask = in.op->result_pagemask;
    }

    if(in.op->tmplt->instruction_type == OpType::TLBWI || in.op->tmplt->instruction_type == OpType::TLBWR){
        uint8_t tlb_index;
        if(in.op->tmplt->instruction_type == OpType::TLBWI) tlb_index = cp0.index;
        else tlb_index = cp0.random;
        cp0.TLB[tlb_index][0] = in.op->result_pagemask;
        cp0.TLB[tlb_index][1] = in.op->result_entryHI;
        cp0.TLB[tlb_index][2] = in.op->result_entryLO0;
        cp0.TLB[tlb_index][3] = in.op->result_entryLO1;
    }
    
    return false;
}



//Pipeline data cache stage
inline bool VR4300Cycle::DC()
{
    if(stall_depth > 1)return false;
    auto& in  = DC_in;


    //what DC does is:
    // gets the segment operated on
    // tlb ranslates the addr of the data to be written to/from
    // if the data is read from the cache it fetches it
    //a lot of the code below is bloated by exception and interlock handling

    //for now disabled
    //if(DC_in.op->fire_fpu_exception){
    //    DC_in.op->fire_fpu_exception = false;
    //    EX_in.op->fire_fpu_exception = false;
    //    uint8_t Cause = ((fpu.FCR31 >> 12) & 0x3F);
    //    uint8_t Enables = ((fpu.FCR31 >> 7) & 0x1F);
    //    //wooo magic numbers
    //    fpu.FCR31 |= (Cause & 0x1F & ~Enables) << 2;
    //    if( ((Enables | 0x20) & Cause) != 0){
    //        cp0.cause = cp0.set_bits(cp0.cause,0x3 << 28,0 << 28); // idk if this should be set only during fpu exceptions or all of them?
    //        handle_general_exception(*DC_in.op,FPE);
    //        return true;
    //    }
    //}

    if(update_conditional){
        fpu.FCR31 = (fpu.FCR31 & ~(1<<23)) | (in.op->conditional_val << 23);
        fpu.COC = in.op->conditional_val;
        update_conditional = false;
    }

    uint8_t IP = cp0.get_bits(cp0.cause,CAUSE_IP_MASK,CAUSE_IP_SHIFT);
    if((IP & cp0.IM) && cp0.IE && !cp0.EXL && !cp0.ERL)[[unlikely]]{
        if(in.op->tmplt->instruction_type == OpType::ERET) handle_general_exception(*RF_in.op,Int);
        else handle_general_exception(*in.op,Int);

        //WB();
        //WB_in = {};
        return true;
    }
    
    if(in.op->tmplt->is_trap && in.op->result)[[unlikely]]{
        handle_general_exception(*in.op, Tr);
        return true;
    }
    
    if(!(in.op->tmplt->is_store) && !in.op->tmplt->is_load && !(in.op->tmplt->instruction_type == OpType::CACHE)){
        return false;
    }
    
    const CP0::Segment& segment = cp0.get_segment(in.op->data_addr);
    in.op->cacheable = segment.cacheable;
    
    //DADE
    bool misalligned = ((in.op->tmplt->access_size == 8) && in.op->data_addr % 8 != 0) ||
    ((in.op->tmplt->access_size == 4) && in.op->data_addr % 4 != 0) ||
    ((in.op->tmplt->access_size == 2) && in.op->data_addr % 2 != 0);
    bool wrong_mode = ((cp0.mode == Mode::USER) && !segment.user_accesible) ||
    ((cp0.mode == Mode::SUPERVISOR) && !segment.supervisor_accesible) ||
    ((cp0.mode == Mode::KERNEL) && !segment.kernel_accesible);
    bool sided = ( DC_in.op->tmplt->right_access ||DC_in.op->tmplt->left_access );
    if( !sided && (misalligned || wrong_mode))[[unlikely]]{
        cp0.badVAddr = in.op->data_addr;
        if(in.op->tmplt->is_store)handle_general_exception(*in.op,AdES);
        else handle_general_exception(*in.op,AdEL);
        return true;
    }

    if(segment.tlb_mapped)[[unlikely]]{
        CP0::TLB_Result tlb_result = cp0.tlb_translate(in.op->data_addr);
        if(tlb_result.miss){
            //tlb miss exception
            if(in.op->tmplt->is_store)
                handle_tlb_miss_exception(in.op->data_addr, *in.op, TLBS);
            else
                handle_tlb_miss_exception(in.op->data_addr, *in.op, TLBL);
            return true;
            
        }
        if(!tlb_result.valid){
            set_tlb_context(in.op->data_addr);
            if(in.op->tmplt->is_store)
                handle_general_exception(*in.op, TLBS);
            else
                handle_general_exception(*in.op, TLBL);
            return true;
        }
        if(!tlb_result.dirty && in.op->tmplt->is_store){
            set_tlb_context(in.op->data_addr);
            handle_general_exception(*in.op, Mod);
            return true;
        }
        
        in.op->data_addr_p = tlb_result.p_addr;
        in.op->cacheable = tlb_result.cache != 2;
    }else 
    in.op->data_addr_p = in.op->data_addr - segment.translation_offset;
    in.op->dcache_index = (in.op->data_addr & 0x1FF0) >> 4;
    
    
    if((cp0.watchLo & WATCHLO_R_MASK) && ((in.op->data_addr_p >> 3) == (cp0.watchLo>>3) && in.op->tmplt->is_load))[[unlikely]]{
        handle_general_exception(*in.op,WATCH);
        return true;
    }
    if((cp0.watchLo & WATCHLO_W_MASK) && ((in.op->data_addr_p >> 3) == (cp0.watchLo>>3) && in.op->tmplt->is_store))[[unlikely]]{
        handle_general_exception(*in.op,WATCH);
        return true;
    }
    
    if(in.op->cacheable)[[likely]]{
        Dcache_line& line = Dcache[in.op->dcache_index];
        bool cache_hit = ((in.op->data_addr_p >> 12) == line.tag);
        
        //this interlock logic is messy in general and probably needs to be cleaned up for accuracy, but it works for now
        if(WB_in.op->tmplt->is_store /*&& cache_hit*/){// this will cause the interlock to happen too often, but it fixes a bug with two stores following each other 
            //DCB on hit
            if(!in.DCB_triggered){
                stall = 1;
                stall_depth = 1;
                in.DCB_triggered = true;
                return true;
            }
        }

        if(in.op->tmplt->instruction_type == OpType::CACHE){
            if(!in.COp_triggered){
                stall = handle_cache_op(*in.op); // it might be smart to move this to after the stall
                stall_depth = 1;
                in.COp_triggered = true;
                return true;
            }
            return false;
        }

        if((!cache_hit || !line.valid)  && !in.DCB_triggered){
            //DCM + DCB
            stall = 1 + DCACHE_STALL_TIME;
            stall_depth = 1;
            in.DCB_triggered = true;
            return true;
        }else if((!cache_hit || !line.valid)  && in.DCB_triggered){

            if(line.dirty && line.valid){
                //write back previous entry
                dcache_write_back(line, in.op->dcache_index);
            }
            //update dcache
            uint64_t line_start_addr = in.op->data_addr_p & ~0xF;
            for (int i = 0; i < 16; i++) line.data[i] = rcp->read_size(line_start_addr + i, 1);
            line.tag = in.op->data_addr_p >> 12;
            line.valid = true;
            line.dirty = false;
        }

        if(in.op->tmplt->is_load){
            if(in.op->tmplt->atomic) LLBit = 1;
            //fetch data from the cache to put in a reg
            uint32_t offset_into_line = in.op->data_addr_p & 0xF;
            uint64_t mem = dcache_read_size(line,(offset_into_line & ~(in.op->tmplt->access_size - 1)),in.op->tmplt->access_size );

            uint8_t byte_offset = in.op->data_addr_p & (in.op->tmplt->access_size - 1);
            uint8_t bit_offset = byte_offset * 8;
            uint8_t bits = in.op->tmplt->access_size * 8;
            bool sign_extended = in.op->tmplt->sign_extended;

            if (in.op->tmplt->left_access){
                uint64_t mask = ~0ULL << bit_offset;
                in.op->result = (in.op->rt_val & ~mask) | ((mem << bit_offset) & mask);
                in.op->result = (sign_extended) ?(int32_t)in.op->result:in.op->result; //there are only two options, LW and LD
            }
            else if (in.op->tmplt->right_access){
                uint64_t mask = ~0ULL >> (bits - bit_offset - 8);
                if(in.op->tmplt->access_size == 4) mask >>= 32;
                in.op->result = (in.op->rt_val & ~mask) | ((mem >> (bits - bit_offset - 8)) & mask);
                in.op->result = (sign_extended) ?(int32_t)in.op->result:in.op->result; //there are only two options, LW and LD
            }else {
                
                in.op->result = dcache_read_size(line,offset_into_line,in.op->tmplt->access_size);
                if(in.op->tmplt->access_size == 1) in.op->result = (sign_extended) ? (int64_t)(int8_t)in.op->result:(uint64_t)(uint8_t)in.op->result;
                else if(in.op->tmplt->access_size == 2) in.op->result = (sign_extended) ? (int64_t)(int16_t)in.op->result:(uint64_t)(uint16_t)in.op->result;
                else if(in.op->tmplt->access_size == 4) in.op->result = (sign_extended) ? (int64_t)(int32_t)in.op->result:(uint64_t)(uint32_t)in.op->result;
                else if(in.op->tmplt->access_size == 8) in.op->result = (sign_extended) ? in.op->result:in.op->result;
            }
        }else if(in.op->tmplt->is_store){
            uint32_t offset_into_line = in.op->data_addr_p & 0xF;
            uint64_t og_val = dcache_read_size(line, offset_into_line & ~(in.op->tmplt->access_size - 1), in.op->tmplt->access_size);
            uint8_t byte_offset = in.op->data_addr_p & (in.op->tmplt->access_size - 1);
            uint8_t bit_offset = byte_offset * 8;
            uint8_t bits = in.op->tmplt->access_size * 8;
            if(in.op->tmplt->left_access){
                uint64_t mask = ~0xFFULL << (bits - bit_offset - 8);
                in.op->result = (og_val & mask) | (in.op->result & ~mask);
            }else if(in.op->tmplt->right_access){
                uint64_t mask = ~0ULL << (bits - bit_offset - 8);
                in.op->result = (og_val & ~mask) | (in.op->result & mask);
            }
            line.dirty = 1;
        }
    }else{//if not cacheable
        if( in.op->tmplt->is_load || in.op->tmplt->is_store){
            if(!in.uncacheable_stall_triggered){
                stall = DCACHE_STALL_TIME;
                stall_depth = 1;
                in.uncacheable_stall_triggered = 1;
                return true;
            } 
            if(WB_in.op->tmplt->is_store && in.op->data_addr_p == WB_in.op->data_addr_p && !WB_in.op->cacheable){
                //WB();
                //WB_in = {};
            }
            if(in.op->tmplt->is_load){

                //this is the most consise i could get it...
                uint64_t mem = rcp->read_size(in.op->data_addr_p & ~(in.op->tmplt->access_size - 1), in.op->tmplt->access_size);

                uint8_t byte_offset = in.op->data_addr_p & (in.op->tmplt->access_size - 1);
                uint8_t bit_offset = byte_offset * 8;
                uint8_t bits = in.op->tmplt->access_size * 8;
                bool sign_extended = in.op->tmplt->sign_extended;

                if (in.op->tmplt->left_access){
                    uint64_t mask = ~0ULL << bit_offset;
                    in.op->result = (in.op->rt_val & ~mask) | ((mem << bit_offset) & mask);
                    in.op->result = (sign_extended) ?(int32_t)in.op->result:in.op->result; //there are only two options, LW and LD
                }
                else if (in.op->tmplt->right_access){
                    uint64_t mask = ~0ULL >> (bits - bit_offset - 8);
                    if(in.op->tmplt->access_size == 4) mask >>= 32;
                    in.op->result = (in.op->rt_val & ~mask) | ((mem >> (bits - bit_offset - 8)) & mask);
                    in.op->result = (sign_extended) ?(int32_t)in.op->result:in.op->result; //there are only two options, LW and LD
                }else {
                    
                    in.op->result = rcp->read_size(in.op->data_addr_p, in.op->tmplt->access_size);
                    if(in.op->tmplt->access_size == 1) in.op->result = (sign_extended) ? (int64_t)(int8_t)in.op->result:(uint64_t)(uint8_t)in.op->result;
                    else if(in.op->tmplt->access_size == 2) in.op->result = (sign_extended) ? (int64_t)(int16_t)in.op->result:(uint64_t)(uint16_t)in.op->result;
                    else if(in.op->tmplt->access_size == 4) in.op->result = (sign_extended) ? (int64_t)(int32_t)in.op->result:(uint64_t)(uint32_t)in.op->result;
                    else if(in.op->tmplt->access_size == 8) in.op->result = (sign_extended) ? in.op->result:in.op->result;
                }
            }
            if(in.op->tmplt->is_store){
                uint64_t og_val;
                og_val = rcp->read_size(in.op->data_addr_p & ~(in.op->tmplt->access_size - 1), in.op->tmplt->access_size);
                uint8_t byte_offset = in.op->data_addr_p & (in.op->tmplt->access_size - 1);
                uint8_t bit_offset = byte_offset * 8;
                uint8_t bits = in.op->tmplt->access_size * 8;
                if(in.op->tmplt->left_access){
                    uint64_t mask = ~0xFFULL << (bits - bit_offset - 8);
                    in.op->result = (og_val & mask) | (in.op->result & ~mask);
                }else if(in.op->tmplt->right_access){
                    uint64_t mask = ~0ULL << (bits - bit_offset - 8);
                    in.op->result = (og_val & ~mask) | (in.op->result & mask);
                }
            }
        }
    }

    return false;
}

//Pipeline execute stage
inline bool VR4300Cycle::EX()
{
    if(stall_depth > 2)return false;
    auto& in  = EX_in;
    auto& dc  = DC_in;
    //what EX does is:
    // 
    // does the operation


    uint8_t CU = (cp0.status >> 28) & 0xF;

    if((in.op->tmplt->is_cpz) && !((CU >> in.op->tmplt->CPz) & 1))[[unlikely]]{
        if(!(in.op->tmplt->CPz == 0 && (cp0.mode == Mode::KERNEL))){
            cp0.cause = cp0.set_bits(cp0.cause,0x3 << 28,in.op->tmplt->CPz << 28);
            handle_general_exception(*in.op,CpU);
            return true;
        }
    }

    if(!in.LDI_triggered && (dc.op->tmplt->is_load)){
        if(!(dc.op->tmplt->writes_cp) && dc.op->dest_reg != 0){
            if(in.op->rt == dc.op->dest_reg || in.op->rs == dc.op->dest_reg){
                stall = 1;
                stall_depth = 2;
                in.LDI_triggered = true;
                return true;
            }
        }
        if(dc.op->tmplt->writes_cp && in.op->tmplt->reads_cp){
            if(in.op->rt == dc.op->dest_reg || in.op->rd == dc.op->dest_reg){
                stall = 1;
                stall_depth = 2;
                in.LDI_triggered = true;
                return true;
            }
        }
    }else if(in.LDI_triggered && (dc.op->tmplt->is_load)){
            // this might not be perfect forwaring logic
        forward_write(*dc.op, *in.op);
    }

    if(in.op->tmplt->multicycle && !in.MCI_triggered){
        //MCI
        stall = in.op->tmplt->multicycle;
        stall_depth = 2;
        in.MCI_triggered = true;
        return true;
    }

    if(in.op->tmplt->instruction_type == OpType::SYSCALL)[[unlikely]]{
        handle_general_exception(*in.op, Sys);
        return true;
    }
//
    if(in.op->tmplt->instruction_type == OpType::BREAK)[[unlikely]]{
        handle_general_exception(*in.op, Bp);
        return true;
    }

    //this isn't perfect but i actually don't understand how sc would know during dc whether it succeded or not
    //even assuming it knows, with how the pipeline works here implementing that would be hell
    if((DC_in.op->tmplt->atomic) && (DC_in.op->tmplt->is_store) && (in.op->rs == DC_in.op->dest_reg)){
        in.op->rs_val = LLBit;
    }
    if((DC_in.op->tmplt->atomic) && (DC_in.op->tmplt->is_store) && (in.op->rt == DC_in.op->dest_reg)){
        in.op->rt_val = LLBit;
    }

    in.op->tmplt->execute(*this);
    return false;
}

//Pipeline register fetch stage
inline bool VR4300Cycle::RF()
{
    //what RF does is:
    // gets data from earlier stages if needed registers were operated on
    // get physical PC from TLB
    // get instruction from instruction cache
    // decode op_code
    // get values of registers to be messed with
    // increment PC
    //a lot of code below is bloated by exceptions and interlocks
    
    //add microtlb miss in the future

    //this 100% could be done smarter

    //replace this with removing the op in whatever causes bd
    if(discard_bd){
        discard_bd = false;
        RF_in.op->tmplt = &noptmplt;
        return false;
    }

    if(next_op_bd){
        RF_in.op->bd = true;
        next_op_bd = false;
    }

    uint32_t PC_p;
    const CP0::Segment& segment = cp0.get_segment(RF_in.op->PC);
    bool cacheable = segment.cacheable;

    //IADE
    if(
        // wrong mode (user, kernel, supervisor)
        ((cp0.mode == Mode::USER) && !segment.user_accesible) ||
        ((cp0.mode == Mode::SUPERVISOR) && !segment.supervisor_accesible) ||
        ((cp0.mode == Mode::KERNEL) && !segment.kernel_accesible)
    )[[unlikely]]{
        cp0.badVAddr = RF_in.op->PC;
        handle_general_exception(*RF_in.op,AdEL);
        return true;
    }

    if(segment.tlb_mapped)[[unlikely]]{
        CP0::TLB_Result tlb_result = cp0.tlb_translate(RF_in.op->PC);
        if(tlb_result.miss){
            //tlb miss exception
            handle_tlb_miss_exception(RF_in.op->PC, *RF_in.op, TLBL);
            return true;
        }
        if(!tlb_result.valid){
            set_tlb_context(RF_in.op->PC);
            handle_general_exception(*RF_in.op, TLBL);
            return true;
        }
        PC_p = tlb_result.p_addr;
        cacheable = tlb_result.cache != 2;
    }else 
    PC_p = RF_in.op->PC - segment.translation_offset;

    uint32_t op_code;
    if(cacheable){
        Icache_line& line = Icache[RF_in.op->icache_index];
        bool hit = ((PC_p >> 12) == line.tag) && line.valid;
        
        if(!hit && !RF_in.ICB_triggered){
            //update icache
            uint64_t line_start_addr = PC_p & ~ 0x1F;
            for (int i = 0; i < 8; i++)
            {
                decode_op(rcp->read_size(line_start_addr + i * 4, 4),line.data[i] );
            }
            line.tag = PC_p >> 12;
            line.valid = true;
            
            stall = ICACHE_STALL_TIME;
            stall_depth = 3;
            RF_in.ICB_triggered = true;
            return true; 
            
        }
        uint8_t offset = (PC_p >> 2) & 0x7;

        RF_in.op->tmplt = line.data[offset].tmplt;

        RF_in.op->rs = line.data[offset].rs;
        RF_in.op->rt = line.data[offset].rt;
        RF_in.op->rd = line.data[offset].rd;
        RF_in.op->sa = line.data[offset].sa;
        RF_in.op->dest_reg = line.data[offset].dest_options[line.data[offset].tmplt->dest_id];
        RF_in.op->opcode = line.data[offset].opcode;

        fetch_regs();

        if (RF_in.op->tmplt->causes_bd)
            next_op_bd = true;

        return false;

        //return false;
    }else{//this logic of avoiding cache is a little scuffed, might need some review
        if(!RF_in.uncacheable_stall_triggered){
            //not cacheable so read diractly from memory
            stall = DCACHE_STALL_TIME; //probably takes as long as refilling dcache
            stall_depth = 3;
            RF_in.uncacheable_stall_triggered = 1;
            return true;
        }
        op_code = rcp->read_size(PC_p, 4);
    }

    //if(decode_op(op_code))
    //return true;
    decode_op(op_code,*RF_in.op);

    fetch_regs();

    if (RF_in.op->tmplt->causes_bd)
        next_op_bd = true;

    return false;
}

//Pipeline instruction cache stage
inline bool VR4300Cycle::IC()
{
    //what IC does is:
    // address icache and microtlb
    //it's comical how little it does

    return false;
}


inline bool VR4300Cycle::decode_op(uint32_t word, Operation& op)
{
    uint8_t opcode = word >> 26;
    
    const OperationTemplate* tmplt;
    
    if(opcode == 0)
    tmplt = &special_op_lut[word & 0x3F];
    else if(opcode == 1)
    tmplt = &regimm_op_lut[(word >> 16) & 0x1F];
    else if((opcode >> 2) == 4){
        if(((word >> 21) & 0x1F) == 8){
            if(((word >> 26) & 0x3) == 0)
                tmplt = &COP0rt_op_lut[(word >> 16) & 0x1F];
            else if(((word >> 26) & 0x3) == 1)
                tmplt = &COP1rt_op_lut[(word >> 16) & 0x1F];
            else
                tmplt = &COP2rt_op_lut[(word >> 16) & 0x1F];
        }
        else if((word >> 25) == 33)
        tmplt = &CP0_op_lut[word & 0x3F];
        else if((word >> 25) == 35)
        tmplt = &CP1_op_lut[word & 0x3F];
        else{
            if(((word >> 26) & 0x3) == 0)
                tmplt = &COP0rs_op_lut[(word >> 21) & 0x1F];
            else if(((word >> 26) & 0x3) == 1)
                tmplt = &COP1rs_op_lut[(word >> 21) & 0x1F];
            else
                tmplt = &COP2rs_op_lut[(word >> 21) & 0x1F];
        }
    }
    else
    tmplt = &primary_op_lut[opcode];
    
    
    //if (!tmplt->execute)[[unlikely]]{
    //    // invalid instruction exception (RI)
    //    handle_general_exception(op,RI);
    //    return true;
    //}
    
    op.tmplt = tmplt;

    op.rs = (word >> 21) & 0x1F;
    op.rt = (word >> 16) & 0x1F;
    op.rd = (word >> 11) & 0x1F;
    op.sa = (word >> 6) & 0x1F;
    op.dest_reg = op.dest_options[tmplt->dest_id];
    op.opcode = word;
    return false;
    
}

inline void VR4300Cycle::submit_pipeline(){

    //std::cout<<*WB_in.op << "\n";

    if(cp0.random == cp0.wired && cp0.wired < 32)
        cp0.random = (cp0.wired > 31)?63:31;
    else{
        cp0.random--;
        if(!cp0.random) cp0.random = (cp0.wired > 31)?63:31;
    }

    stall_depth = 0;

    WB_in.CP0I_triggered = false;
    DC_in.COp_triggered = false;
    DC_in.DCB_triggered = false;
    DC_in.uncacheable_stall_triggered = false;
    EX_in.LDI_triggered = false;
    EX_in.MCI_triggered = false;
    RF_in.ICB_triggered = false;
    RF_in.uncacheable_stall_triggered = false;
    
    Operation* temp = WB_in.op;
    WB_in.op = DC_in.op;
    DC_in.op = EX_in.op;
    EX_in.op = RF_in.op;
    RF_in.op = temp;
    RF_in.op->tmplt = &noptmplt;
    RF_in.op->bd = false;

    RF_in.op->icache_index = (PC >> 5) & 0x1FF;
    RF_in.op->PC = PC;
    PC += 4;
}

void VR4300Cycle::abort_pipeline() {
    
    WB_in.CP0I_triggered = false;
    DC_in.COp_triggered = false;
    DC_in.DCB_triggered = false;
    DC_in.uncacheable_stall_triggered = false;
    EX_in.LDI_triggered = false;
    EX_in.MCI_triggered = false;
    RF_in.ICB_triggered = false;
    RF_in.uncacheable_stall_triggered = false;

    *RF_in.op = {};
    *EX_in.op = {};
    *DC_in.op = {};
    *WB_in.op = {};
    
    stall = 0; // maybe
    stall_depth = 0;
    next_op_bd = false;
    discard_bd = true;
}

void VR4300Cycle::handle_tlb_miss_exception(uint64_t addr, const Operation& op, ExceptionCode cause){
    bool op_bd = op.bd;
    uint64_t op_PC = op.PC;
    abort_pipeline();
    //this is literally just the flow chart from page 203 copied
    cp0.cause = cp0.set_bits(cp0.cause,CAUSE_EXCCODE_MASK,cause<<CAUSE_EXCCODE_SHIFT);
    set_tlb_context(addr);
    uint16_t jump_offset;
    
    if(!cp0.EXL){
        if(op_bd){
            cp0.EPC = op_PC - 4;
            cp0.cause = cp0.set_bits(cp0.cause, CAUSE_BD_MASK, 1 << CAUSE_BD_SHIFT);
        } else{
            cp0.cause = cp0.set_bits(cp0.cause, CAUSE_BD_MASK, 0 << CAUSE_BD_SHIFT);
            cp0.EPC = op_PC;
        }
        if(cp0.is_xmode()) jump_offset=0x0080;
        else jump_offset=0x0000;
    }else{
        jump_offset=0x180; // says 80 in flow chart but 180 in description. 180 makes more sense prolly
    }
    cp0.status = cp0.set_bits(cp0.status, STATUS_EXL_MASK, 1 << STATUS_EXL_SHIFT);
    cp0.stash_status();
    uint64_t jump_base = (cp0.status & STATUS_BEV_MASK)? BOOTSTRAP_EXCEPTION_VEC_64 : EXCEPTION_VEC_64;
    PC = jump_base + jump_offset;
}

//outside of this:
//set ce when coprocessor
//set fp status registers
//set tlb related registers 
//set badvaddr
void VR4300Cycle::handle_general_exception(const Operation& op, ExceptionCode cause){
    bool op_bd = op.bd;
    uint64_t op_PC = op.PC;
    abort_pipeline();
    cp0.cause = cp0.set_bits(cp0.cause,CAUSE_EXCCODE_MASK,cause<<CAUSE_EXCCODE_SHIFT);
    
    if(!cp0.EXL){
        if(op_bd){
            cp0.EPC = op_PC - 4;
            cp0.cause = cp0.set_bits(cp0.cause, CAUSE_BD_MASK, 1 << CAUSE_BD_SHIFT);
        } else{
            cp0.cause = cp0.set_bits(cp0.cause, CAUSE_BD_MASK, 0 << CAUSE_BD_SHIFT);
            cp0.EPC = op_PC;
        }
    }
    cp0.status = cp0.set_bits(cp0.status, STATUS_EXL_MASK, 1 << STATUS_EXL_SHIFT);
    cp0.stash_status();
    uint64_t jump_base = (cp0.status & STATUS_BEV_MASK)? BOOTSTRAP_EXCEPTION_VEC_64 : EXCEPTION_VEC_64;
    PC = jump_base + 0x0180;
}

void VR4300Cycle::update_hardware_interrupt(uint8_t enable, uint8_t value){
    value &= 0x1F;
    value <<= 2;
    enable &= 0x1F;
    enable <<= 2;
    uint8_t prev_inter = cp0.get_bits(cp0.cause,CAUSE_IP_MASK,CAUSE_IP_SHIFT);
    uint8_t new_inter = (prev_inter & ~enable) | (enable & value);
    cp0.cause = cp0.set_bits(cp0.cause, CAUSE_IP_MASK, new_inter << CAUSE_IP_SHIFT);
}

inline void VR4300Cycle::set_tlb_context(uint64_t addr){
    cp0.badVAddr = addr;
    cp0.context  = cp0.set_bits(cp0.context, CONTEXT_BADVPN2_MASK, (addr >> 13) << CONTEXT_BADVPN2_SHIFT);
    cp0.xcontext = cp0.set_bits(cp0.xcontext, XCONTEXT_BADVPN2_MASK, (addr >> 13) << XCONTEXT_BADVPN2_SHIFT);
    cp0.entryHi  = cp0.set_bits(cp0.entryHi, ENTRYHI_VPN2_MASK, (addr >> 13) << ENTRYHI_VPN2_SHIFT);
}

void VR4300Cycle::dcache_write_size(VR4300Cycle::Dcache_line &line, uint8_t offset, uint64_t value, uint8_t size)
{
    for (int i = 0; i < size; i++)
    {
        *(line.data + offset + i) = (uint8_t)(value >> ((size - 1 - i) * 8));
    }
}

uint64_t VR4300Cycle::dcache_read_size(const VR4300Cycle::Dcache_line &line, uint8_t offset, uint8_t size)
{
    uint64_t result = 0;
    for (int i = 0; i < size; i++)
    {
        result |= (uint64_t)*(line.data + offset + i) << ((size - 1 - i) * 8);
    }
    return result;
}

uint8_t VR4300Cycle::handle_cache_op(const VR4300Cycle::Operation& op){
    uint8_t sub_op_code = op.rt >> 2;
    uint8_t accessed_cache = op.rt & 0x3;
    uint32_t icalculated_id = (op.data_addr_p & 0x3FE0) >> 5;
    uint32_t icalculated_tag = op.data_addr_p >> 12;
    Icache_line& i_line = Icache[icalculated_id];
    uint32_t dcalculated_id = (op.data_addr_p & 0x1FF0) >> 4;
    uint32_t dcalculated_tag = op.data_addr_p >> 12;
    Dcache_line& d_line = Dcache[dcalculated_id];
    bool d_hit = d_line.tag == dcalculated_tag;
    bool i_hit = i_line.tag == icalculated_tag;
    switch (sub_op_code)
    {
        case 0:
        if (accessed_cache == 0) i_line.valid = 0;
        else{
            //Index_Write_Back_Invalidate
            if(d_line.valid){
                dcache_write_back(d_line,dcalculated_id);
            }
            d_line.valid = 0;
        }
        break;
        case 1:
        //Index_Load_Tag
        if(accessed_cache == 0){
            cp0.tagLo = cp0.set_bits(cp0.tagLo, TAGLO_PTAGLO_MASK, i_line.tag << TAGLO_PTAGLO_SHIFT);
            cp0.tagLo = cp0.set_bits(cp0.tagLo, TAGLO_PSTATE_MASK, i_line.valid << (TAGLO_PTAGLO_SHIFT + 1));
        }
        else{
            cp0.tagLo = cp0.set_bits(cp0.tagLo, TAGLO_PTAGLO_MASK, d_line.tag << TAGLO_PTAGLO_SHIFT);
            cp0.tagLo = cp0.set_bits(cp0.tagLo, TAGLO_PSTATE_MASK, (d_line.valid?3:0) << TAGLO_PSTATE_SHIFT);
        }
        break;
        case 2:
        //Index_Store_Tag
        if(accessed_cache == 0){
            i_line.tag = cp0.get_bits(cp0.tagLo, TAGLO_PTAGLO_MASK, TAGLO_PTAGLO_SHIFT);
            i_line.valid = (cp0.get_bits(cp0.tagLo, TAGLO_PSTATE_MASK, TAGLO_PSTATE_SHIFT) == 2)?1:0;
        }
        else{
            d_line.tag = cp0.get_bits(cp0.tagLo, TAGLO_PTAGLO_MASK, TAGLO_PTAGLO_SHIFT);
            d_line.valid = (cp0.get_bits(cp0.tagLo, TAGLO_PSTATE_MASK, TAGLO_PSTATE_SHIFT) == 3)?1:0;
        }
        break;
        case 3:
        //Create_Dirty_Exclusive
        if(accessed_cache == 1){
            if(d_line.dirty && !d_hit){
                dcache_write_back(d_line,dcalculated_id);
            }
            d_line.tag = dcalculated_tag;
            d_line.dirty = 1;
            d_line.valid = 1;
        }
        break;
        case 4:
        //Hit_Invalidate
        if(accessed_cache == 0 && i_hit)i_line.valid = 0;
        if(accessed_cache == 1 && d_hit){d_line.valid = 0; d_line.dirty = 0;}
        break;
        case 5:
        if(accessed_cache == 1){
            //Hit_Write_Back_Invalidate
            if(d_hit && d_line.valid){
                if(d_line.dirty){
                    dcache_write_back(d_line,dcalculated_id);
                }
                d_line.valid = 0;
            }
        }else if(accessed_cache == 0){
            //Fill
            uint64_t line_start_addr = op.data_addr_p & ~0xF;
            for (int i = 0; i < 8; i++) decode_op(rcp->read_size(line_start_addr + i * 4, 4),i_line.data[i]) ;
            i_line.tag = icalculated_tag;
            i_line.valid = true;
        }
        break;
        case 6:
        if(accessed_cache == 1){
            //Hit_Write_Back
            if(d_hit && d_line.valid){
                if(d_line.dirty){
                    dcache_write_back(d_line,dcalculated_id);
                }
            }
        }else if(accessed_cache == 0){
            if(i_hit && i_line.valid){
                uint64_t line_start_addr = op.data_addr_p & ~0xF;
                for (int i = 0; i < 8; i++) rcp->write_size(line_start_addr + i * 4, i_line.data[i].opcode, 4);
            }
        }
        break;
        default:
        break;
    }
    return 0;
}

void VR4300Cycle::dcache_write_back(VR4300Cycle::Dcache_line& line, uint16_t index){
    uint64_t half_1 = dcache_read_size(line, 0, 8);
    uint64_t half_2 = dcache_read_size(line, 8, 8);
    rcp->write_size((line.tag << 12) + ((index & 0xFF) << 4),half_1,8);
    rcp->write_size((line.tag << 12) + ((index & 0xFF) << 4) + 8,half_2,8);
    line.dirty = 0;
}

VR4300Cycle::Operation::Operation()
{
    tmplt = &noptmplt;
}

const char *VR4300Cycle::Operation::op_name() const
{
    return optype_str[static_cast<uint32_t>(tmplt->instruction_type)];
}

std::ostream &operator<<(std::ostream &os, const VR4300Cycle::Operation &op)
{
    os<<"PC: "<< std::left <<std::setw(8) << std::hex <<(((op.PC) & 0xFFFFFFFF))
    << " Operation: "<< std::left << std::setw(8) << op.op_name() 
    <<" Result: 0x" << std::left << std::setw(16) << std::hex << op.result
    << " Rs val: "<< std::left <<std::setw(8) << std::hex << (int)op.rs_val
    << " Rt val: "<< std::left <<std::setw(8) << std::hex << (int)op.rt_val
    << " Rs: "<< std::left <<std::setw(3) << std::dec << (int)op.rs
    << " Rt: "<< std::left <<std::setw(3) << std::dec << (int)op.rt
    << " Dest reg: "<< std::left <<std::setw(3) << std::dec << (int)op.dest_reg
    << " Data addr phys: "<< std::left <<std::setw(7) << std::hex << (int)op.data_addr_p;
    return os;
}

inline void VR4300Cycle::fetch_regs()
{
    RF_in.op->rs_val = GPR[RF_in.op->rs];
    // RF_in.op->rd_val = *(RF_in.op->tmplt->rd_source_reg_file + RF_in.op->rd);
    // RF_in.op->rt_val = *(RF_in.op->tmplt->rt_source_reg_file + RF_in.op->rt);
    RF_in.op->rd_val = fetch_reg(RF_in.op->tmplt->rd_source_reg_file, RF_in.op->rd);
    RF_in.op->rt_val = fetch_reg(RF_in.op->tmplt->rt_source_reg_file, RF_in.op->rt);

    forward_write(*DC_in.op, *RF_in.op);
    forward_write(*EX_in.op, *RF_in.op);
}

inline uint64_t VR4300Cycle::fetch_reg(RegFile reg_file, uint8_t reg_num)
{
    switch (reg_file)
    {
    case RegFile::CPU_GPR:
        return GPR[reg_num];
        break;
    case RegFile::CP0_REGS:
        return cp0.regs[reg_num];
        break;
    case RegFile::FPU_CONTROL_REGS:
        return fpu.control_regs[reg_num];
        break;
    case RegFile::FPU_REGS:
        return fpu.regs[reg_num];
        break;
    default:
        break;
    }
    return 0;
}

inline void VR4300Cycle::forward_write(const VR4300Cycle::Operation &stage_op, VR4300Cycle::Operation &in_op)
{
    if (!(stage_op.tmplt->writes_reg) )
    return;
    if((stage_op.tmplt->writes_hi) || (stage_op.tmplt->writes_lo))
    return;
    if(stage_op.tmplt->dest_reg_file == RegFile::CPU_GPR && stage_op.dest_reg == 0)return;
    //
    //if (!(stage_op.tmplt->writes_cp))
    //    if (in_op.rs == stage_op.dest_reg) in_op.rs_val = stage_op.result;
    //
    //if(stage_op.tmplt->writes_cp && in_op.tmplt->reads_cp){
        //    if(stage_op.tmplt->writes_cp && stage_op.tmplt->CPz != in_op.tmplt->CPz)
        //        return;
        //    if (in_op.rt == stage_op.dest_reg) in_op.rt_val = (in_op.tmplt->CPz== 0)? (stage_op.result & cp0.write_masks[stage_op.dest_reg]): stage_op.result;
        //    if (in_op.rd == stage_op.dest_reg) in_op.rd_val = (in_op.tmplt->CPz== 0)? (stage_op.result & cp0.write_masks[stage_op.dest_reg]): stage_op.result;
        //    return;
        //}else if(!(stage_op.tmplt->writes_cp) && !(in_op.tmplt->reads_cp) && !(stage_op.dest_reg == 0))
        //    if (in_op.rt == stage_op.dest_reg) in_op.rt_val = stage_op.result;
        
    //uint64_t* dest = stage_op.tmplt->dest_reg_file_ptr + stage_op.dest_reg;
    ////if(dest == this->GPR)return; // if dest is 0
    //if((in_op.tmplt->rs_source_reg_file_ptr + in_op.rs) == dest) in_op.rs_val = stage_op.result;
    //if((in_op.tmplt->rt_source_reg_file_ptr + in_op.rt) == dest) in_op.rt_val = stage_op.result;
    //if((in_op.tmplt->rd_source_reg_file_ptr + in_op.rd) == dest) in_op.rd_val = stage_op.result;
    if(RegFile::CPU_GPR == stage_op.tmplt->dest_reg_file && in_op.rs == stage_op.dest_reg)in_op.rs_val = stage_op.result;
    if(in_op.tmplt->rt_source_reg_file == stage_op.tmplt->dest_reg_file && in_op.rt == stage_op.dest_reg)in_op.rt_val = stage_op.result;
    if(in_op.tmplt->rd_source_reg_file == stage_op.tmplt->dest_reg_file && in_op.rd == stage_op.dest_reg)in_op.rd_val = stage_op.result;
};

VR4300Cycle::OperationTemplate::OperationTemplate()
:execute(nullptr), flags(0), multicycle(0),access_size(0),instruction_type()
{
}

VR4300Cycle::OperationTemplate::OperationTemplate(void (*execute)(VR4300Cycle &cpu), uint32_t flags, uint8_t multicycle, uint8_t access_size,uint8_t CPz, OpType instruction_type)
    : execute(execute), flags(flags), multicycle(multicycle), access_size(access_size), instruction_type(instruction_type), CPz(CPz)
{
    is_load = flags & IS_LOAD;
    is_store = flags & IS_STORE;
    sign_extended = flags & SIGN_EXTENDED;
    causes_bd = flags & CAUSES_BRANCH_DELAY;
    reads_cp = flags & READS_CP;
    cp_control = flags & CPControl;
    writes_cp =  flags & WRITES_CP;
    writes_hi =  flags & WRITES_HI;
    writes_lo =  flags & WRITES_LO;
    writes_reg = flags & WRITES_REG;
    right_access = flags & RIGHT_ACCESS;
    left_access = flags & LEFT_ACCESS;
    is_trap = flags & IS_TRAP;
    atomic = flags & ATOMIC;
    is_cpz = flags & CPZ;
    stores_in_rd = flags & STORES_IN_RD;
    stores_in_31 = flags & STORES_IN_31;
    stores_in_rt = flags & STORES_IN_RT;
    stores_in_sa = flags & STORES_IN_SA;
    if(stores_in_rd)dest_id = DestId::RD;
    if(stores_in_31)dest_id = DestId::REG31;
    if(stores_in_rt)dest_id = DestId::RT;
    if(stores_in_sa)dest_id = DestId::SA;

    rt_source_reg_file = RegFile::CPU_GPR;
    rd_source_reg_file = RegFile::FPU_REGS;
    if(CPz == 0 && (reads_cp))
        rd_source_reg_file = RegFile::CP0_REGS;
    if(CPz == 1 && (reads_cp)){
        if(cp_control){
            rd_source_reg_file = RegFile::FPU_CONTROL_REGS;
        }
        else {
            rd_source_reg_file = RegFile::FPU_REGS;
            rt_source_reg_file = RegFile::FPU_REGS;
        }
    }

    if(writes_reg){
        if(writes_cp){
            if(CPz == 0){
                dest_reg_file = RegFile::CP0_REGS;
            }
            if(CPz== 1 && cp_control)
                dest_reg_file = RegFile::FPU_CONTROL_REGS;
            else if(CPz == 1) 
                dest_reg_file = RegFile::FPU_REGS;
        }else
            dest_reg_file = RegFile::CPU_GPR;
    }
}
