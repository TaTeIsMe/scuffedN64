#include "VR4300.h"
#include<bit>
#include <cmath>

void NOP(VR4300& cpu){};

//Load/Store
void LB(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void LBU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void LH(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void LHU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void LW(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void LWL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void LWR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void SB(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
    op.result = op.rt_val;
}
void SH(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
    op.result = op.rt_val;
}
void SW(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
    op.result = op.rt_val;
}
void SWL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
    op.result = op.rt_val >> ((op.data_addr & 0x3) * 8);
}
void SWR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
    op.result = op.rt_val << ((3 - (op.data_addr & 0x3)) * 8);
}
void LD(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void LDL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void LDR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void LL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void LLD(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void LWU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void SC(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void SCD(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void SD(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
    op.result = op.rt_val;
}
void SDL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
    op.result = op.rt_val >> ((op.data_addr & 0x7) * 8);
}
void SDR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
    op.result = op.rt_val << ((7-(op.data_addr & 0x7)) * 8);
}
//Computational
void ADDI(VR4300 &cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)((int16_t)op.immediate + op.rs_val);
}
void ADDIU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)((int16_t)op.immediate + op.rs_val);
}
void SLTI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = ((int64_t)(int16_t)op.immediate > (int64_t)op.rs_val);
}
void SLTIU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = ((uint64_t)(int16_t)op.immediate > (uint64_t)op.rs_val);
}
void ANDI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (op.immediate & op.rs_val);
}
void ORI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (op.immediate | op.rs_val);
}
void XORI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (op.immediate ^ op.rs_val);
}

//these sign extentions might have to happen later
void LUI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)(op.immediate<<16);
}
void DADDI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int16_t)op.immediate + op.rs_val;
}
void DADDIU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int16_t)op.immediate + op.rs_val;
}
void ADD(VR4300 &cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)(op.rs_val + op.rt_val);
}
void ADDU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)(op.rs_val + op.rt_val);
}
void SUB(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)(op.rs_val - op.rt_val);
}
void SUBU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)(op.rs_val - op.rt_val);
}
void SLT(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = ((int64_t)op.rt_val > (int64_t)op.rs_val);
}
void SLTU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (op.rt_val > op.rs_val);
}
void AND(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (op.rt_val & op.rs_val);
}
void OR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (op.rt_val | op.rs_val);
}
void XOR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (op.rt_val ^ op.rs_val);
}
void NOR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = ~(op.rt_val | op.rs_val);
}
void DADD(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (op.rs_val + op.rt_val);
}
void DADDU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (op.rs_val + op.rt_val);
}
void DSUB(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (op.rs_val - op.rt_val);
}
void DSUBU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (op.rs_val - op.rt_val);
}
void SLL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)(op.rt_val << op.sa);
}
void SRL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)((uint32_t)op.rt_val >> op.sa);
}
void SRA(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)(op.rt_val >> op.sa);
}
void SLLV(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)(op.rt_val << (op.rs_val & 0x1F));
}
void SRLV(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)((uint32_t)op.rt_val >> (op.rs_val & 0x1F));
}
void SRAV(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)(op.rt_val >> (op.rs_val & 0x1F));
}
void DSLL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.rt_val << op.sa;
}
void DSRL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.rt_val >> op.sa;
}
//not confident in this either
void DSRA(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int64_t)op.rt_val >> op.sa;
}
void DSLLV(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.rt_val << (op.rs_val & 0x3F);
}
void DSRLV(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.rt_val >> (op.rs_val & 0x3F);
}
//this too...
void DSRAV(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int64_t)op.rt_val >> (op.rs_val & 0x3F);
}
void DSLL32(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.rt_val << (32 + op.sa);
}
void DSRL32(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.rt_val >> (32 + op.sa);
}
void DSRA32(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int64_t)op.rt_val >> (32 + op.sa); 
}
void MULT(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    int64_t result = (int64_t)(int32_t)op.rs_val * (int64_t)(int32_t)op.rt_val;
    op.result_LO = (int32_t)(result);
    op.result_HI = (int32_t)(result>>32);
}
void MULTU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t result = (uint64_t)(uint32_t)op.rs_val * (uint64_t)(uint32_t)op.rt_val;
    op.result_LO = (int32_t)(result);
    op.result_HI = (int32_t)(result>>32);
}
void DIV(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rt_val == 0){
        op.result_LO = ((int32_t)op.rs_val<0)?1:-1;
        op.result_HI = op.rs_val;
        return;
    }

    if (op.rs_val == (int32_t)0x80000000 && op.rt_val == -1) {
        op.result_LO = (int32_t)0x80000000;
        op.result_HI = 0;
        return;
    }

    int32_t quotient = (int32_t)op.rs_val / (int32_t)op.rt_val;
    int32_t remainder = (int32_t)op.rs_val % (int32_t)op.rt_val;
    op.result_LO = quotient;
    op.result_HI = remainder;
}
void DIVU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rt_val == 0){
        op.result_LO = -1;
        op.result_HI = op.rs_val;
        return;
    }
    int32_t quotient = (uint32_t)op.rs_val / (uint32_t)op.rt_val;
    int32_t remainder = (uint32_t)op.rs_val % (uint32_t)op.rt_val;
    op.result_LO = quotient;
    op.result_HI = remainder;
}
void MFHI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(cpu.EX_out.op.flags & WRITES_HI)op.result = cpu.EX_out.op.result_HI;
    else if(cpu.DC_out.op.flags & WRITES_HI)op.result = cpu.DC_out.op.result_HI;
    else if(cpu.WB_in.op.flags & WRITES_HI)op.result = cpu.WB_in.op.result_HI;
    else op.result = cpu.HI;
}
void MFLO(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
     if(cpu.EX_out.op.flags & WRITES_LO)op.result = cpu.EX_out.op.result_LO;
    else if(cpu.DC_out.op.flags & WRITES_LO)op.result = cpu.DC_out.op.result_LO;
    else if(cpu.WB_in.op.flags & WRITES_LO)op.result = cpu.WB_in.op.result_LO;
    else op.result = cpu.LO;
}
void MTHI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result_HI = op.rs_val;
}
void MTLO(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result_LO = op.rs_val;
}
void DMULT(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    __int128_t result = (__int128_t)(int64_t)op.rs_val * (__int128_t)(int64_t)op.rt_val;
    op.result_LO = result;
    op.result_HI = result>>64;
}
void DMULTU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    __uint128_t result = (__uint128_t)(uint64_t)op.rs_val * (__uint128_t)(uint64_t)op.rt_val;
    op.result_LO = result;
    op.result_HI = result>>64;
}
void DDIV(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;

    if(op.rt_val == 0){
        op.result_LO = ((int64_t)op.rs_val<0)?1:-1;
        op.result_HI = op.rs_val;
        return;
    }

    if (op.rs_val == (int64_t)0x8000000000000000 && op.rt_val == -1) {
        op.result_LO = (int64_t)0x8000000000000000;
        op.result_HI = 0;
        return;
    }

    int64_t quotient = (int64_t)op.rs_val / (int64_t)op.rt_val;
    int64_t remainder = (int64_t)op.rs_val % (int64_t)op.rt_val;
    op.result_LO = quotient;
    op.result_HI = remainder;
}
void DDIVU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;

    if(op.rt_val == 0){
        op.result_LO = -1;
        op.result_HI = op.rs_val;
        return;
    }

    int64_t quotient = (uint64_t)op.rs_val / (uint64_t)op.rt_val;
    int64_t remainder = (uint64_t)op.rs_val % (uint64_t)op.rt_val;
    op.result_LO = quotient;
    op.result_HI = remainder;
}

//Jump/Branch
//description says it is coupled with high 4 bits, but specifics say it copies everything but bottom 28
void J(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    cpu.PC = ((op.PC + 4) & (~0xFFFFFFF)) | (op.target << 2); // value might be wrong
}
void JAL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.PC + 8;
    cpu.PC = ((op.PC + 4) & (~0xFFFFFFF)) | (op.target << 2);//value might be wrong
}
void JR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    cpu.PC = op.rs_val;
}

void JALR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rd)op.dest_reg = op.rd;
    op.result = op.PC + 8;
    cpu.PC = op.rs_val ;
}
void BEQ(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if(op.rs_val == op.rt_val)cpu.PC = target;
}
void BNE(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if(op.rs_val != op.rt_val)cpu.PC = target;
}
void BLEZ(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val <= 0)cpu.PC = target;
}
void BGTZ(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val > 0)cpu.PC = target;
}
void BLTZ(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val < 0)cpu.PC = target;
}
void BGEZ(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val >= 0)cpu.PC = target;
}
void BLTZAL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.PC + 8;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val < 0)cpu.PC = target;
}
void BGEZAL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.PC + 8;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val >= 0)cpu.PC = target;
}
void BEQL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if(op.rs_val == op.rt_val)cpu.PC = target;
    else cpu.discard_bd = true;
}
void BNEL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if(op.rs_val != op.rt_val)cpu.PC = target;
    else cpu.discard_bd = true;
}
void BLEZL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val <= 0)cpu.PC = target;
    else cpu.discard_bd = true;
}
void BGTZL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val > 0)cpu.PC = target;
    else cpu.discard_bd = true;
}
void BLTZL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val < 0)cpu.PC = target;
    else cpu.discard_bd = true;
}
void BGEZL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val >= 0)cpu.PC = target;
    else cpu.discard_bd = true;
}
void BLTZALL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.PC + 8;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val < 0)cpu.PC = target;
    else cpu.discard_bd = true;
}
void BGEZALL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.PC + 8;
    uint64_t target = 4 +  op.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val >= 0)cpu.PC = target;
    else cpu.discard_bd = true;
}
//Special
void SYNC(VR4300& cpu){
    NOP(cpu);//good thing i checked...
}
void SYSCALL(VR4300& cpu){
    //exception handled in ex logic
}
void BREAK(VR4300& cpu){
    //exception handled in ex logic
}
void TGE(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if((int64_t)op.rs_val >= (int64_t)op.rt_val)op.result = 1;
}
void TGEU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rs_val >= op.rt_val)op.result = 1;
}
void TLT(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if((int64_t)op.rs_val < (int64_t)op.rt_val)op.result = 1;
}
void TLTU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rs_val < op.rt_val)op.result = 1;
}
void TEQ(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rs_val == op.rt_val)op.result = 1;
}
void TNE(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rs_val != op.rt_val)op.result = 1;
}
void TGEI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if((int64_t)op.rs_val >= (int16_t)op.immediate)op.result = 1;
}
void TGEIU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rs_val >= op.immediate)op.result = 1;
}
void TLTI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if((int64_t)op.rs_val < (int16_t)op.immediate)op.result = 1;
}
void TLTIU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rs_val < op.immediate)op.result = 1;
}
//immidiate is sign extended here, it doesn't say in general description, so good i checked...
void TEQI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rs_val == (uint64_t)(int16_t)op.immediate)op.result = 1;
}
void TNEI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rs_val == (uint64_t)(int16_t)op.immediate)op.result = 1;
}
void LWCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}

void SWCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
    op.result = op.cp_val;
}
void MTCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
        op.result = (int32_t)op.rt_val;
}
void MFCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)op.cp_val;
}
void CTCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)op.rt_val & 0x183ffff; //Whatever, this only writes one register anyway so might as well hard code this mask
}
void CFCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)op.cp_val;
}

void COPz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
}
//this only does stuff for fpu so make later with fpu, hopefully
void BCzT(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
}
//this only does stuff for fpu so make later with fpu, hopefully
void BCzF(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
}
void DMTCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.rt_val;
}
void DMFCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.cp_val;
}
void LDCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void SDCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
    op.result = op.cp_val;
}
//this only does stuff for fpu so make later with fpu, hopefully
void BCzTL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    cpu.discard_bd = true;
}
//this only does stuff for fpu so make later with fpu, hopefully
void BCzFL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    cpu.discard_bd = true;
}
/* These are sub categories of MTCz and such
void MTC0(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (uint32_t)op.rt_val;
}
void MFC0(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)op.cp_val;
}
void DMTC0(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.rt_val;
}
void DMFC0(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.cp_val;
}*/
void TLBR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint8_t tlb_index = cpu.cp0.index & 0x3F;
    uint8_t G = (cpu.cp0.TLB[tlb_index][1] >> 12) & 1;
    op.result_pagemask = cpu.cp0.TLB[tlb_index][0];
    op.result_entryHI = cpu.cp0.TLB[tlb_index][1] & ~(1 << 12);
    op.result_entryLO0 = cpu.cp0.TLB[tlb_index][2] + G;
    op.result_entryLO1 = cpu.cp0.TLB[tlb_index][3] + G;
}
void TLBWI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint8_t G = cpu.cp0.entryLo0 & 1;
    op.result_pagemask = cpu.cp0.pageMask;
    op.result_entryHI = cpu.cp0.entryHi;
    op.result_entryLO0 = cpu.cp0.entryLo0 & (~1);
    op.result_entryLO1 = cpu.cp0.entryLo1 & (~1);
}
void TLBWR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint8_t G = cpu.cp0.entryLo0 & 1;
    op.result_pagemask = cpu.cp0.pageMask;
    op.result_entryHI = cpu.cp0.entryHi;
    op.result_entryLO0 = cpu.cp0.entryLo0 & (~1);
    op.result_entryLO1 = cpu.cp0.entryLo1 & (~1);
}

//thsi will need to write the index reg
void TLBP(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    for (int i = 0; i < 32; i++)
    {
        if((cpu.cp0.TLB[i][1] & ~(1 << 12)) == cpu.cp0.entryHi){
            op.result = (int32_t)((1<<31) + i);
            return;
        }
    }
    cpu.cp0.entryHi = cpu.cp0.set_bits(cpu.cp0.entryHi,1<<31,0);
}
void ERET(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    cpu.discard_bd = true;
    if(cpu.cp0.status & STATUS_ERL_MASK){
        cpu.PC = cpu.cp0.errorEPC;
        cpu.cp0.status = cpu.cp0.set_bits(cpu.cp0.status,STATUS_EXL_MASK,0);
    }else{
        cpu.PC = cpu.cp0.EPC;
        cpu.cp0.status = cpu.cp0.set_bits(cpu.cp0.status,STATUS_EXL_MASK,0);
    }
    cpu.LLBit = 0;

}
void CACHE(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}


//these would have been so much nicer to split into sub formats
void CVTSfmt(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t fpr_val = op.cp_val;
    uint8_t fmt = op.rs;
    cpu.fpu.FCR31 &= ~(0x3F << 12);
    
    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    float converted;
    switch (fmt)
    {
    case 16:{
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        converted = (float) interpreted;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);
        unimplemented = true;
        break;
    }
    case 17:{
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        converted = (float) interpreted;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);
        inexact = (converted != interpreted);
        overflow = std::fabs(interpreted) > std::numeric_limits<float>::max();
        underflow = std::fabs(interpreted) < std::numeric_limits<float>::min() && interpreted != 0.0;
        unimplemented = (std::fpclassify(interpreted) == FP_SUBNORMAL);
        invalid = std::isnan(interpreted) && !((fpr_val >> 51) & 1); // make sure this should be ! before checking the quiet bit
        break;
    }
    case 20:{
        int32_t interpreted = (int32_t)fpr_val;
        converted = (float) interpreted;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);
        inexact = (converted != interpreted);
        break;
    }
    case 21:{
        int64_t interpreted = (int64_t)fpr_val;
        converted = (float) interpreted;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);
        inexact = (converted != interpreted);
        overflow = std::fabs(interpreted) > std::numeric_limits<float>::max();
        unimplemented = ((interpreted & 0xEFFFFFFFFFFFFFFF) >> 55) != 0;
        break;
    }
    default:
        break;
    }
    cpu.fpu.FCR31 |= (inexact << CAUSE_INEXACT_SHIFT);
    cpu.fpu.FCR31 |= (overflow << CAUSE_OVERFLOW_SHIFT);
    cpu.fpu.FCR31 |= (underflow << CAUSE_UNDERFLOW_SHIFT);
    cpu.fpu.FCR31 |= (invalid << CAUSE_INVALID_SHIFT);
    cpu.fpu.FCR31 |= (unimplemented << CAUSE_UNIMPLEMENTED_SHIFT);
    if((cpu.fpu.FCR31 >> 12) & 0x3F) cpu.EX_out.fire_fpu_exception = 1;
}

void CVTDfmt(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t fpr_val = op.cp_val;
    uint8_t fmt = op.rs;
    double converted;
    switch (fmt)
    {
    case 16:{
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        converted = (double) interpreted;
        op.result = std::bit_cast<uint64_t>(converted);
        break;
    }
    case 17:{
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        converted = (double) interpreted;
        op.result = std::bit_cast<uint64_t>(converted);
        break;
    }
    case 20:{
        int32_t interpreted = (int32_t)fpr_val;
        converted = (double) interpreted;
        op.result = std::bit_cast<uint64_t>(converted);
        break;
    }
    case 21:{
        int64_t interpreted = (int64_t)fpr_val;
        converted = (double) interpreted;
        op.result = std::bit_cast<uint64_t>(converted);
        break;
    }
    default:
        break;
    }
};
void CVTLfmt(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t fpr_val = op.cp_val;
    uint8_t fmt = op.rs;
    int64_t converted;
    switch (fmt)
    {
    case 16:{
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        converted = (int64_t) interpreted;
        op.result = std::bit_cast<uint64_t>(converted);
        break;
    }
    case 17:{
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        converted = (int64_t) interpreted;
        op.result = std::bit_cast<uint64_t>(converted);
        break;
    }
    case 20:{
        int32_t interpreted = (int32_t)fpr_val;
        converted = (int64_t) interpreted;
        op.result = converted;
        break;
    }
    case 21:{
        int64_t interpreted = (int64_t)fpr_val;
        converted = (int64_t) interpreted;
        op.result = converted;
        break;
    }
    default:
        break;
    }
};
void CVTWfmt(VR4300& cpu){
        VR4300::Operation& op = cpu.EX_in.op;
    uint64_t fpr_val = op.cp_val;
    uint8_t fmt = op.rs;
    int32_t converted;
    switch (fmt)
    {
    case 16:{
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        converted = (int32_t) interpreted;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);
        break;
    }
    case 17:{
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        converted = (int32_t) interpreted;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);
        break;
    }
    case 20:{
        int32_t interpreted = (int32_t)fpr_val;
        converted = (int32_t) interpreted;
        op.result = (uint64_t)converted;
        break;
    }
    case 21:{
        int64_t interpreted = (int64_t)fpr_val;
        converted = (int32_t) interpreted;
        op.result = (uint64_t) converted;
        break;
    }
    default:
        break;
    }
};

void ROUNDLfmt(VR4300& cpu){};
void ROUNDWfmt(VR4300& cpu){};
void TRUNCLfmt(VR4300& cpu){};
void TRUNCWfmt(VR4300& cpu){};
void CEILLfmt(VR4300& cpu){};
void CEILWfmt(VR4300& cpu){};
void FLOORLfmt(VR4300& cpu){};
void FLOORWfmt(VR4300& cpu){};

//fpu computational
void ADDfmt(VR4300& cpu){};
void SUBfmt(VR4300& cpu){};
void MULfmt(VR4300& cpu){};
void DIVfmt(VR4300& cpu){};
void ABSfmt(VR4300& cpu){};
void MOVfmt(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = op.cp_val;
};
void NEGfmt(VR4300& cpu){};
void SQRTfmt(VR4300& cpu){};

//fpu compare
void Ccondfmt(VR4300& cpu){};

VR4300::OperationTemplate primary_op_lut[64]{
/*00*/ {nullptr,0,0,OpType::SPECIAL},                       // SPECIAL
/*01*/ {nullptr,0,0,OpType::REGIMM},                        // REGIMM
/*02*/ {J, CAUSES_BRANCH_DELAY, 0, OpType::J},
/*03*/ {JAL, CAUSES_BRANCH_DELAY | STORES_IN_31 | WRITES_REG,0,OpType::JAL},
/*04*/ {BEQ, CAUSES_BRANCH_DELAY,0,OpType::BEQ},
/*05*/ {BNE, CAUSES_BRANCH_DELAY,0,OpType::BNE},
/*06*/ {BLEZ, CAUSES_BRANCH_DELAY,0,OpType::BLEZ},
/*07*/ {BGTZ, CAUSES_BRANCH_DELAY,0,OpType::BGTZ},

/*08*/ {ADDI, WRITES_REG | STORES_IN_RT | CAUSES_OVERFLOW_EXCEPTION,0,OpType::ADDI},
/*09*/ {ADDIU, WRITES_REG | STORES_IN_RT,0,OpType::ADDIU},
/*0A*/ {SLTI, WRITES_REG | STORES_IN_RT,0,OpType::SLTI},
/*0B*/ {SLTIU, WRITES_REG | STORES_IN_RT,0,OpType::SLTIU},
/*0C*/ {ANDI, WRITES_REG | STORES_IN_RT,0,OpType::ANDI},
/*0D*/ {ORI, WRITES_REG | STORES_IN_RT,0,OpType::ORI},
/*0E*/ {XORI, WRITES_REG | STORES_IN_RT,0,OpType::XORI},
/*0F*/ {LUI, WRITES_REG | STORES_IN_RT ,0,OpType::LUI}, //technically not IS_LOAD since it doesn't access memory

/*10*/ {COPz, 0,0,OpType::COPz},
/*11*/ {COPz, 0,0,OpType::COPz},
/*12*/ {},
/*13*/ {},

/*14*/ {BEQL, CAUSES_BRANCH_DELAY,0,OpType::BEQL},
/*15*/ {BNEL, CAUSES_BRANCH_DELAY,0,OpType::BNEL},
/*16*/ {BLEZL, CAUSES_BRANCH_DELAY,0,OpType::BLEZL},
/*17*/ {BGTZL, CAUSES_BRANCH_DELAY,0,OpType::BGTZL},

/*18*/ {DADDI, WRITES_REG | STORES_IN_RT | CAUSES_OVERFLOW_EXCEPTION, 0, OpType::DADDI},
/*19*/ {DADDIU, WRITES_REG | STORES_IN_RT, 0, OpType::DADDIU},
/*1A*/ {LDL, IS_LOAD | LEFT_ACCESS | ACCESSES_DOUBLE_WORD | WRITES_REG | STORES_IN_RT, 0, OpType::LDL},
/*1B*/ {LDR, IS_LOAD | RIGHT_ACCESS | ACCESSES_DOUBLE_WORD | WRITES_REG | STORES_IN_RT, 0, OpType::LDR},

/*1C*/ {},
/*1D*/ {},
/*1E*/ {},
/*1F*/ {},

/*20*/ {LB, IS_LOAD | ACCESSES_BYTE | SIGN_EXTENDED | WRITES_REG | STORES_IN_RT | SIGN_EXTENDED,0,OpType::LB},
/*21*/ {LH, IS_LOAD | ACCESSES_HALF_WORD | SIGN_EXTENDED | WRITES_REG | STORES_IN_RT | SIGN_EXTENDED,0,OpType::LH},
/*22*/ {LWL, IS_LOAD | ACCESSES_WORD | WRITES_REG | STORES_IN_RT | LEFT_ACCESS,0,OpType::LWL},
/*23*/ {LW, IS_LOAD | ACCESSES_WORD | SIGN_EXTENDED | WRITES_REG | STORES_IN_RT,0,OpType::LW},
/*24*/ {LBU, IS_LOAD | ACCESSES_BYTE | WRITES_REG | STORES_IN_RT,0,OpType::LBU},
/*25*/ {LHU, IS_LOAD | ACCESSES_HALF_WORD | WRITES_REG | STORES_IN_RT,0,OpType::LHU},
/*26*/ {LWR, IS_LOAD | ACCESSES_WORD | WRITES_REG | STORES_IN_RT | RIGHT_ACCESS,0,OpType::LWR},
/*27*/ {LWU, IS_LOAD | ACCESSES_WORD | WRITES_REG | STORES_IN_RT,0,OpType::LWU},

/*28*/ {SB, IS_STORE | ACCESSES_BYTE,0,OpType::SB},
/*29*/ {SH, IS_STORE | ACCESSES_HALF_WORD,0,OpType::SH},
/*2A*/ {SWL, IS_STORE | ACCESSES_WORD | LEFT_ACCESS,0,OpType::SWL},
/*2B*/ {SW, IS_STORE | ACCESSES_WORD,0,OpType::SW},
/*2C*/ {SDL, IS_STORE | ACCESSES_DOUBLE_WORD | LEFT_ACCESS, 0, OpType::SDL},
/*2D*/ {SDR, IS_STORE | ACCESSES_DOUBLE_WORD | RIGHT_ACCESS, 0, OpType::SDR},
/*2E*/ {SWR, IS_STORE | ACCESSES_WORD | RIGHT_ACCESS,0,OpType::SWR},
/*2F*/ {CACHE, 0, 0, OpType::CACHE},

/*30*/ {LL, IS_LOAD | ACCESSES_WORD | WRITES_REG | STORES_IN_RT | ATOMIC,0, OpType::LL},
/*31*/ {LWCz, IS_LOAD | ACCESSES_WORD | WRITES_REG | SIGN_EXTENDED | STORES_IN_RT | WRITES_CP,0, OpType::LWCz},
/*32*/ {LWCz, IS_LOAD | ACCESSES_WORD | WRITES_REG | SIGN_EXTENDED | STORES_IN_RT | WRITES_CP,0, OpType::LWCz},
/*33*/ {},

/*34*/ {LLD, IS_LOAD | ACCESSES_DOUBLE_WORD | WRITES_REG | STORES_IN_RT | ATOMIC,0,OpType::LLD},
/*35*/ {LDCz, WRITES_REG | ACCESSES_DOUBLE_WORD | IS_LOAD | SIGN_EXTENDED | WRITES_CP | STORES_IN_RT ,0,OpType::LDCz},
/*36*/ {LDCz, WRITES_REG | ACCESSES_DOUBLE_WORD | IS_LOAD | SIGN_EXTENDED| WRITES_CP | STORES_IN_RT ,0,OpType::LDCz},
/*37*/ {LD, IS_LOAD | ACCESSES_DOUBLE_WORD | SIGN_EXTENDED | WRITES_REG | STORES_IN_RT,0,OpType::LD},

/*38*/ {SC, IS_STORE | ACCESSES_WORD | STORES_IN_RT | ATOMIC,0,OpType::SC}, //this technically writes reg but it's a special case since result has data to be stored
/*39*/ {SWCz, IS_STORE | READS_CP | ACCESSES_WORD,0,OpType::SWCz},
/*3A*/ {SWCz, IS_STORE | READS_CP | ACCESSES_WORD,0,OpType::SWCz},
/*3B*/ {},

/*3C*/ {SCD, IS_STORE | ACCESSES_DOUBLE_WORD | STORES_IN_RT | ATOMIC, 0, OpType::SCD}, //this technically writes reg but it's a s[ecial case since result has data to be stored
/*3D*/ {SDCz, IS_STORE | READS_CP | ACCESSES_DOUBLE_WORD,0,OpType::SDCz},
/*3E*/ {SDCz, IS_STORE | READS_CP | ACCESSES_DOUBLE_WORD,0,OpType::SDCz},
/*3F*/ {SD, IS_STORE | ACCESSES_DOUBLE_WORD,0,OpType::SD}
};
VR4300::OperationTemplate special_op_lut[64]{
/*00*/ {SLL, WRITES_REG | STORES_IN_RD,0,OpType::SLL},
/*01*/ {},
/*02*/ {SRL, WRITES_REG | STORES_IN_RD,0,OpType::SRL},
/*03*/ {SRA, WRITES_REG | STORES_IN_RD,0,OpType::SRA},
/*04*/ {SLLV, WRITES_REG | STORES_IN_RD,0,OpType::SLLV},
/*05*/ {},
/*06*/ {SRLV, WRITES_REG | STORES_IN_RD,0,OpType::SRLV},
/*07*/ {SRAV, WRITES_REG | STORES_IN_RD,0,OpType::SRAV},

/*08*/ {JR, CAUSES_BRANCH_DELAY,0,OpType::JR},
/*09*/ {JALR, CAUSES_BRANCH_DELAY | STORES_IN_RD | WRITES_REG,0,OpType::JALR},
/*0A*/ {},
/*0B*/ {},
/*0C*/ {SYSCALL, CAUSED_EXCEPTION,0,OpType::SYSCALL},
/*0D*/ {BREAK, CAUSED_EXCEPTION,0,OpType::BREAK},
/*0E*/ {},
/*0F*/ {SYNC, 0, 0, OpType::SYNC},

/*10*/ {MFHI, WRITES_REG | STORES_IN_RD,0,OpType::MFHI},
/*11*/ {MTHI, WRITES_HI, 0, OpType::MTHI},
/*12*/ {MFLO, WRITES_REG | STORES_IN_RD,0,OpType::MFLO},
/*13*/ {MTLO, WRITES_LO, 0, OpType::MTLO},
/*14*/ {DSLLV, WRITES_REG | STORES_IN_RD, 0, OpType::DSLLV},
/*15*/ {},
/*16*/ {DSRLV, WRITES_REG | STORES_IN_RD, 0, OpType::DSRLV},
/*17*/ {DSRAV, WRITES_REG | STORES_IN_RD, 0, OpType::DSRAV},

/*18*/ {MULT, WRITES_HI | WRITES_LO, 5,OpType::MULT},
/*19*/ {MULTU, WRITES_HI | WRITES_LO, 5,OpType::MULTU},
/*1A*/ {DIV, WRITES_HI | WRITES_LO, 37,OpType::DIV},
/*1B*/ {DIVU, WRITES_HI | WRITES_LO, 37,OpType::DIVU},
/*1C*/ {DMULT,WRITES_HI | WRITES_LO,8,OpType::DMULT},
/*1D*/ {DMULTU,WRITES_HI | WRITES_LO,8,OpType::DMULTU},
/*1E*/ {DDIV, WRITES_HI | WRITES_LO, 69, OpType::DDIV},
/*1F*/ {DDIVU, WRITES_HI | WRITES_LO, 69, OpType::DDIVU},

/*20*/ {ADD, WRITES_REG | STORES_IN_RD | CAUSES_OVERFLOW_EXCEPTION,0,OpType::ADD},
/*21*/ {ADDU, WRITES_REG | STORES_IN_RD,0,OpType::ADDU},
/*22*/ {SUB, WRITES_REG | STORES_IN_RD | CAUSES_OVERFLOW_EXCEPTION,0,OpType::SUB},
/*23*/ {SUBU, WRITES_REG | STORES_IN_RD,0,OpType::SUBU},
/*24*/ {AND, WRITES_REG | STORES_IN_RD,0,OpType::AND},
/*25*/ {OR, WRITES_REG | STORES_IN_RD,0,OpType::OR},
/*26*/ {XOR, WRITES_REG | STORES_IN_RD,0,OpType::XOR},
/*27*/ {NOR, WRITES_REG | STORES_IN_RD,0,OpType::NOR},

/*28*/ {},
/*29*/ {},
/*2A*/ {SLT, WRITES_REG | STORES_IN_RD,0,OpType::SLT},
/*2B*/ {SLTU, WRITES_REG | STORES_IN_RD,0,OpType::SLTU},
/*2C*/ {DADD,  WRITES_REG | STORES_IN_RD | CAUSES_OVERFLOW_EXCEPTION, 0, OpType::DADD},
/*2D*/ {DADDU, WRITES_REG | STORES_IN_RD, 0, OpType::DADDU},
/*2E*/ {DSUB,  WRITES_REG | STORES_IN_RD | CAUSES_OVERFLOW_EXCEPTION, 0, OpType::DSUB},
/*2F*/ {DSUBU, WRITES_REG | STORES_IN_RD, 0, OpType::DSUBU},

/*30*/ {TGE,  IS_TRAP, 0, OpType::TGE},
/*31*/ {TGEU, IS_TRAP, 0, OpType::TGEU},
/*32*/ {TLT,  IS_TRAP, 0, OpType::TLT},
/*33*/ {TLTU, IS_TRAP, 0, OpType::TLTU},
/*34*/ {TEQ,  IS_TRAP, 0, OpType::TEQ},
/*35*/ {},
/*36*/ {TNE,  IS_TRAP, 0, OpType::TNE},
/*37*/ {},

/*38*/ {DSLL,   WRITES_REG | STORES_IN_RD, 0, OpType::DSLL},
/*39*/ {},
/*3A*/ {DSRL,   WRITES_REG | STORES_IN_RD, 0, OpType::DSRL},
/*3B*/ {DSRA,   WRITES_REG | STORES_IN_RD, 0, OpType::DSRA},
/*3C*/ {DSLL32, WRITES_REG | STORES_IN_RD, 0, OpType::DSLL32},
/*3D*/ {},
/*3E*/ {DSRL32, WRITES_REG | STORES_IN_RD, 0, OpType::DSRL32},
/*3F*/ {DSRA32, WRITES_REG | STORES_IN_RD, 0, OpType::DSRA32},
};
VR4300::OperationTemplate regimm_op_lut[32]{
/*00*/ {BLTZ, CAUSES_BRANCH_DELAY,0,OpType::BLTZ},
/*01*/ {BGEZ, CAUSES_BRANCH_DELAY,0,OpType::BGEZ},
/*02*/ {BLTZL, CAUSES_BRANCH_DELAY,0,OpType::BLTZL},
/*03*/ {BGEZL, CAUSES_BRANCH_DELAY,0,OpType::BGEZL},

/*04*/ {},
/*05*/ {},
/*06*/ {},
/*07*/ {},

/*08*/ {TGEI, IS_TRAP,0,OpType::TGEI},
/*09*/ {TGEIU, IS_TRAP,0,OpType::TGEIU},
/*0A*/ {TLTI, IS_TRAP,0,OpType::TLTI},
/*0B*/ {TLTIU, IS_TRAP,0,OpType::TLTIU},
/*0C*/ {TEQI, IS_TRAP,0,OpType::TEQI},
/*0D*/ {},
/*0E*/ {TNEI, IS_TRAP,0,OpType::TNEI},
/*0F*/ {},

/*10*/ {BLTZAL, CAUSES_BRANCH_DELAY | STORES_IN_31 | WRITES_REG,0,OpType::BLTZAL},
/*11*/ {BGEZAL, CAUSES_BRANCH_DELAY | STORES_IN_31 | WRITES_REG,0,OpType::BGEZAL},
/*12*/ {BLTZALL, CAUSES_BRANCH_DELAY | STORES_IN_31 | WRITES_REG, 0, OpType::BLTZALL},
/*13*/ {BGEZALL, CAUSES_BRANCH_DELAY | STORES_IN_31 | WRITES_REG, 0, OpType::BGEZALL},

};

VR4300::OperationTemplate COPzrs_op_lut[32]{
/*00*/ {MFCz, WRITES_REG | STORES_IN_RT | READS_CP | ACCESSES_WORD | CPZ,0,OpType::MFCz},
/*01*/ {DMFCz, WRITES_REG | STORES_IN_RT | READS_CP | ACCESSES_DOUBLE_WORD | CPZ,0,OpType::DMFCz},
/*02*/ {CFCz, WRITES_REG | STORES_IN_RT | READS_CP | CPControl | CPZ,0,OpType::CFCz},
/*03*/ {},
/*04*/ {MTCz, WRITES_REG | WRITES_CP | STORES_IN_RD | ACCESSES_WORD | CPZ,0,OpType::MTCz},
/*05*/ {DMTCz, WRITES_REG | WRITES_CP | STORES_IN_RD | ACCESSES_DOUBLE_WORD | CPZ,0,OpType::DMTCz},
/*06*/ {CTCz, WRITES_REG | WRITES_CP | CPControl | CPZ,0,OpType::CTCz},
/*07*/ {},

/*08*/ {},
/*09*/ {},
/*0A*/ {},
/*0B*/ {},
/*0C*/ {},
/*0D*/ {},
/*0E*/ {},
/*0F*/ {},
};
VR4300::OperationTemplate COPzrt_op_lut[32]{
/*00*/ {BCzF, CAUSES_BRANCH_DELAY | CPZ,0,OpType::BCzF},
/*01*/ {BCzT, CAUSES_BRANCH_DELAY | CPZ,0,OpType::BCzT},
/*02*/ {BCzFL, CAUSES_BRANCH_DELAY | CPZ,0,OpType::BCzFL},
/*03*/ {BCzTL, CAUSES_BRANCH_DELAY | CPZ,0,OpType::BCzTL},

/*04*/ {},
/*05*/ {},
/*06*/ {},
/*07*/ {},

/*08*/ {},
/*09*/ {},
/*0A*/ {},
/*0B*/ {},
/*0C*/ {},
/*0D*/ {},
/*0E*/ {},
/*0F*/ {},

/*10*/ {},
/*11*/ {},
/*12*/ {},
/*13*/ {},
/*14*/ {},
/*15*/ {},
/*16*/ {},
/*17*/ {},

/*18*/ {},
/*19*/ {},
/*1A*/ {},
/*1B*/ {},
/*1C*/ {},
/*1D*/ {},
/*1E*/ {},
/*1F*/ {}
};
VR4300::OperationTemplate CP0_op_lut[32]{
/*00*/ {},
/*01*/ {TLBR, CPZ,0,OpType::TLBR},
/*02*/ {TLBWI, CPZ,0,OpType::TLBWI},
/*03*/ {},
/*04*/ {},
/*05*/ {},
/*06*/ {TLBWR, CPZ,0,OpType::TLBWR},
/*07*/ {},

/*08*/ {TLBP, CPZ,0,OpType::TLBP},
/*09*/ {},
/*0A*/ {},
/*0B*/ {},
/*0C*/ {},
/*0D*/ {},
/*0E*/ {},
/*0F*/ {},

/*10*/ {},
/*11*/ {},
/*12*/ {},
/*13*/ {},
/*14*/ {},
/*15*/ {},
/*16*/ {},
/*17*/ {},

/*18*/ {ERET, CAUSES_BRANCH_DELAY | CPZ,0,OpType::ERET},
/*19*/ {},
/*1A*/ {},
/*1B*/ {},
/*1C*/ {},
/*1D*/ {},
/*1E*/ {},
/*1F*/ {}
};

VR4300::OperationTemplate CP1_op_lut[64]{
/*00*/ {NOP},
/*01*/ {NOP},
/*02*/ {NOP},
/*03*/ {NOP},
/*04*/ {NOP},
/*05*/ {NOP},
/*06*/ {MOVfmt, READS_CP | WRITES_CP | WRITES_REG | STORES_IN_SA | ACCESSES_DOUBLE_WORD | CPZ,0, OpType::MOVfmt}, // remember to handle access size correctly here
/*07*/ {NOP},

/*08*/ {NOP},
/*09*/ {NOP},
/*0A*/ {NOP},
/*0B*/ {NOP},
/*0C*/ {NOP},
/*0D*/ {NOP},
/*0E*/ {NOP},
/*0F*/ {NOP},

/*10*/ {NOP},
/*11*/ {NOP},
/*12*/ {NOP},
/*13*/ {NOP},
/*14*/ {NOP},
/*15*/ {NOP},
/*16*/ {NOP},
/*17*/ {NOP},

/*18*/ {NOP},
/*19*/ {NOP},
/*1A*/ {NOP},
/*1B*/ {NOP},
/*1C*/ {NOP},
/*1D*/ {NOP},
/*1E*/ {NOP},
/*1F*/ {NOP},

/*20*/ {CVTSfmt,WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | ACCESSES_DOUBLE_WORD | CPZ,0,OpType::CVTSfmt},
/*21*/ {CVTDfmt,WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | ACCESSES_DOUBLE_WORD | CPZ,0,OpType::CVTDfmt},
/*22*/ {NOP},
/*23*/ {NOP},
/*24*/ {CVTWfmt,WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | ACCESSES_DOUBLE_WORD | CPZ,0,OpType::CVTWfmt},
/*25*/ {CVTLfmt,WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | ACCESSES_DOUBLE_WORD | CPZ,0,OpType::CVTLfmt},
/*26*/ {NOP},
/*27*/ {NOP},

/*28*/ {NOP},
/*29*/ {NOP},
/*2A*/ {NOP},
/*2B*/ {NOP},
/*2C*/ {NOP},
/*2D*/ {NOP},
/*2E*/ {NOP},
/*2F*/ {NOP},

/*30*/ {NOP},
/*31*/ {NOP},
/*32*/ {NOP},
/*33*/ {NOP},
/*34*/ {NOP},
/*35*/ {NOP},
/*36*/ {NOP},
/*37*/ {NOP},

/*38*/ {NOP},
/*39*/ {NOP},
/*3A*/ {NOP},
/*3B*/ {NOP},
/*3C*/ {NOP},
/*3D*/ {NOP},
/*3E*/ {NOP},
/*3F*/ {NOP},

};