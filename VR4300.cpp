#include "VR4300.h"
#include "Operations.h"
#include <iostream>
#include <cstring>
#include <iomanip>
VR4300::VR4300(Bus& bus):
cp0(),
bus(bus)
{
    discard_bd = true;
}

VR4300::~VR4300()
{

}

//todo
//for sided access make sure cache across lines is working
//make sure next_op_branchdelay is working correctly
//fix asid
//add exceptions.
// overflow exception in dc
// interrupts and their exceptions
// all exception caused by operations will be added with them
// fpu exceptions will be added with it
// reset exceptions
// wrong operation is lethal anyway so skip for now
//fpu
void VR4300::on_clock()
{
    //on interlock the ENTIRE pipeline is stalled

    cp0.count++;//make sure this advances at the correct rate todo

    if(stall){
        stall--;
        return;
    }

    //writes back at the end to make sure cpu state doesn't get modified until submit pipeline
    if ( DC() || EX() || RF() || IC() || WB() ) return;
    PC += 4; //make sure it's ok for this to be here
    submit_pipeline();
}

//Pipeline writeback stage
bool VR4300::WB()
{
    auto& in = WB_in;
    
    if (in.op.flags & CAUSED_EXCEPTION && DC_in.op.flags & READS_CP0 && !in.CP0I_triggered)
    {
        //CP0I
        stall = 1;
        in.CP0I_triggered = true;
        return true;
    }
    //what WB does is:
    // write back. just as the name suggests really
    if(in.op.flags & IS_STORE){
        uint8_t access_size = in.op.flags & (ACCESSES_BYTE | ACCESSES_DOUBLE_WORD | ACCESSES_HALF_WORD | ACCESSES_WORD);
        if(in.op.flags & (LEFT_ACCESS | RIGHT_ACCESS)) in.op.data_addr_p = in.op.data_addr_p & ~(access_size - 1);
        if(in.cacheable){
            if((in.op.flags & ATOMIC) && !LLBit){
                GPR[in.op.dest_reg] = 0;
                return false;
            }else if((in.op.flags & ATOMIC) && LLBit) GPR[in.op.dest_reg] = 1;
            uint8_t offset = in.op.data_addr_p & 0xF;
            Dcache_line &line = Dcache[in.op.dcache_index];
            dcache_write_size(line, offset, in.op.result, access_size);
        }else{
            bus.write_size(in.op.data_addr_p, in.op.result, access_size);
        }
    }
    if(in.op.flags & WRITES_REG){
        if(in.op.flags & WRITES_CP){
            if(in.op.CPz == 0) cp0.regs[in.op.dest_reg] = in.op.result;
            //if(in.op.CPz == 1) fpu.regs[in.op.dest_reg] = in.op.result;
        }else
        if(in.op.dest_reg != 0)
            GPR[in.op.dest_reg] = in.op.result;
    }
    if(in.op.flags & WRITES_LO) LO = in.op.result_LO;
    if(in.op.flags & WRITES_HI) HI = in.op.result_HI;
    if(in.op.instruction_type == OpType::TLBR){
        cp0.entryHi = in.op.result_entryHI;
        cp0.entryLo0 = in.op.result_entryLO0;
        cp0.entryLo1 = in.op.result_entryLO1;
    }
    if(in.op.instruction_type == OpType::TLBP){
        cp0.index = in.op.result;
    }
    if(in.op.instruction_type == OpType::TLBWI || in.op.instruction_type == OpType::TLBWR){
        uint8_t tlb_index;
        if(in.op.instruction_type == OpType::TLBWI) tlb_index = cp0.index;
        else tlb_index = cp0.random;
        cp0.TLB[tlb_index][0] = in.op.result_pagemask;
        cp0.TLB[tlb_index][1] = in.op.result_entryHI;
        cp0.TLB[tlb_index][2] = in.op.result_entryLO0;
        cp0.TLB[tlb_index][3] = in.op.result_entryLO1;
    }
    if(((in.op.PC & 0xFFFC)) == 0x420){
        std::cout<<"stop condition";
    }
    if(in.op.result == 0x7FFFF888){
        std::cout<<"stop condition 2";
    }

    std::cout<<"PC: "<< std::left <<std::setw(3)<<((in.op.PC & 0xFFFC));
    std::cout<< " Operation: "<< std::left << std::setw(8) << in.op.op_name();
    std::cout<<" Result: 0x" << std::hex << std::left << std::setw(16) << in.op.result;
    if(in.op.rs_val) std::cout<< " Rs val: " << (int)in.op.rs_val;
    if(in.op.rt_val) std::cout<< " Rt val: " << (int)in.op.rt_val;
    if(in.op.rs) std::cout<< " Rs: " << (int)in.op.rs;
    if(in.op.rt) std::cout<< " Rt: " << (int)in.op.rt;
    if(in.op.dest_reg) std::cout<< " Dest reg: " << (int)in.op.dest_reg;
    std::cout<< "\n";
    
    return false;
}



//Pipeline data cache stage
bool VR4300::DC()
{
    auto& in  = DC_in;
    auto& out = DC_out;
    out.op = in.op;

    //what DC does is:
    // gets the segment operated on
    // tlb ranslates the addr of the data to be written to/from
    // if the data is read from the cache it fetches it
    //a lot of the code below is bloated by exception and interlock handling

    //if(in.op.instruction_type == ICACHE){
    //    stall = CACHE_OP_STALL_TIME;
    //    return true;
    //}
    //if(in.op.instruction_type == DCACHE){
    //    stall = 2;
    //    return true;
    //}

    
    if((in.op.flags & IS_TRAP) && in.op.result){
        handle_general_exception(in.op, Tr);
        return true;
    }
    
    if(!(in.op.flags & (IS_STORE | IS_LOAD)) && !(in.op.instruction_type == OpType::CACHE)){
        out.op = in.op;
        return false;
    }
    
    const CP0::Segment& segment = cp0.get_segment(in.op.data_addr);
    out.cacheable = segment.cacheable;
    
    if(segment.tlb_mapped){
        CP0::TLB_Result tlb_result = cp0.tlb_translate(in.op.data_addr);
        if(tlb_result.miss){
            //tlb miss exception
            if(in.op.flags & IS_STORE) handle_tlb_miss_exception(in.op.data_addr, in.op, TLBS);
            else handle_tlb_miss_exception(in.op.data_addr, in.op, TLBL);
            return true;
            
        }
        if(!tlb_result.valid){
            set_tlb_context(in.op.data_addr);
            if(in.op.flags & IS_STORE) handle_general_exception(in.op, TLBS);
            else handle_general_exception(in.op, TLBL);
            return true;
        }
        if(!tlb_result.dirty && in.op.flags & IS_STORE){
            set_tlb_context(in.op.data_addr);
            handle_general_exception(in.op, Mod);
            return true;
        }
        
        out.op.data_addr_p = tlb_result.p_addr;
        out.cacheable = tlb_result.cache;
    }else out.op.data_addr_p = in.op.data_addr - segment.translation_offset;
    out.op.dcache_index = (in.op.data_addr & 0x1FF0) >> 4;
    //DADE
    bool misalligned = (in.op.flags & ACCESSES_DOUBLE_WORD && out.op.data_addr_p % 8 != 0) ||
    (in.op.flags & ACCESSES_WORD && out.op.data_addr_p % 4 != 0) ||
    (in.op.flags & ACCESSES_HALF_WORD && out.op.data_addr_p % 2 != 0);
    bool wrong_mode = (cp0.in_user_mode() && !segment.user_accesible) ||
    (cp0.in_supervisor_mode() && !segment.supervisor_accesible) ||
    (cp0.in_kernel_mode() && !segment.kernel_accesible);
    bool sided = (in.op.flags & (RIGHT_ACCESS | LEFT_ACCESS));
    if( !sided && (misalligned || wrong_mode)){
        cp0.badVAddr = in.op.data_addr;
        if(in.op.flags & IS_STORE)handle_general_exception(in.op,AdES);
        else handle_general_exception(in.op,AdEL);
        return true;
    }
    
    
    if((cp0.watchLo & WATCHLO_R_MASK) && ((out.op.data_addr_p >> 3) == (cp0.watchLo>>3) && in.op.flags & IS_LOAD)){
        handle_general_exception(in.op,WATCH);
        return true;
    }
    if((cp0.watchLo & WATCHLO_W_MASK) && ((out.op.data_addr_p >> 3) == (cp0.watchLo>>3) && in.op.flags & IS_STORE)){
        handle_general_exception(in.op,WATCH);
        return true;
    }
    
    if(out.cacheable){
        Dcache_line& line = Dcache[out.op.dcache_index];
        bool cache_hit = ((out.op.data_addr_p >> 12) == line.tag);
        
        if(WB_in.op.flags & IS_STORE && cache_hit){
            //DCB on hit
            if(!in.DCB_triggered){
                stall = 1;
                in.DCB_triggered = true;
                return true;
            }else{
                WB();
                WB_in = {};
            }
        }

        if(in.op.instruction_type == OpType::CACHE){
            if(!in.COp_triggered){
                stall = handle_cache_op(out.op); // it might be smart to move this to after the stall
                in.COp_triggered = true;
                return true;
            }
        }

        if((!cache_hit || !line.valid)  && !in.DCB_triggered){
            //DCM + DCB
            stall = 1 + DCACHE_STALL_TIME;
            in.DCB_triggered = true;
            return true;
        }else if((!cache_hit || !line.valid)  && in.DCB_triggered){

            if(line.dirty){
                //write back previous entry
                dcache_write_back(line, out.op.dcache_index);
            }
            //update dcache
            uint64_t line_start_addr = out.op.data_addr_p & ~0xF;
            for (int i = 0; i < 16; i++) line.data[i] = bus.read_byte(line_start_addr + i);
            line.tag = out.op.data_addr_p >> 12;
            line.valid = true;
        }

        if(in.op.flags & IS_LOAD){
            if(in.op.flags & ATOMIC) LLBit = 1;
            //fetch data from the cache to put in a reg
            uint32_t offset_into_line = out.op.data_addr_p & 0xF;
            uint8_t access_size = in.op.flags & (ACCESSES_BYTE | ACCESSES_DOUBLE_WORD | ACCESSES_HALF_WORD | ACCESSES_WORD);            
            uint64_t mem = dcache_read_size(line,(offset_into_line & ~(access_size - 1)),access_size );

            uint8_t byte_offset = out.op.data_addr_p & (access_size - 1);
            uint8_t bit_offset = byte_offset * 8;
            uint8_t bits = access_size * 8;

            if (in.op.flags & LEFT_ACCESS){
                uint64_t mask = ~0ULL << (bits - bit_offset);
                out.op.result = (in.op.rt_val & ~mask) | (mem << bit_offset);
            }
            else if (in.op.flags & RIGHT_ACCESS){
                uint64_t mask = ~(~0ULL << bit_offset);
                out.op.result = (in.op.rt_val & ~mask) | (mem >> (bits - bit_offset));
            }else {
                bool sign_extended = in.op.flags & SIGN_EXTENDED;
                
                out.op.result = dcache_read_size(line,offset_into_line,access_size);
                if(access_size == 1) out.op.result = (sign_extended) ? (int64_t)(int8_t)out.op.result:(uint64_t)(uint8_t)out.op.result;
                else if(access_size == 2) out.op.result = (sign_extended) ? (int64_t)(int16_t)out.op.result:(uint64_t)(uint16_t)out.op.result;
                else if(access_size == 4) out.op.result = (sign_extended) ? (int64_t)(int32_t)out.op.result:(uint64_t)(uint32_t)out.op.result;
                else if(access_size == 8) out.op.result = (sign_extended) ? out.op.result:out.op.result;
            }

        }else if(in.op.flags & IS_STORE){
            line.dirty = 1;//this might have to happen in wb
        }
    }else{//if not cacheable
        if( in.op.flags & (IS_LOAD | IS_STORE)){
            if(!in.uncacheable_stall_triggered){
                stall = DCACHE_STALL_TIME;
                in.uncacheable_stall_triggered = 1;
                return true;
            } 
            uint8_t access_size = in.op.flags & (ACCESSES_BYTE | ACCESSES_DOUBLE_WORD | ACCESSES_HALF_WORD | ACCESSES_WORD);
            if(in.op.flags & IS_LOAD){

                //this is the most consise i could get it...
                uint64_t mem = bus.read_size(out.op.data_addr_p & ~(access_size - 1), access_size);

                uint8_t byte_offset = out.op.data_addr_p & (access_size - 1);
                uint8_t bit_offset = byte_offset * 8;
                uint8_t bits = access_size * 8;

                if (in.op.flags & LEFT_ACCESS){
                    uint64_t mask = ~0ULL << (bits - bit_offset);
                    out.op.result = (in.op.rt_val & ~mask) | (mem << bit_offset);
                }
                else if (in.op.flags & RIGHT_ACCESS){
                    uint64_t mask = ~(~0ULL << bit_offset);
                    out.op.result = (in.op.rt_val & ~mask) | (mem >> (bits - bit_offset));
                }else {
                    bool sign_extended = in.op.flags & SIGN_EXTENDED;
                    
                    out.op.result = bus.read_size(out.op.data_addr_p, access_size);
                    if(access_size == 1) out.op.result = (sign_extended) ? (int64_t)(int8_t)out.op.result:(uint64_t)(uint8_t)out.op.result;
                    else if(access_size == 2) out.op.result = (sign_extended) ? (int64_t)(int16_t)out.op.result:(uint64_t)(uint16_t)out.op.result;
                    else if(access_size == 4) out.op.result = (sign_extended) ? (int64_t)(int32_t)out.op.result:(uint64_t)(uint32_t)out.op.result;
                    else if(access_size == 8) out.op.result = (sign_extended) ? out.op.result:out.op.result;
                }
            }
            if(in.op.flags & IS_STORE){
                //this og_val can't get info from one instruction back in the future
                uint64_t og_val;
                if(WB_in.op.flags & IS_STORE && out.op.data_addr_p == WB_in.op.data_addr_p)
                    og_val = WB_in.op.result;
                else og_val = bus.read_size(out.op.data_addr_p & ~(access_size - 1), access_size);
                uint8_t byte_offset = out.op.data_addr_p & (access_size - 1);
                uint8_t bit_offset = byte_offset * 8;
                uint8_t bits = access_size * 8;
                if(in.op.flags & LEFT_ACCESS){
                    uint64_t mask = ~(~0ULL << (bit_offset));
                    out.op.result = (og_val & mask) | out.op.result;
                }else if(in.op.flags & RIGHT_ACCESS){
                    uint64_t mask = ~0ULL << (bits - bit_offset);
                    out.op.result = (og_val & mask) | out.op.result;
                }
            }
        }
    }
    return false;
}

//Pipeline execute stage
bool VR4300::EX()
{
    auto& in  = EX_in;
    auto& out = EX_out;
    auto& dc  = DC_out;
    //what EX does is:
    // 
    // does the operation


    if(dc.op.flags & IS_LOAD && !in.LDI_triggered && dc.op.dest_reg != 0 && in.op.rt == dc.op.dest_reg){
        //LDI
        stall = 1;
        in.LDI_triggered = true;
        return true;
    }else if(in.LDI_triggered && in.op.rt == dc.op.dest_reg){
        in.op.rt_val = dc.op.result;
    }

    if(dc.op.flags & IS_LOAD && !in.LDI_triggered && dc.op.dest_reg != 0 && in.op.rs == dc.op.dest_reg){
        //LDI
        stall = 1;
        in.LDI_triggered = true;
        return true;
    }else if(in.LDI_triggered && in.op.rs == dc.op.dest_reg){
        in.op.rs_val = dc.op.result;
    }


    if(in.op.multicycle && !in.MCI_triggered){
        //MCI
        stall = in.op.multicycle;
        in.MCI_triggered = true;
        return true;
    }

    if(in.op.instruction_type == OpType::SYSCALL){
        handle_general_exception(in.op, Sys);
        return true;
    }

    if(in.op.instruction_type == OpType::BREAK){
        handle_general_exception(in.op, Bp);
        return true;
    }

    in.op.execute(*this);
    out.op = in.op;
    return false;
}

//Pipeline register fetch stage
bool VR4300::RF()
{
    auto& in  = RF_in;
    auto& out = RF_out;
    auto& ex  = EX_out;
    auto& dc  = DC_out;
    auto& wb  = WB_in;

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
    if(discard_bd){
        discard_bd = false;
        uint64_t prev_PC = in.op.PC;
        in.op = Operation();
        in.op.PC = prev_PC;
        out.op = in.op;
        return false;
    }

    if(next_op_bd){
        in.op.flags = in.op.flags | IS_IN_BRANCH_DELAY;
        next_op_bd = false;
    }else in.op.flags = in.op.flags & ~IS_IN_BRANCH_DELAY;


    uint32_t PC_p;
    CP0::Segment segment = cp0.get_segment(RF_in.op.PC);
    bool cacheable = segment.cacheable;

    //IADE
    if(
        // wrong mode (user, kernel, supervisor)
        (cp0.in_user_mode() && !segment.user_accesible) ||
        (cp0.in_supervisor_mode() && !segment.supervisor_accesible) ||
        (cp0.in_kernel_mode() && !segment.kernel_accesible)
    ){
        cp0.badVAddr = RF_in.op.PC;
        handle_general_exception(in.op,AdEL);
        return true;
    }

    if(segment.tlb_mapped){
        CP0::TLB_Result tlb_result = cp0.tlb_translate(RF_in.op.PC);
        if(tlb_result.miss){
            //tlb miss exception
            handle_tlb_miss_exception(RF_in.op.PC, in.op, TLBL);
            return true;
        }
        if(!tlb_result.valid){
            set_tlb_context(RF_in.op.PC);
            handle_general_exception(in.op, TLBL);
            return true;
        }
        PC_p = tlb_result.p_addr;
        cacheable = tlb_result.cache;
    }else PC_p = RF_in.op.PC - segment.translation_offset;

    uint32_t op_code;
    if(cacheable){
        uint8_t offset = (PC_p >> 2) & 0x7;
        Icache_line& line = Icache[in.icache_index];
        if((!((PC_p >> 12) == line.tag) || !line.valid) && !in.ICB_triggered){
            //ICB
            stall = ICACHE_STALL_TIME;
            in.ICB_triggered = true;
            return true; 
        }else if((!((PC_p >> 12) == line.tag) || !line.valid) && in.ICB_triggered){
            //update icache
            uint64_t line_start_addr = PC_p & ~ 0x1F;
            for (int i = 0; i < 8; i++)
            {
                line.data[i] = bus.read_word(line_start_addr + i * 4);
            }
            line.tag = PC_p >> 12;
            line.valid = true;
        }
        op_code = line.data[offset];
    }else{//this logic of avoiding cache is a little scuffed, might need some review
        if(!in.uncacheable_stall_triggered){
            //not cacheable so read diractly from memory
            stall = DCACHE_STALL_TIME; //probably takes as long as refilling dcache
            in.uncacheable_stall_triggered = 1;
            return true;
        }
        op_code = bus.read_word(PC_p);
    }

    decode_op(op_code);

    in.op.rs_val = GPR[in.op.rs];
    in.op.rt_val = GPR[in.op.rt];

    //see if override is nessesarry
    if(wb.op.flags & WRITES_REG && wb.op.dest_reg != 0 && in.op.rs == wb.op.dest_reg)
        in.op.rs_val = wb.op.result;
    if(wb.op.flags & WRITES_REG && wb.op.dest_reg != 0 && in.op.rt == wb.op.dest_reg)
        in.op.rt_val = wb.op.result;
    if(dc.op.flags & WRITES_REG && dc.op.dest_reg != 0 && in.op.rs == dc.op.dest_reg)
        in.op.rs_val = dc.op.result;
    if(dc.op.flags & WRITES_REG && dc.op.dest_reg != 0 && in.op.rt == dc.op.dest_reg)
        in.op.rt_val = dc.op.result;
    if(ex.op.flags & WRITES_REG && ex.op.dest_reg != 0 && in.op.rs == ex.op.dest_reg)
        in.op.rs_val = ex.op.result;
    if(ex.op.flags & WRITES_REG && ex.op.dest_reg != 0 && in.op.rt == ex.op.dest_reg)
        in.op.rt_val = ex.op.result;

    if(in.op.CPz == 0)in.op.cp_val = cp0.regs[in.op.rd];
    //if(in.op.CPz == 1)in.op.cp_val = fpu.regs[in.op.rd]; // uncomment this when making fpu

    if(in.op.flags & CAUSES_BRANCH_DELAY) next_op_bd = true;

    out.op = in.op;
    return false;
}

//Pipeline instruction cache stage
bool VR4300::IC()
{
    //what IC does is:
    // address icache and microtlb
    //it's comical how little it does

    IC_out.icache_index = (PC & 0x3FE0) >> 5;
    IC_out.op.PC = PC;
    return false;
}

void VR4300::submit_pipeline(){
    RF_in = IC_out;
    EX_in = RF_out;
    DC_in = EX_out;
    WB_in = DC_out;
}

void VR4300::decode_op(uint32_t word)
{
    uint8_t opcode = word >> 26;

    const OperationTemplate* tmplt;

    if(opcode == 0)
        tmplt = &special_op_lut[word & 0x3F];
    else if(opcode == 1)
        tmplt = &regimm_op_lut[(word >> 16) & 0x1F];
    else if((opcode >> 2) == 4){
        if(((word >> 21) & 0x1F) == 8)
            tmplt = &COPzrt_op_lut[(word >> 16) & 0x1F];
        else if((word >> 25) == 33)
            tmplt = &CP0_op_lut[word & 0x3F];
        else
        tmplt = &COPzrs_op_lut[(word >> 21) & 0x1F];
    }
    else
        tmplt = &primary_op_lut[opcode];

        
    Operation& op = RF_in.op;
    if (!tmplt->execute) {
        // invalid instruction exception (RI)
        handle_general_exception(op,RI);
    }

    op.execute = tmplt->execute;
    op.multicycle = tmplt->multicycle;
    op.flags = tmplt->flags | (op.flags & IS_IN_BRANCH_DELAY);
    op.instruction_type = tmplt->instruction_type;

    op.rs = (word >> 21) & 0x1F;
    op.rt = (word >> 16) & 0x1F;
    op.rd = (word >> 11) & 0x1F;
    op.sa = (word >> 6) & 0x1F;
    if(op.flags & STORES_IN_RD) op.dest_reg = op.rd;
    if(op.flags & STORES_IN_RT) op.dest_reg = op.rt;
    if(op.flags & STORES_IN_31) op.dest_reg = 31;
    op.immediate = (word & 0xFFFF);
    op.target = (word & 0x3FFFFFF);
    op.CPz = (word >> 26) & 0x3;

}

void VR4300::abort_pipeline() {
    RF_in = {};
    EX_in = {};
    DC_in = {};
    WB_in = {};
    
    RF_out = {};
    EX_out = {};
    DC_out = {};
    IC_out = {};
    stall = 0; // maybe
    next_op_bd = false;
}

void VR4300::handle_tlb_miss_exception(uint64_t addr, const Operation& op, ExceptionCode cause){
    std::cout<<"tlb_exception! \n";
    abort_pipeline();
    //this is literally just the flow chart from page 203 copied
    set_tlb_context(addr);
    uint32_t EXL = cp0.get_bits(cp0.status, STATUS_EXL_MASK, STATUS_EXL_SHIFT);
    uint16_t jump_offset;
    
    if(!EXL){
        if(op.flags & IS_IN_BRANCH_DELAY){
            cp0.EPC = op.PC - 4;
            cp0.cause = cp0.set_bits(cp0.cause, CAUSE_BD_MASK, 1 << CAUSE_BD_SHIFT);
        } else{
            cp0.cause = cp0.set_bits(cp0.cause, CAUSE_BD_MASK, 0 << CAUSE_BD_SHIFT);
            cp0.EPC = op.PC;
        }
        uint8_t UX = cp0.get_bits(cp0.status,STATUS_UX_MASK, STATUS_UX_SHIFT);
        uint8_t SX = cp0.get_bits(cp0.status,STATUS_SX_MASK, STATUS_SX_SHIFT);
        uint8_t KX = cp0.get_bits(cp0.status,STATUS_KX_MASK, STATUS_KX_SHIFT);
        bool is_xmode = (cp0.in_user_mode() && UX)|| (cp0.in_supervisor_mode() && SX) || (cp0.in_kernel_mode() && KX);
        if(is_xmode) jump_offset=0x0080;
        else      jump_offset=0x0000;
    }else{
        jump_offset=0x180; // says 80 in flow chart but 180 in description. 180 makes more sense prolly
    }
    cp0.status = cp0.set_bits(cp0.status, STATUS_EXL_MASK, 1 << STATUS_EXL_SHIFT);
    uint64_t jump_base = (cp0.status & STATUS_BEV_MASK)? BOOTSTRAP_EXCEPTION_VEC_64 : EXCEPTION_VEC_64;
    PC = jump_base + jump_offset;
}

//outside of this:
//set ce when coprocessor
//set fp status registers
//set tlb related registers 
//set badvaddr
void VR4300::handle_general_exception(const Operation& op, ExceptionCode cause){
    std::cout<<"general_exception! \n";
    abort_pipeline();
    cp0.cause = cp0.set_bits(cp0.cause,CAUSE_EXCCODE_MASK,cause<<CAUSE_EXCCODE_SHIFT);
    
    uint32_t EXL = cp0.get_bits(cp0.status, STATUS_EXL_MASK, STATUS_EXL_SHIFT);
    if(!EXL){
        if(op.flags & IS_IN_BRANCH_DELAY){
            cp0.EPC = op.PC - 4;
            cp0.cause = cp0.set_bits(cp0.cause, CAUSE_BD_MASK, 1 << CAUSE_BD_SHIFT);
        } else{
            cp0.cause = cp0.set_bits(cp0.cause, CAUSE_BD_MASK, 0 << CAUSE_BD_MASK);
            cp0.EPC = op.PC;
        }
    }
    cp0.status = cp0.set_bits(cp0.status, STATUS_EXL_MASK, 1);
    uint64_t jump_base = (cp0.status & STATUS_BEV_MASK)? BOOTSTRAP_EXCEPTION_VEC_64 : EXCEPTION_VEC_64;
    PC = jump_base + 0x0180;
}

inline void VR4300::set_tlb_context(uint64_t addr){
    cp0.badVAddr = addr;
    cp0.context  = cp0.set_bits(cp0.context, CONTEXT_BADVPN2_MASK, (addr >> 13) << CONTEXT_BADVPN2_SHIFT);
    cp0.xcontext = cp0.set_bits(cp0.xcontext, XCONTEXT_BADVPN2_MASK, (addr >> 13) << XCONTEXT_BADVPN2_SHIFT);
    cp0.entryHi  = cp0.set_bits(cp0.entryHi, ENTRYHI_VPN2_MASK, (addr >> 13) << ENTRYHI_VPN2_SHIFT);
}

void VR4300::dcache_write_size(VR4300::Dcache_line &line, uint8_t offset, uint64_t value, uint8_t size)
{
    for (int i = 0; i < size; i++)
    {
        *(line.data + offset + i) = (uint8_t)(value >> ((size - 1 - i) * 8));
    }
}

uint64_t VR4300::dcache_read_size(VR4300::Dcache_line &line, uint8_t offset, uint8_t size)
{
    uint64_t result = 0;
    for (int i = 0; i < size; i++)
    {
        result |= (uint64_t)*(line.data + offset + i) << ((size - 1 - i) * 8);
    }
    return result;
}

uint8_t VR4300::handle_cache_op(VR4300::Operation op){
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
        if(accessed_cache == 0) i_line.tag = cp0.get_bits(cp0.tagLo, TAGLO_PTAGLO_MASK, TAGLO_PTAGLO_SHIFT);
        else d_line.tag = cp0.get_bits(cp0.tagLo, TAGLO_PTAGLO_MASK, TAGLO_PTAGLO_SHIFT);
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
            for (int i = 0; i < 8; i++) i_line.data[i] = bus.read_word(line_start_addr + i * 4);
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
                for (int i = 0; i < 8; i++) bus.write_word(line_start_addr + i * 4, i_line.data[i]);
            }
        }
        break;
        default:
        break;
    }
    return 0;
}

void VR4300::dcache_write_back(VR4300::Dcache_line& line, uint16_t index){
    uint64_t half_1 = dcache_read_size(line, 0, 8);
    uint64_t half_2 = dcache_read_size(line, 8, 8);
    bus.write_doubleword((line.tag << 12) + (index << 4),half_1);
    bus.write_doubleword((line.tag << 12) + (index << 4) + 8,half_2);
    line.dirty = 0;
}

VR4300::Operation::Operation()
{
    execute = NOP;
}

const char *VR4300::Operation::op_name()
{
    return optype_str[static_cast<uint32_t>(instruction_type)];
}