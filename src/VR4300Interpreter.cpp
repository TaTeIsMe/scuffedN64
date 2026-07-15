#include "VR4300Interpreter.h"
#include "RCP.h"
#include "OperationsInterpreter.h"
#include <iomanip>

constexpr uint32_t rs(uint32_t op)    { return (op >> 21) & 0x1F; }
constexpr uint32_t rt(uint32_t op)    { return (op >> 16) & 0x1F; }
constexpr uint32_t rd(uint32_t op)    { return (op >> 11) & 0x1F; }
constexpr uint32_t sa(uint32_t op)    { return (op >>  6) & 0x1F; }
constexpr uint32_t funct(uint32_t op) { return op & 0x3F; }
constexpr uint32_t imm(uint32_t op)   { return op & 0xFFFF; }
constexpr uint32_t target(uint32_t op){ return op & 0x03FFFFFF; }
constexpr uint32_t cz(uint32_t op)    { return (op >> 26) & 0x3; }

Instruction decode_op(uint32_t op_code){

    if((op_code >> 26) == 0)
    return i_special_op_lut[op_code & 0x3F];
    else if((op_code >> 26) == 1)
    return i_regimm_op_lut[(op_code >> 16) & 0x1F];
    else if((op_code >> 28) == 4){
        if(((op_code >> 21) & 0x1F) == 8){
            if(((op_code >> 26) & 0x3) == 0)
                return i_COP0rt_op_lut[(op_code >> 16) & 0x1F];
            else if(((op_code >> 26) & 0x3) == 1)
                return i_COP1rt_op_lut[(op_code >> 16) & 0x1F];
            else
                return i_COP2rt_op_lut[(op_code >> 16) & 0x1F];
        }
        else if((op_code >> 25) == 33)
            return i_CP0_op_lut[op_code & 0x3F];
        else if((op_code >> 25) == 35){
            uint8_t fmt = rs(op_code);
            if(fmt == 16)
                return i_CP1_op_lut_S[op_code & 0x3F];
            else if(fmt == 17)
                return i_CP1_op_lut_D[op_code & 0x3F];
            else if(fmt == 20)
                return i_CP1_op_lut_W[op_code & 0x3F];
            else if(fmt == 21)
                return i_CP1_op_lut_L[op_code & 0x3F];
            else
                return nullptr;
        }
        else{
            if(((op_code >> 26) & 0x3) == 0)
                return i_COP0rs_op_lut[(op_code >> 21) & 0x1F];
            else if(((op_code >> 26) & 0x3) == 1)
                return i_COP1rs_op_lut[(op_code >> 21) & 0x1F];
            else
                return i_COP2rs_op_lut[(op_code >> 21) & 0x1F];
        }
    }
    else
    return i_primary_op_lut[(op_code >> 26)];

}

const char* decode_op_name(uint32_t op_code){

    if((op_code >> 26) == 0)
    return i_special_op_name_lut[op_code & 0x3F];
    else if((op_code >> 26) == 1)
    return i_regimm_op_name_lut[(op_code >> 16) & 0x1F];
    else if((op_code >> 28) == 4){
        if(((op_code >> 21) & 0x1F) == 8){
            if(((op_code >> 26) & 0x3) == 0)
                return i_COP0rt_op_name_lut[(op_code >> 16) & 0x1F];
            else if(((op_code >> 26) & 0x3) == 1)
                return i_COP1rt_op_name_lut[(op_code >> 16) & 0x1F];
            else
                return i_COP2rt_op_name_lut[(op_code >> 16) & 0x1F];
        }
        else if((op_code >> 25) == 33)
        return i_CP0_op_name_lut[op_code & 0x3F];
        else if((op_code >> 25) == 35)
        return i_CP1_op_name_lut[op_code & 0x3F];
        else{
            if(((op_code >> 26) & 0x3) == 0)
                return i_COP0rs_op_name_lut[(op_code >> 21) & 0x1F];
            else if(((op_code >> 26) & 0x3) == 1)
                return i_COP1rs_op_name_lut[(op_code >> 21) & 0x1F];
            else
                return i_COP2rs_op_name_lut[(op_code >> 21) & 0x1F];
        }
    }
    else
    return i_primary_op_name_lut[(op_code >> 26)];

}

void VR4300Interpreter::on_pclock(){

    //TODO replace with actual timings
    static uint64_t i = 0;
    i++;
    if(i%2 == 0){
        next_PC = PC + 4;
        
        do_instruction();
        
        PC = next_PC;
    }
}

void VR4300Interpreter::print_instruction(uint32_t op_code)
{
    std::cout<<"PC: "<< std::left <<std::setw(8) << std::hex <<(((PC) & 0xFFFFFFFF))
    << " Operation: "<< std::left << std::setw(8) << decode_op_name(op_code)
    << " Rs val: "<< std::left <<std::setw(8) << std::hex << (int)GPR[rs(op_code)]
    << " Rt val: "<< std::left <<std::setw(8) << std::hex << (int)GPR[rt(op_code)]
    << " Rs: "<< std::left <<std::setw(3) << std::dec << (int)rs(op_code)
    << " Rt: "<< std::left <<std::setw(3) << std::dec << (int)rt(op_code)
    << " Rd: "<< std::left <<std::setw(3) << std::dec << (int)rd(op_code)
    << " Imm: "<< std::left << std::setw(8) << std::hex << (int)imm(op_code)
    << "\n";
}

bool VR4300Interpreter::validate_PC_align(uint64_t PC)
{
    if(PC & 0x3){
        cp0.badVAddr = PC;
        handle_general_exception(PC,AdEL);
        return false;
    }
    return true;
}

bool VR4300Interpreter::cp_usable(uint8_t cp)
{
    uint8_t CU = (cp0.status >> 28) & 0xF;

    if(!((CU >> cp) & 1)){
        if(!(cp == 0 && (cp0.mode == Mode::KERNEL))){
            cp0.cause = cp0.set_bits(cp0.cause,0x3 << 28,cp << 28);
            handle_general_exception(PC,CpU);
            return false;
        }
    }

    return true;
}

void VR4300Interpreter::do_instruction(){

    dec_random();

    inc_count();

    uint8_t IP = cp0.get_bits(cp0.cause,CAUSE_IP_MASK,CAUSE_IP_SHIFT);
    if((IP & cp0.IM) && cp0.IE && !cp0.EXL && !cp0.ERL){
        handle_general_exception(PC,Int);
        return;
    }
    
    std::optional<uint32_t> PC_p = translate_address(PC);
    
    if (!PC_p)
        return;

    uint32_t op_code = 0;
    //TODO more graceful memory access
    if((*PC_p >> 24) < 4)
        op_code = rcp->rdram.read_size(*PC_p,4);
    else 
        op_code = rcp->read_size(*PC_p,4);

    //print_instruction(op_code);

    Instruction instr_func = decode_op(op_code);

    if (instr_func == nullptr){
        handle_general_exception(PC,RI);
        return;
    }

    instr_func(*this, op_code);
    GPR[0] = 0;

}

void VR4300Interpreter::do_bd(){
    PC += 4;
    is_bd = true;
    do_instruction();
    is_bd = false;
}

std::optional<uint32_t> VR4300Interpreter::translate_address(uint64_t v_addr){

    const CP0::Segment& segment = cp0.get_segment(v_addr);
    uint32_t p_addr = 0;

    if(segment.tlb_mapped){
        CP0::TLB_Result tlb_result = cp0.tlb_translate(v_addr);
        if(tlb_result.miss){
            //tlb miss exception
            handle_tlb_miss_exception(PC, v_addr, TLBL);
            return std::nullopt;
        }
        if(!tlb_result.valid){
            set_tlb_context(v_addr);
            handle_general_exception(PC, TLBL);
            return std::nullopt;
        }
        p_addr = tlb_result.p_addr;
    }else 
    p_addr = v_addr - segment.translation_offset;

    return p_addr;

}

VR4300Interpreter::VR4300Interpreter():fpu(cp0){}

void VR4300Interpreter::handle_tlb_miss_exception(uint64_t op_PC, uint64_t addr, ExceptionCode cause)
{
    //this is literally just the flow chart from page 203 copied
    cp0.cause = cp0.set_bits(cp0.cause,CAUSE_EXCCODE_MASK,cause<<CAUSE_EXCCODE_SHIFT);
    set_tlb_context(addr);
    uint16_t jump_offset;
    
    if(!cp0.EXL){
        if(is_bd){
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
    next_PC = jump_base + jump_offset;
}

void VR4300Interpreter::handle_general_exception(uint64_t op_PC, ExceptionCode cause){
    cp0.cause = cp0.set_bits(cp0.cause,CAUSE_EXCCODE_MASK,cause<<CAUSE_EXCCODE_SHIFT);
    
    if(!cp0.EXL){
        if(is_bd){
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
    next_PC = jump_base + 0x0180;
}

bool VR4300Interpreter::check_fpu_exception(uint64_t op_PC){
    uint8_t Cause = ((fpu.FCR31 >> 12) & 0x3F);
    uint8_t Enables = ((fpu.FCR31 >> 7) & 0x1F);
    //wooo magic numbers
    fpu.FCR31 |= (Cause & 0x1F & ~Enables) << 2;
    if( ((Enables | 0x20) & Cause) != 0){
        //TODO fix fpu exceptions
        //cp0.cause = cp0.set_bits(cp0.cause,0x3 << 28,0 << 28); // idk if this should be set only during fpu exceptions or all of them?
        //handle_general_exception(op_PC,FPE);
        return true;
    }
    return false;
}

void VR4300Interpreter::set_tlb_context(uint64_t addr){
    cp0.badVAddr = addr;
    cp0.context  = cp0.set_bits(cp0.context, CONTEXT_BADVPN2_MASK, (addr >> 13) << CONTEXT_BADVPN2_SHIFT);
    cp0.xcontext = cp0.set_bits(cp0.xcontext, XCONTEXT_BADVPN2_MASK, (addr >> 13) << XCONTEXT_BADVPN2_SHIFT);
    cp0.entryHi  = cp0.set_bits(cp0.entryHi, ENTRYHI_VPN2_MASK, (addr >> 13) << ENTRYHI_VPN2_SHIFT);
}

void VR4300Interpreter::update_hardware_interrupt(uint8_t enable, uint8_t value){
    value &= 0x1F;
    value <<= 2;
    enable &= 0x1F;
    enable <<= 2;
    uint8_t prev_inter = cp0.get_bits(cp0.cause,CAUSE_IP_MASK,CAUSE_IP_SHIFT);
    uint8_t new_inter = (prev_inter & ~enable) | (enable & value);
    cp0.cause = cp0.set_bits(cp0.cause, CAUSE_IP_MASK, new_inter << CAUSE_IP_SHIFT);
}

void VR4300Interpreter::dec_random(){
    if(cp0.random == cp0.wired && cp0.wired < 32)
        cp0.random = (cp0.wired > 31)?63:31;
    else{
        cp0.random--;
        if(!cp0.random) cp0.random = (cp0.wired > 31)?63:31;
    }
}

void VR4300Interpreter::inc_count(){
    if(increment){
        if(cp0.count == cp0.compare )
            cp0.cause = cp0.set_bits(cp0.cause, CAUSE_IP_TIMER_MASK, 1 << CAUSE_IP_TIMER_SHIFT);
        cp0.count = (uint32_t)(cp0.count + 1);
        increment = false;
    }else increment = true;
}