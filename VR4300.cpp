#include "VR4300.h"
#include "Operations.cpp"
#include <iostream>

VR4300::VR4300(Bus& bus):
cp0(),
bus(bus),
primary_op_lut{
    {},{},{},{},{},{},{},{},
    {ADDI, WRITES_REG | STORES_IN_RT, 0, ADDI_I}
},
special_op_lut{
    {},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},
    {ADD, WRITES_REG | STORES_IN_RD, 0, ADD_I},
},
regimm_op_lut{}
{

}

VR4300::~VR4300()
{

}

//todo
//make sure next_op_branchdelay is working correctly
//writing back to memory on cache miss and CACHE
//entire CACHE operation
//make sure RF pc incrementation doesn't fuck with jump instructions
//make sure data access ops access the correct size instead of always writing/loading a word
//make sure resultHI and resultLo are correctly handled on some ops
//add writing to coprocessors
//add exceptions.
//handle ll bit on conditional and link correctly
//fix asid
// overflow exception in dc
// interrupts and their exceptions
// all exception caused by operations will be added with them
// fpu exceptions will be added with it
// reset exceptions
// wrong operation is lethal anyway so skip for now
//add all the operations and populate the lut
void VR4300::on_clock()
{
    std::cout << "stall" << stall << "\n";
    //on interlock the ENTIRE pipeline is stalled
    if(stall){
        stall--;
        return;
    }

    //WB is last because it modifies state immidietely, even if the cpu should stall
    //I hope that is how the cpu actually works
    if (DC())return;
    if (EX())return;
    if (RF())return;
    if (IC())return;
    if (WB())return;
    //submit pipeline makes sure that cpu state only changes if there were no interlocks
    // this causes the order of stages to be arbitrary
    submit_pipeline();
}

//Pipeline writeback stage
bool VR4300::WB()
{
    auto& in = WB_in;
    //what WB does is:
    // write back. just as the name suggests really

    if(in.op.flags & WRITES_DATA && in.cacheable) Dcache[in.op.dcache_index].data[in.op.write_offset] = in.op.result;
    if(in.op.flags & WRITES_DATA && !in.cacheable) bus.write_word(in.op.write_destination,in.op.result);
    if(in.op.flags & WRITES_REG  && in.op.write_destination != 0) GPR[in.op.write_destination] = in.op.result;

    if (in.op.flags & CAUSED_EXCEPTION && DC_in.op.flags & READS_CP0 && !in.CP0I_triggered)
    {
        //CP0I
        stall = 1;
        in.CP0I_triggered = true;
        return true;
    }
    return false;
}

//Pipeline data cache stage
bool VR4300::DC()
{
    std::cout << "entered_DC \n";
    auto& in  = DC_in;
    auto& out = DC_out;

    //what DC does is:
    // tlb ranslates the addr of the data to be written to/from
    // if the data is read from the cache it fetches it
    //a lot of the code below is bloated by exception and interlock handling

    if(in.op.instruction_type == ICACHE_I){
        stall = CACHE_OP_STALL_TIME;
        return true;
    }
    if(in.op.instruction_type == DCACHE_I){
        stall = 2;
        return true;
    }
    if((in.op.flags & IS_TRAP) && in.op.result){
        handle_general_exception(in.op, Tr);
        return true;
    }

    if(!(in.op.flags & (WRITES_DATA | READS_DATA))){
        out.op = in.op;
        return false;
    }   

    uint32_t data_p_addr;

    const CP0::Segment& segment = cp0.get_segment(in.op.result);
    bool cacheable = segment.cacheable;
    
    if(segment.tlb_mapped){
        CP0::TLB_Result tlb_result = cp0.tlb_translate(in.op.result);
        if(tlb_result.miss){
            //tlb miss exception
            if(in.op.flags & IS_STORE) handle_tlb_miss_exception(in.op.result, in.op, TLBS);
            else handle_tlb_miss_exception(in.op.result, in.op, TLBL);
            return true;

        }
        if(!tlb_result.valid){
            cp0.badVAddr = in.op.result;
            cp0.context = cp0.set_bits(cp0.context,CONTEXT_BADVPN2_MASK,in.op.result >> 13); 
            cp0.xcontext = cp0.set_bits(cp0.xcontext,XCONTEXT_BADVPN2_MASK,in.op.result >> 13);
            cp0.entryHi = cp0.set_bits(cp0.entryHi,ENTRYHI_VPN2_MASK,in.op.result >> 13);
            if(in.op.flags & IS_STORE) handle_general_exception(in.op, TLBS);
            else handle_general_exception(in.op, TLBL);
            return true;
        }
        if(!tlb_result.dirty && in.op.flags & IS_STORE){
            cp0.badVAddr = in.op.result;
            cp0.context = cp0.set_bits(cp0.context,CONTEXT_BADVPN2_MASK,in.op.result >> 13); 
            cp0.xcontext = cp0.set_bits(cp0.xcontext,XCONTEXT_BADVPN2_MASK,in.op.result >> 13);
            cp0.entryHi = cp0.set_bits(cp0.entryHi,ENTRYHI_VPN2_MASK,in.op.result >> 13);
            handle_general_exception(in.op, Mod);
            return true;
        }

        data_p_addr = tlb_result.p_addr;
        cacheable = tlb_result.cache;
    }else data_p_addr = in.op.result - segment.translation_offset;
    in.op.dcache_index = (in.op.result & 0x1FF0) >> 4;

    //DADE
    if(
        //missaligned access
        (in.op.flags & ACCESSES_DOUBLE_WORD && data_p_addr % 8 != 0) ||
        (in.op.flags & ACCESSES_WORD && data_p_addr % 4 != 0) ||
        (in.op.flags & ACCESSES_HALF_WORD && data_p_addr % 2 != 0) ||
        // or wrong mode (user, kernel, supervisor)
        (cp0.in_user_mode() && !segment.user_accesible) ||
        (cp0.in_supervisor_mode() && !segment.supervisor_accesible) ||
        (cp0.in_kernel_mode() && !segment.kernel_accesible)
    ){
        cp0.badVAddr = in.op.result;
        if(in.op.flags & IS_STORE)handle_general_exception(in.op,AdES);
        else handle_general_exception(in.op,AdEL);
        return true;
    }

    if((cp0.watchLo & WATCHLO_R_MASK) && ((data_p_addr >> 3) == (cp0.watchLo>>3) && in.op.flags & IS_LOAD)){
        handle_general_exception(in.op,WATCH);
        return true;
    }
    if((cp0.watchLo & WATCHLO_W_MASK) && ((data_p_addr >> 3) == (cp0.watchLo>>3) && in.op.flags & IS_STORE)){
        handle_general_exception(in.op,WATCH);
        return true;
    }

    if(cacheable){
        Dcache_line& line = Dcache[in.op.dcache_index];
        bool cache_hit = ((data_p_addr >> 12) == line.tag);

        if(WB_in.op.flags & WRITES_DATA && cache_hit && !in.DCB_triggered){
            //DCB on hit
            stall = 1;
            in.DCB_triggered = true;
            return true;
        }

        if((!cache_hit || !line.valid)  && !in.DCB_triggered){
            //DCM + DCB
            stall = 1 + DCACHE_STALL_TIME;
            in.DCB_triggered = true;
            return true;
        }else if(in.DCB_triggered){
            //update dcache
            uint64_t line_start_addr = data_p_addr & ~0xF;
            for (int i = 0; i < 4; i++)
            {
                line.data[i] = bus.read_word(line_start_addr + i * 4);
            }
            line.tag = data_p_addr >> 12;
            line.valid = true;
        }

        if(in.op.flags & READS_DATA){
            //fetch data from the cache to put in a reg
            uint8_t offset = (in.op.result >> 2) & 0x3;
            in.op.result = line.data[offset];
        }else if(in.op.flags & WRITES_DATA){
            //nothing, WB is the write stage
        }
    }else{//this logic of avoiding cache is a little scuffed, might need some review
        if(in.op.flags & READS_DATA || in.op.flags & WRITES_DATA){
            if(!in.uncacheable_stall_triggered){
                stall = DCACHE_STALL_TIME;
                in.uncacheable_stall_triggered = 1;
                return true;
            } 
            if(in.op.flags & READS_DATA) in.op.result = bus.read_word(data_p_addr);
            if(in.op.flags & WRITES_DATA){in.op.write_destination = data_p_addr; out.cacheable = false;}
        }
    }
    out.op = in.op;
    return false;
}

//Pipeline execute stage
bool VR4300::EX()
{
    std::cout << "entered_EX \n";
    auto& in  = EX_in;
    auto& out = EX_out;
    auto& dc  = DC_in;
    auto& wb  = WB_in;
    //what EX does is:
    // gets data from earlier stages if needed registers were operated on
    // does the operation

    //see if override is nessesarry
    if(dc.op.flags & WRITES_REG && dc.op.write_destination != 0 && in.op.rs == dc.op.write_destination){
        if(dc.op.flags & IS_LOAD && !in.LDI_triggered){
            //LDI
            //only stall, we know what DC will load into a reg prematurely anyway
            stall = 1;
            in.LDI_triggered = true;
            return true;
        }
        in.op.rs_val = dc.op.result;
    }
    if(dc.op.flags & WRITES_REG && dc.op.write_destination != 0 && in.op.rt == dc.op.write_destination){
        if(dc.op.flags & IS_LOAD && !in.LDI_triggered){
            //LDI
            stall = 1;
            in.LDI_triggered = true;
            return true;
        }
        in.op.rt_val = dc.op.result;
    }
    if(wb.op.flags & WRITES_REG && wb.op.write_destination != 0 && in.op.rs == wb.op.write_destination)
        in.op.rs_val = wb.op.result;
    if(wb.op.flags & WRITES_REG && wb.op.write_destination != 0 && in.op.rt == wb.op.write_destination)
        in.op.rt_val = wb.op.result;

    if(in.op.multicycle && !in.MCI_triggered){
        //MCI
        stall = in.op.multicycle;
        in.MCI_triggered = true;
        return true;
    }

    if(in.op.instruction_type == SYSCALL_I){
        handle_general_exception(in.op, Sys);
        return true;
    }

    if(in.op.instruction_type == BREAK_I){
        handle_general_exception(in.op, Bp);
        return true;
    }

    std::cout << "operation rs, rt:" << in.op.rs << in.op.rt << "\n";
    
    in.op.execute(*this);
    out.op = in.op;
    return false;
}

//Pipeline register fetch stage
bool VR4300::RF()
{
    std::cout << "entered_RF \n";
    auto& in  = RF_in;
    auto& out = RF_out;

    //what RF does is:
    // get physical PC from TLB
    // get instruction from instruction cache
    // decode op_code
    // get values of registers to be messed with
    // increment PC
    //a lot of code below is bloated by exceptions and interlocks
    
    //add microtlb miss in the future
    if(next_op_bd){
        in.op.flags = in.op.flags | IS_IN_BRANCH_DELAY;
        next_op_bd = false;
    }else in.op.flags = in.op.flags & ~IS_IN_BRANCH_DELAY;


    uint32_t PC_p;
    CP0::Segment segment = cp0.get_segment(in.op.PC);
    bool cacheable = segment.cacheable;

    //IADE
    if(
        // wrong mode (user, kernel, supervisor)
        (cp0.in_user_mode() && !segment.user_accesible) ||
        (cp0.in_supervisor_mode() && !segment.supervisor_accesible) ||
        (cp0.in_kernel_mode() && !segment.kernel_accesible)
    ){
        cp0.badVAddr = in.op.PC;
        handle_general_exception(in.op,AdEL);
        return true;
    }

    if(segment.tlb_mapped){
        CP0::TLB_Result tlb_result = cp0.tlb_translate(in.op.PC);
        if(tlb_result.miss){
            //tlb miss exception
            handle_tlb_miss_exception(in.op.PC, in.op, TLBL);
            return true;
        }
        if(!tlb_result.valid){
            cp0.badVAddr = in.op.PC;
            cp0.context = cp0.set_bits(cp0.context,CONTEXT_BADVPN2_MASK,in.op.PC >> 13); 
            cp0.xcontext = cp0.set_bits(cp0.xcontext,XCONTEXT_BADVPN2_MASK,in.op.PC>> 13);
            cp0.entryHi = cp0.set_bits(cp0.entryHi,ENTRYHI_VPN2_MASK,in.op.PC >> 13);
            handle_general_exception(in.op, TLBL);
            return true;
        }
        PC_p = tlb_result.p_addr;
        cacheable = tlb_result.cache;
    }else PC_p = in.op.PC - segment.translation_offset;

    uint32_t op_code;
    if(cacheable){
        uint8_t offset = (PC_p >> 2) & 0x7;
        Icache_line& line = Icache[in.icache_index];
        if((!((PC_p >> 12) == line.tag) || !line.valid) && !in.ICB_triggered){
            //ICB
            stall = ICACHE_STALL_TIME;
            in.ICB_triggered = true;
            return true; 
        }else if(in.ICB_triggered){
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
    if(in.op.CPz == 0)in.op.cp_val = cp0.regs[in.op.rd];
    //if(in.op.CPz == 1)in.op.cp_val = fpu.regs[in.op.rd]; // uncomment this when making fpu

    if(in.op.flags & CAUSES_BRANCH_DELAY) next_op_bd = true;

    PC += 4;
    out.op = in.op;
    return false;
}

//Pipeline instruction cache stage
bool VR4300::IC()
{
    std::cout << "entered_IC \n";
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
    else
        tmplt = &primary_op_lut[opcode];

    if (!tmplt->execute) {
    // invalid instruction exception (RI)
    }

    Operation& op = RF_in.op;

    op.execute = tmplt->execute;
    op.multicycle = tmplt->multicycle;
    op.flags = tmplt->flags | (op.flags & IS_IN_BRANCH_DELAY);

    op.rs = (word >> 21) & 0x1F;
    op.rt = (word >> 16) & 0x1F;
    op.rd = (word >> 11) & 0x1F;
    op.sa = (word >> 6) & 0x1F;
    if(op.flags & STORES_IN_RD) op.write_destination = op.rd;
    if(op.flags & STORES_IN_RT) op.write_destination = op.rt;
    if(op.flags & STORES_IN_31) op.write_destination = 31;
    op.immediate = (word & 0xFFFF);
    op.target = (word & 0x3FFFFFF);
    op.CPz = (word >> 26) & 0x3;

}



VR4300::Operation::Operation()
{
    execute = NOP;
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
    abort_pipeline();
    //this is literally just the flow chart from page 203 copied
    cp0.badVAddr = addr;
    cp0.cause = cp0.set_bits(cp0.cause,CAUSE_EXCCODE_MASK,cause);
    cp0.context = cp0.set_bits(cp0.context,CONTEXT_BADVPN2_MASK,addr >> 13); 
    cp0.xcontext = cp0.set_bits(cp0.xcontext,XCONTEXT_BADVPN2_MASK,addr >> 13);
    cp0.entryHi = cp0.set_bits(cp0.entryHi,ENTRYHI_VPN2_MASK,addr >> 13);
    uint32_t EXL = cp0.get_bits(cp0.status, STATUS_EXL_MASK, STATUS_EXL_SHIFT);
    uint16_t jump_offset;

    if(!EXL){
        if(op.flags & IS_IN_BRANCH_DELAY){
            cp0.EPC = op.PC - 4;
            cp0.cause = cp0.set_bits(cp0.cause, CAUSE_BD_MASK, 1);
        } else{
            cp0.cause = cp0.set_bits(cp0.cause, CAUSE_BD_MASK, 0);
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
    cp0.status = cp0.set_bits(cp0.status, STATUS_EXL_MASK, 1);
    uint64_t jump_base = (cp0.status & STATUS_BEV_MASK)? BOOTSTRAP_EXCEPTION_VEC_64 : EXCEPTION_VEC_64;
    PC = jump_base + jump_offset;
}

//outside of this:
//set ce when coprocessor
//set fp status registers
//set tlb related registers 
//set badvaddr
void VR4300::handle_general_exception(const Operation& op, ExceptionCode cause){
    abort_pipeline();
    cp0.cause = cp0.set_bits(cp0.cause,CAUSE_EXCCODE_MASK,cause);

    uint32_t EXL = cp0.get_bits(cp0.status, STATUS_EXL_MASK, STATUS_EXL_SHIFT);
    if(!EXL){
        if(op.flags & IS_IN_BRANCH_DELAY){
            cp0.EPC = op.PC - 4;
            cp0.cause = cp0.set_bits(cp0.cause, CAUSE_BD_MASK, 1);
        } else{
            cp0.cause = cp0.set_bits(cp0.cause, CAUSE_BD_MASK, 0);
            cp0.EPC = op.PC;
        }
    }
    cp0.status = cp0.set_bits(cp0.status, STATUS_EXL_MASK, 1);
    uint64_t jump_base = (cp0.status & STATUS_BEV_MASK)? BOOTSTRAP_EXCEPTION_VEC_64 : EXCEPTION_VEC_64;
    PC = jump_base + 0x0180;
}