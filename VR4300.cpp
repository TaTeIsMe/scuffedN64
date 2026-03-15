#include "VR4300.h"
#include <iostream>

VR4300::VR4300(Bus& bus):
cp0(),
bus(bus),
primary_op_lut{
    {},{},{},{},{},{},{},{},
    {ADDI, WRITES_REG | STORES_IN_RT, 0, 0}
},
special_op_lut{
    {},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},
    {ADD, WRITES_REG | STORES_IN_RD, 0, 0},
},
regimm_op_lut{}
{

}

VR4300::~VR4300()
{

}

//todo
//add exceptions.
// instruction address error when address sapces are done
// interrupts and their exceptions
// all exception caused by operations will be added with them
// address space exceptions will be added with them
// fpu exceptions will be added with it
// reset exceptions
// wrong operation is lethal anyway so skip for now
//add virtual address spaces
//add tlb translation logic
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
    // this causes the order of stages arbitrary
    submit_pipeline();
}

//Pipeline writeback stage
bool VR4300::WB()
{
    auto& in  = WB_in;
    //what WB does is:
    // write back. just as the name suggests really

    if(in.op.flags & WRITES_CACHE ) Dcache[in.op.dcache_index].data[in.op.write_offset] = in.op.result;
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

    if(in.op.instruction_type == ICACHE){
        stall = CACHE_OP_STALL_TIME;
        return true;
    }
    if(in.op.instruction_type == DCACHE){
        stall = 2;
        return true;
    }

    if(in.op.flags & (WRITES_CACHE | READS_CACHE)){

        
        //uint64_t data_addr = rs_val + offset; move to execute
        CP0::TLB_Result tlb_result = cp0.tlb_translate(in.data_addr);
        uint64_t data_p_addr;
        if(tlb_result.miss){
            //tlb miss exception
            if(in.op.flags & IS_STORE) handle_tlb_miss_exception(in.data_addr, in.op, TLBS);
            else handle_tlb_miss_exception(in.data_addr, in.op, TLBL);
            
            return true;

        }
        if(!tlb_result.valid){
            cp0.badVAddr = in.data_addr;
            cp0.context = cp0.set_bits(cp0.context,CONTEXT_BADVPN2_MASK,in.data_addr >> 13); 
            cp0.xcontext = cp0.set_bits(cp0.xcontext,XCONTEXT_BADVPN2_MASK,in.data_addr >> 13);
            cp0.entryHi = cp0.set_bits(cp0.entryHi,ENTRYHI_VPN2_MASK,in.data_addr >> 13);
            if(in.op.flags & IS_STORE) handle_general_exception(in.op, TLBS);
            else handle_general_exception(in.op, TLBL);
            return true;
        }
        if(!tlb_result.dirty && in.op.flags & IS_STORE){
            cp0.badVAddr = in.data_addr;
            cp0.context = cp0.set_bits(cp0.context,CONTEXT_BADVPN2_MASK,in.data_addr >> 13); 
            cp0.xcontext = cp0.set_bits(cp0.xcontext,XCONTEXT_BADVPN2_MASK,in.data_addr >> 13);
            cp0.entryHi = cp0.set_bits(cp0.entryHi,ENTRYHI_VPN2_MASK,in.data_addr >> 13);
            handle_general_exception(in.op, Mod);
            return true;
        }
        data_p_addr = tlb_result.p_addr;
        in.op.dcache_index = (in.data_addr & 0x1FF0) >> 4;

        //DADE
        if(
            //missaligned access
            (in.op.flags & ACCESSES_DOUBLE_WORD && data_p_addr % 8 != 0) ||
            (in.op.flags & ACCESSES_WORD && data_p_addr % 4 != 0) ||
            (in.op.flags & ACCESSES_HALF_WORD && data_p_addr % 2 != 0)
            // or wrong mode (user, kernel, supervisor) todo
        ){
            cp0.badVAddr = in.data_addr;
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
        Dcache_line& line = Dcache[in.op.dcache_index];
        bool cache_hit = ((data_p_addr >> 12) == line.tag);
        

        if(WB_in.op.flags & WRITES_CACHE && cache_hit && !in.DCB_triggered){
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

        if(in.op.flags & READS_CACHE){
            //fetch data from the cache to put in a reg
            uint8_t offset = (in.data_addr >> 2) & 0x3;
            in.op.result = line.data[offset];
        }else if(in.op.flags & WRITES_CACHE){
            //nothing, WB is the write stage
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
    if(dc.op.flags & WRITES_REG && in.op.rs == dc.op.write_destination){
        if(dc.op.flags & IS_LOAD && !in.LDI_triggered){
            //LDI
            //only stall, we know what DC will load into a reg prematurely anyway
            stall = 1;
            in.LDI_triggered = true;
            return true;
        }
        in.op.rs_val = dc.op.result;
    }
    if(dc.op.flags & WRITES_REG && in.op.rt == dc.op.write_destination){
        if(dc.op.flags & IS_LOAD && !in.LDI_triggered){
            //LDI
            stall = 1;
            in.LDI_triggered = true;
            return true;
        }
        in.op.rt_val = dc.op.result;
    }
    if(wb.op.flags & WRITES_REG && in.op.rs == wb.op.write_destination)
        in.op.rs_val = wb.op.result;
    if(wb.op.flags & WRITES_REG && in.op.rt == wb.op.write_destination)
        in.op.rt_val = wb.op.result;

    if(in.op.multicycle && !in.MCI_triggered){
        //MCI
        stall = in.op.multicycle;
        in.MCI_triggered = true;
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
    if(in.next_op_bd){
        in.op.flags = in.op.flags | IS_IN_BRANCH_DELAY;
        in.next_op_bd = false;
    }else in.op.flags = in.op.flags & ~IS_IN_BRANCH_DELAY;

    CP0::TLB_Result tlb_result = cp0.tlb_translate(in.op.PC);
    uint64_t PC_p;
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
    uint32_t op_code = line.data[offset];
    decode_op(op_code);
    if(in.op.flags & CAUSES_BRANCH_DELAY) in.next_op_bd = true;

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

    Operation& op = RF_in.op;

    op.execute = tmplt->execute;
    op.multicycle = tmplt->multicycle;
    op.flags = tmplt->flags | (op.flags & IS_IN_BRANCH_DELAY);

    op.rs = (word >> 21) & 31;
    op.rt = (word >> 16) & 31;
    op.rd = (word >> 11) & 31;
    if(op.flags & STORES_IN_RD) op.write_destination = op.rd;
    else if(op.flags & STORES_IN_RT) op.write_destination = op.rt;
    op.immediate = (int16_t)(word & 0xFFFF);
    op.rs_val = GPR[op.rs];
    op.rt_val = GPR[op.rt];
}

void ADD(VR4300 &cpu)
{
    cpu.EX_in.op.result = cpu.EX_in.op.rs_val + cpu.EX_in.op.rt_val;
}

void ADDI(VR4300 &cpu)
{
    cpu.EX_in.op.result = cpu.EX_in.op.rs_val + cpu.EX_in.op.immediate;
}

VR4300::Operation::Operation()
{
    execute = ADD;
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
        if(xmode) jump_offset=0x0080;
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