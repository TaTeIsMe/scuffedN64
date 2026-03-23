#include "VR4300.h"
#pragma once


void NOP(VR4300& cpu){};

//these totally could have been merged into a singular function and then just put into the correct operation lut entry
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
}
void SH(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void SW(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void SWL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void SWR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
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
}
void SDL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
}
void SDR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.data_addr = (int16_t)op.immediate + op.rs_val;
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
    op.result = ((uint64_t)(int16_t)op.immediate > (int64_t)op.rs_val);
}
void SLTIU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = ((uint64_t)(int16_t)op.immediate > op.rs_val);
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

//this might not work
void SRA(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int32_t)((int32_t)op.rt_val >> op.sa);
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
    op.result = (int32_t)((int32_t)op.rt_val >> (op.rs_val & 0x1F));
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
    int64_t result = (int32_t)op.rs_val * (int32_t)op.rt_val;
    op.result_LO = (int32_t)(result);
    op.result_HI = result>>32;
}
void MULTU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    int64_t result = (uint32_t)op.rs_val * (uint32_t)op.rt_val;
    op.result_LO = (int32_t)(result);
    op.result_HI = result>>32;
}
void DIV(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rt_val == 0){
        op.result_LO = -1;
        op.result_HI = op.rs_val;
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
    op.result = cpu.HI;
}
void MFLO(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = cpu.LO;
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
    __int128_t result = (int64_t)op.rs_val * (int64_t)op.rt_val;
    op.result_LO = result;
    op.result_HI = result>>64;
}
void DMULTU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    __int128_t result = (uint64_t)op.rs_val * (uint64_t)op.rt_val;
    op.result_LO = result;
    op.result_HI = result>>64;
}
void DDIV(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    int64_t quotient = (int64_t)op.rs_val / (int64_t)op.rt_val;
    int64_t remainder = (int64_t)op.rs_val % (int64_t)op.rt_val;
    op.result_LO = quotient;
    op.result_HI = remainder;
}
void DDIVU(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    int64_t quotient = (uint64_t)op.rs_val / (uint64_t)op.rt_val;
    int64_t remainder = (uint64_t)op.rs_val % (uint64_t)op.rt_val;
    op.result_LO = quotient;
    op.result_HI = remainder;
}

//Jump/Branch
//description says it is coupled with high 4 bits, but specifics say it copies everything but bottom 28
void J(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    cpu.PC = (cpu.PC & (~0xFFFFFFF)) | (op.target << 2); 
}
void JAL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = cpu.PC + 4;
    cpu.PC = (cpu.PC & (~0xFFFFFFF)) | (op.target << 2);
}
void JR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    cpu.PC = op.rs_val; 
}
//this instruction writes to reg31 if no rd is specified. i don't quite understand, like, 0 is undefined?? that is a reg number, just a specific one though...
void JALR(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rd)op.data_addr = op.rd;
    op.result = cpu.PC + 4;
    cpu.PC = op.rs_val;
}
void BEQ(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if(op.rs_val == op.rt_val)cpu.PC = target;
}
void BNE(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if(op.rs_val != op.rt_val)cpu.PC = target;
}
void BLEZ(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val <= 0)cpu.PC = target;
}
void BGTZ(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val > 0)cpu.PC = target;
}
void BLTZ(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val < 0)cpu.PC = target;
}
void BGEZ(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val >= 0)cpu.PC = target;
}
void BLTZAL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = cpu.PC + 4;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val < 0)cpu.PC = target;
}
void BGEZAL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = cpu.PC + 4;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val >= 0)cpu.PC = target;
}
void BEQL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if(op.rs_val == op.rt_val)cpu.PC = target;
    else cpu.RF_in = {};
}
void BNEL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if(op.rs_val != op.rt_val)cpu.PC = target;
    else cpu.RF_in = {};
}
void BLEZL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val <= 0)cpu.PC = target;
    else cpu.RF_in = {};
}
void BGTZL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val > 0)cpu.PC = target;
    else cpu.RF_in = {};
}
void BLTZL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val < 0)cpu.PC = target;
    else cpu.RF_in = {};
}
void BGEZL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val >= 0)cpu.PC = target;
    else cpu.RF_in = {};
}
void BLTZALL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = cpu.PC + 4;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val < 0)cpu.PC = target;
    else cpu.RF_in = {};
}
void BGEZALL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = cpu.PC + 4;
    uint64_t target = cpu.PC + (((int16_t)op.immediate) << 2); 
    if((int64_t)op.rs_val >= 0)cpu.PC = target;
    else cpu.RF_in = {};
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
    if(op.rs_val == (int16_t)op.immediate)op.result = 1;
}
void TNEI(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    if(op.rs_val == (int16_t)op.immediate)op.result = 1;
}
void LWCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int16_t)op.immediate + op.rs_val;
}
void SWCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int16_t)op.immediate + op.rs_val;
}
void MTCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (uint32_t)op.rt_val; // i think these shouldn't be sign extended??
}
void MFCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (uint32_t)op.cp_val;
}
//this only does stuff for fpu so make later with fpu
void CTCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
}
//this only does stuff for fpu so make later with fpu
void CFCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
}
//this only does stuff for fpu so make later with fpu
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
    op.result = (int16_t)op.immediate + op.rs_val;
}
void SDCz(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
    op.result = (int16_t)op.immediate + op.rs_val;
}
//this only does stuff for fpu so make later with fpu, hopefully
void BCzTL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
}
//this only does stuff for fpu so make later with fpu, hopefully
void BCzFL(VR4300& cpu){
    VR4300::Operation& op = cpu.EX_in.op;
}
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
}
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
    op.result = (int16_t)op.immediate + op.rs_val;
}

