#include "VR4300.h"
#include <bit>
#include <cmath>
#include <fenv.h>

#pragma STDC FENV_ACCESS ON

void NOP(VR4300 &cpu) {};

// Load/Store
void LB(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void LBU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void LH(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void LHU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void LW(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void LWL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void LWR(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void SB(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
    op.result = op.rt_val;
}
void SH(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
    op.result = op.rt_val;
}
void SW(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
    op.result = op.rt_val;
}
void SWL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
    op.result = op.rt_val >> ((op.data_addr & 0x3) * 8);
}
void SWR(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
    op.result = op.rt_val << ((3 - (op.data_addr & 0x3)) * 8);
}
void LD(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void LDL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void LDR(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void LL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void LLD(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void LWU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void SC(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void SCD(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void SD(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
    op.result = op.rt_val;
}
void SDL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
    op.result = op.rt_val >> ((op.data_addr & 0x7) * 8);
}
void SDR(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
    op.result = op.rt_val << ((7 - (op.data_addr & 0x7)) * 8);
}
// Computational
void ADDI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)((int16_t)(op.opcode & 0xFFFF) + op.rs_val);
}
void ADDIU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)((int16_t)(op.opcode & 0xFFFF) + op.rs_val);
}
void SLTI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = ((int64_t)(int16_t)(op.opcode & 0xFFFF) > (int64_t)op.rs_val);
}
void SLTIU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = ((uint64_t)(int16_t)(op.opcode & 0xFFFF) > (uint64_t)op.rs_val);
}
void ANDI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = ((op.opcode & 0xFFFF) & op.rs_val);
}
void ORI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = ((op.opcode & 0xFFFF) | op.rs_val);
}
void XORI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = ((op.opcode & 0xFFFF) ^ op.rs_val);
}

// these sign extentions might have to happen later
void LUI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)((op.opcode & 0xFFFF) << 16);
}
void DADDI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void DADDIU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void ADD(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)(op.rs_val + op.rt_val);
}
void ADDU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)(op.rs_val + op.rt_val);
}
void SUB(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)(op.rs_val - op.rt_val);
}
void SUBU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)(op.rs_val - op.rt_val);
}
void SLT(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = ((int64_t)op.rt_val > (int64_t)op.rs_val);
}
void SLTU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (op.rt_val > op.rs_val);
}
void AND(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (op.rt_val & op.rs_val);
}
void OR(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (op.rt_val | op.rs_val);
}
void XOR(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (op.rt_val ^ op.rs_val);
}
void NOR(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = ~(op.rt_val | op.rs_val);
}
void DADD(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (op.rs_val + op.rt_val);
}
void DADDU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (op.rs_val + op.rt_val);
}
void DSUB(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (op.rs_val - op.rt_val);
}
void DSUBU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (op.rs_val - op.rt_val);
}
void SLL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)(op.rt_val << op.sa);
}
void SRL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)((uint32_t)op.rt_val >> op.sa);
}
void SRA(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)(op.rt_val >> op.sa);
}
void SLLV(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)(op.rt_val << (op.rs_val & 0x1F));
}
void SRLV(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)((uint32_t)op.rt_val >> (op.rs_val & 0x1F));
}
void SRAV(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)(op.rt_val >> (op.rs_val & 0x1F));
}
void DSLL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.rt_val << op.sa;
}
void DSRL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.rt_val >> op.sa;
}
// not confident in this either
void DSRA(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int64_t)op.rt_val >> op.sa;
}
void DSLLV(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.rt_val << (op.rs_val & 0x3F);
}
void DSRLV(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.rt_val >> (op.rs_val & 0x3F);
}
// this too...
void DSRAV(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int64_t)op.rt_val >> (op.rs_val & 0x3F);
}
void DSLL32(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.rt_val << (32 + op.sa);
}
void DSRL32(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.rt_val >> (32 + op.sa);
}
void DSRA32(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int64_t)op.rt_val >> (32 + op.sa);
}
void MULT(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    int64_t result = (int64_t)(int32_t)op.rs_val * (int64_t)(int32_t)op.rt_val;
    op.result_LO = (int32_t)(result);
    op.result_HI = (int32_t)(result >> 32);
}
void MULTU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t result = (uint64_t)(uint32_t)op.rs_val * (uint64_t)(uint32_t)op.rt_val;
    op.result_LO = (int32_t)(result);
    op.result_HI = (int32_t)(result >> 32);
}
void DIV(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if (op.rt_val == 0)
    {
        op.result_LO = ((int32_t)op.rs_val < 0) ? 1 : -1;
        op.result_HI = op.rs_val;
        return;
    }

    if (op.rs_val == (int32_t)0x80000000 && op.rt_val == -1)
    {
        op.result_LO = (int32_t)0x80000000;
        op.result_HI = 0;
        return;
    }

    int32_t quotient = (int32_t)op.rs_val / (int32_t)op.rt_val;
    int32_t remainder = (int32_t)op.rs_val % (int32_t)op.rt_val;
    op.result_LO = quotient;
    op.result_HI = remainder;
}
void DIVU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if (op.rt_val == 0)
    {
        op.result_LO = -1;
        op.result_HI = op.rs_val;
        return;
    }
    int32_t quotient = (uint32_t)op.rs_val / (uint32_t)op.rt_val;
    int32_t remainder = (uint32_t)op.rs_val % (uint32_t)op.rt_val;
    op.result_LO = quotient;
    op.result_HI = remainder;
}
void MFHI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    // if(cpu.EX_out.op.flags & WRITES_HI)op.result = cpu.EX_out.op.result_HI;
    if (cpu.DC_in.op->tmplt->writes_hi)
        op.result = cpu.DC_in.op->result_HI;
    else if (cpu.WB_in.op->tmplt->writes_hi)
        op.result = cpu.WB_in.op->result_HI;
    else
        op.result = cpu.HI;
}
void MFLO(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    // if(cpu.EX_out.op.flags & WRITES_LO)op.result = cpu.EX_out.op.result_LO;
    if (cpu.DC_in.op->tmplt->writes_lo)
        op.result = cpu.DC_in.op->result_LO;
    else if (cpu.WB_in.op->tmplt->writes_lo)
        op.result = cpu.WB_in.op->result_LO;
    else
        op.result = cpu.LO;
}
void MTHI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result_HI = op.rs_val;
}
void MTLO(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result_LO = op.rs_val;
}
void DMULT(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    __int128_t result = (__int128_t)(int64_t)op.rs_val * (__int128_t)(int64_t)op.rt_val;
    op.result_LO = result;
    op.result_HI = result >> 64;
}
void DMULTU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    __uint128_t result = (__uint128_t)(uint64_t)op.rs_val * (__uint128_t)(uint64_t)op.rt_val;
    op.result_LO = result;
    op.result_HI = result >> 64;
}
void DDIV(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;

    if (op.rt_val == 0)
    {
        op.result_LO = ((int64_t)op.rs_val < 0) ? 1 : -1;
        op.result_HI = op.rs_val;
        return;
    }

    if (op.rs_val == (int64_t)0x8000000000000000 && op.rt_val == -1)
    {
        op.result_LO = (int64_t)0x8000000000000000;
        op.result_HI = 0;
        return;
    }

    int64_t quotient = (int64_t)op.rs_val / (int64_t)op.rt_val;
    int64_t remainder = (int64_t)op.rs_val % (int64_t)op.rt_val;
    op.result_LO = quotient;
    op.result_HI = remainder;
}
void DDIVU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;

    if (op.rt_val == 0)
    {
        op.result_LO = -1;
        op.result_HI = op.rs_val;
        return;
    }

    int64_t quotient = (uint64_t)op.rs_val / (uint64_t)op.rt_val;
    int64_t remainder = (uint64_t)op.rs_val % (uint64_t)op.rt_val;
    op.result_LO = quotient;
    op.result_HI = remainder;
}

// Jump/Branch
// description says it is coupled with high 4 bits, but specifics say it copies everything but bottom 28
void J(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    cpu.PC = ((op.PC + 4) & (~0xFFFFFFF)) | ((op.opcode & 0x3FFFFFF) << 2); // value might be wrong
}
void JAL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.PC + 8;
    cpu.PC = ((op.PC + 4) & (~0xFFFFFFF)) | ((op.opcode & 0x3FFFFFF) << 2); // value might be wrong
}
void JR(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    cpu.PC = op.rs_val;
}

void JALR(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if (op.rd)
        op.dest_reg = op.rd;
    op.result = op.PC + 8;
    cpu.GPR[op.dest_reg] = op.result;
    cpu.PC = op.rs_val;
}
void BEQ(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if (op.rs_val == op.rt_val)
        cpu.PC = target;
}
void BNE(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if (op.rs_val != op.rt_val)
        cpu.PC = target;
}
void BLEZ(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if ((int64_t)op.rs_val <= 0)
        cpu.PC = target;
}
void BGTZ(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if ((int64_t)op.rs_val > 0)
        cpu.PC = target;
}
void BLTZ(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if ((int64_t)op.rs_val < 0)
        cpu.PC = target;
}
void BGEZ(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if ((int64_t)op.rs_val >= 0)
        cpu.PC = target;
}
void BLTZAL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.PC + 8;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if ((int64_t)op.rs_val < 0)
        cpu.PC = target;
}
void BGEZAL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.PC + 8;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if ((int64_t)op.rs_val >= 0)
        cpu.PC = target;
}
void BEQL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if (op.rs_val == op.rt_val)
        cpu.PC = target;
    else
        cpu.discard_bd = true;
}
void BNEL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if (op.rs_val != op.rt_val)
        cpu.PC = target;
    else
        cpu.discard_bd = true;
}
void BLEZL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if ((int64_t)op.rs_val <= 0)
        cpu.PC = target;
    else
        cpu.discard_bd = true;
}
void BGTZL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if ((int64_t)op.rs_val > 0)
        cpu.PC = target;
    else
        cpu.discard_bd = true;
}
void BLTZL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if ((int64_t)op.rs_val < 0)
        cpu.PC = target;
    else
        cpu.discard_bd = true;
}
void BGEZL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if ((int64_t)op.rs_val >= 0)
        cpu.PC = target;
    else
        cpu.discard_bd = true;
}
void BLTZALL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.PC + 8;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if ((int64_t)op.rs_val < 0)
        cpu.PC = target;
    else
        cpu.discard_bd = true;
}
void BGEZALL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.PC + 8;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if ((int64_t)op.rs_val >= 0)
        cpu.PC = target;
    else
        cpu.discard_bd = true;
}
// Special
void SYNC(VR4300 &cpu)
{
    NOP(cpu); // good thing i checked...
}
void SYSCALL(VR4300 &cpu)
{
    // exception handled in ex logic
}
void BREAK(VR4300 &cpu)
{
    // exception handled in ex logic
}
void TGE(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if ((int64_t)op.rs_val >= (int64_t)op.rt_val)
        op.result = 1;
}
void TGEU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if (op.rs_val >= op.rt_val)
        op.result = 1;
}
void TLT(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if ((int64_t)op.rs_val < (int64_t)op.rt_val)
        op.result = 1;
}
void TLTU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if (op.rs_val < op.rt_val)
        op.result = 1;
}
void TEQ(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if (op.rs_val == op.rt_val)
        op.result = 1;
}
void TNE(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if (op.rs_val != op.rt_val)
        op.result = 1;
}
void TGEI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if ((int64_t)op.rs_val >= (int16_t)(op.opcode & 0xFFFF))
        op.result = 1;
}
void TGEIU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if (op.rs_val >= (op.opcode & 0xFFFF))
        op.result = 1;
}
void TLTI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if ((int64_t)op.rs_val < (int16_t)(op.opcode & 0xFFFF))
        op.result = 1;
}
void TLTIU(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if (op.rs_val < (op.opcode & 0xFFFF))
        op.result = 1;
}
// immidiate is sign extended here, it doesn't say in general description, so good i checked...
void TEQI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if (op.rs_val == (uint64_t)(int16_t)(op.opcode & 0xFFFF))
        op.result = 1;
}
void TNEI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    if (op.rs_val == (uint64_t)(int16_t)(op.opcode & 0xFFFF))
        op.result = 1;
}
void LWCz(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}

void SWCz(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
    op.result = op.rt_val;
}
void MTCz(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)op.rt_val;
}
void MFCz(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)op.rd_val;
}
void CTCz(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)op.rt_val;
}
void CFCz(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = (int32_t)op.rd_val;
}

void COPz(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
}
// this only does stuff for fpu so make later with fpu, hopefully
void BCzT(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.PC + 8;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if (cpu.fpu.COC)
        cpu.PC = target;
}
// this only does stuff for fpu so make later with fpu, hopefully
void BCzF(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.PC + 8;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if (!cpu.fpu.COC)
        cpu.PC = target;
}
void DMTCz(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.rt_val;
}
void DMFCz(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.rd_val;
}
void LDCz(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}
void SDCz(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
    op.result = op.rt_val;
}

void BCzTL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.PC + 8;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if (cpu.fpu.COC)
        cpu.PC = target;
    else
        cpu.discard_bd = true;
}

void BCzFL(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.PC + 8;
    uint64_t target = 4 + op.PC + (((int16_t)(op.opcode & 0xFFFF)) << 2);
    if (!cpu.fpu.COC)
        cpu.PC = target;
    else
        cpu.discard_bd = true;
}

void TLBR(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint8_t tlb_index = cpu.cp0.index & 0x3F;
    uint64_t G = (cpu.cp0.TLB[tlb_index][1] >> 12) & 1;
    op.result_pagemask = cpu.cp0.TLB[tlb_index][0];
    op.result_entryHI = cpu.cp0.TLB[tlb_index][1] & ~(1 << 12);
    op.result_entryLO0 = cpu.cp0.TLB[tlb_index][2] + G;
    op.result_entryLO1 = cpu.cp0.TLB[tlb_index][3] + G;
}
void TLBWI(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t G = (cpu.cp0.entryLo0 & 1) && (cpu.cp0.entryLo1 & 1);
    uint64_t pagemask_high_bits = cpu.cp0.pageMask & (0b101010101010ULL << 13);
    op.result_pagemask = pagemask_high_bits | (pagemask_high_bits >> 1);
    op.result_entryHI = cpu.cp0.entryHi & ~op.result_pagemask & ~(G << 12) | (G << 12);
    op.result_entryLO0 = cpu.cp0.entryLo0 & (0x3fffffe);
    op.result_entryLO1 = cpu.cp0.entryLo1 & (0x3fffffe);
}
void TLBWR(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t G = (cpu.cp0.entryLo0 & 1) && (cpu.cp0.entryLo1 & 1);
    uint64_t pagemask_high_bits = cpu.cp0.pageMask & (0b101010101010ULL << 13);
    op.result_pagemask = pagemask_high_bits | (pagemask_high_bits >> 1);
    op.result_entryHI = cpu.cp0.entryHi & ~op.result_pagemask & ~(G << 12) | (G << 12);
    op.result_entryLO0 = cpu.cp0.entryLo0 & (0x3fffffe);
    op.result_entryLO1 = cpu.cp0.entryLo1 & (0x3fffffe);
}

// thsi will need to write the index reg
void TLBP(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;

    for (int i = 0; i < 32; i++)
    {
        const uint64_t *tlb_entry = cpu.cp0.TLB[i];
        uint8_t asid = tlb_entry[1] & 0xFF;
        uint8_t current_asid = cpu.cp0.entryHi & 0xFF;
        uint64_t VPN2 = tlb_entry[1] & 0xFFFFFFFE000;
        uint64_t current_VPN2 = cpu.cp0.entryHi & 0xFFFFFFFE000;
        uint64_t page_mask = tlb_entry[0];
        uint64_t R = (cpu.cp0.entryHi >> 62) & 3;
        uint8_t current_R = (tlb_entry[1] >> 62) & 3;
        bool G = (tlb_entry[1] >> 12) & 1;
        if (
            (VPN2 & ~page_mask) == (current_VPN2 & ~page_mask) &&
            (asid == current_asid || G) &&
            (R == current_R))
        {
            op.result = i;
            return;
        }
    }
    op.result = 1 << 31;
}

void ERET(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    cpu.discard_bd = true;
    if (cpu.cp0.status & STATUS_ERL_MASK)
    {
        cpu.PC = cpu.cp0.errorEPC;
        cpu.cp0.status = cpu.cp0.set_bits(cpu.cp0.status, STATUS_EXL_MASK, 0);
        cpu.cp0.stash_status();
    }
    else
    {
        cpu.PC = cpu.cp0.EPC;
        cpu.cp0.status = cpu.cp0.set_bits(cpu.cp0.status, STATUS_EXL_MASK, 0);
        cpu.cp0.stash_status();
    }
    cpu.LLBit = 0;
}

void CACHE(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.data_addr = (int16_t)(op.opcode & 0xFFFF) + op.rs_val;
}

// these would have been so much nicer to split into sub formats
void CVTSfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    float converted;
    switch (fmt)
    {
    case 16:
    {
        unimplemented = true;
        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        converted = (float)interpreted;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);

        invalid = std::isnan(interpreted) && ((fpr_val >> 51) & 1);
        if (invalid)
        {
            op.result = 0x7fbfffff;
            break;
        }

        underflow =
            (converted == 0 && interpreted != 0.0) ||
            ((std::fpclassify(converted) == FP_SUBNORMAL) && (std::fpclassify(interpreted) != FP_SUBNORMAL));

        unimplemented = ((std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                         std::isnan(interpreted) && !((fpr_val >> 51) & 1) ||
                         ((std::fpclassify(converted) == FP_SUBNORMAL) && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && !FS_BIT) ||
                         ((std::fpclassify(converted) == FP_SUBNORMAL) && !FS_BIT));

        if (unimplemented)
        {
            underflow = 0; // weird, i know
            break;
        }
        if ((underflow || (std::fpclassify(converted) == FP_SUBNORMAL)) && FS_BIT)
        {
            converted = cpu.fpu.flush_float(converted);
            op.result = (uint64_t)std::bit_cast<uint32_t>(converted);
        }

        inexact = (converted != interpreted);
        overflow = std::isinf(converted) && std::isfinite(interpreted);
        break;
    }
    case 20:
    {
        int32_t interpreted = (int32_t)fpr_val;
        converted = (float)interpreted;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);
        inexact = ((int32_t)converted != interpreted);
        break;
    }
    case 21:
    {
        int64_t interpreted = (int64_t)fpr_val;
        converted = (float)interpreted;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);
        unimplemented = interpreted != -(1LL << 55) && (std::abs(interpreted) >= (1ULL << 55));
        if (unimplemented)
            break;
        inexact = ((int64_t)converted != interpreted);
        overflow = std::fabs(interpreted) > std::numeric_limits<float>::max();
        break;
    }
    default:
        unimplemented = 1;
        break;
    }
    cpu.fpu.set_cause(inexact, underflow, overflow, 0, invalid, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
}

void CVTDfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;
    bool invalid = false;

    double converted;
    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        converted = (double)interpreted;
        op.result = std::bit_cast<uint64_t>(converted);

        invalid = std::isnan(interpreted) && ((fpr_val >> 22) & 1);
        if (invalid)
        {
            op.result = 0x7ff7ffffffffffff;
            break;
        }

        unimplemented = ((std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                         std::isnan(interpreted) && !((fpr_val >> 22) & 1) ||
                         ((std::fpclassify(converted) == FP_SUBNORMAL) && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         ((std::fpclassify(converted) == FP_SUBNORMAL) && !FS_BIT));

        break;
    }
    case 17:
    {
        unimplemented = true;
        break;
    }
    case 20:
    {
        int32_t interpreted = (int32_t)fpr_val;
        converted = (double)interpreted;
        op.result = std::bit_cast<uint64_t>(converted);
        inexact = ((int32_t)converted != interpreted);
        break;
    }
    case 21:
    {
        int64_t interpreted = (int64_t)fpr_val;
        converted = (double)interpreted;
        op.result = std::bit_cast<uint64_t>(converted);
        unimplemented = interpreted != -(1LL << 55) && (std::abs(interpreted) >= (1ULL << 55));
        if (unimplemented)
            break;
        inexact = ((int64_t)converted != interpreted);
        break;
    }
    default:
        unimplemented = 1;
        break;
    }
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};

void CVTLfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    int64_t converted;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        float rounded = std::nearbyintf(interpreted);
        converted = (int64_t)rounded;
        op.result = (uint64_t)converted;

        unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((float)converted != interpreted);
        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        double rounded = std::nearbyintl(interpreted);
        converted = (int64_t)rounded;
        op.result = (uint64_t)converted;

        unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((double)converted != interpreted);
        break;
    }
    case 20:
    {
        unimplemented = true;
        break;
    }
    case 21:
    {
        unimplemented = true;
        break;
    }
    default:
        unimplemented = true;
        break;
    }
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};

void CVTWfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    int32_t converted;
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        float rounded = std::nearbyintf(interpreted);
        converted = (int32_t)rounded;
        op.result = (uint64_t)converted;

        unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                        (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((float)converted != interpreted);

        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        double rounded = std::nearbyintl(interpreted);
        converted = (int32_t)rounded;
        op.result = (uint64_t)converted;

        unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                        (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((double)converted != interpreted);

        break;
    }
    case 20:
    {
        unimplemented = true;
        break;
    }
    case 21:
    {
        unimplemented = true;
        break;
    }
    default:
        unimplemented = true;
        break;
    }
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};

void ROUNDLfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    int64_t converted;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        converted = (int64_t)roundevenf(interpreted);
        op.result = (uint64_t)converted;
        unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((float)converted != interpreted);

        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        converted = (int64_t)roundevenl(interpreted);
        op.result = (uint64_t)converted;
        unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((double)converted != interpreted);
        break;
    }
    case 20:
    {
        unimplemented = true;
        break;
    }
    case 21:
    {
        unimplemented = true;
        break;
    }
    default:
        unimplemented = true;
        break;
    }
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};
void ROUNDWfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    int32_t converted;
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        float rounded = roundevenf(interpreted);
        ;
        converted = (int32_t)rounded;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);

        unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                        (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((float)converted != interpreted);

        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        double rounded = roundevenl(interpreted);
        converted = (int32_t)rounded;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);

        unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                        (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((double)converted != interpreted);

        break;
    }
    case 20:
    {
        unimplemented = true;
        break;
    }
    case 21:
    {
        unimplemented = true;
        break;
    }
    default:
        unimplemented = true;
        break;
    }
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};
void TRUNCLfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    int64_t converted;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        converted = (int64_t)std::round(interpreted);
        op.result = (uint64_t)converted;
        unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((float)converted != interpreted);

        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        converted = (int64_t)std::round(interpreted);
        op.result = (uint64_t)converted;
        unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((double)converted != interpreted);
        break;
    }
    case 20:
    {
        unimplemented = true;
        break;
    }
    case 21:
    {
        unimplemented = true;
        break;
    }
    default:
        unimplemented = true;
        break;
    }
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};
void TRUNCWfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    int32_t converted;
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        float rounded = std::trunc(interpreted);
        converted = (int32_t)rounded;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);

        unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                        (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((float)converted != interpreted);

        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        double rounded = std::trunc(interpreted);
        converted = (int32_t)rounded;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);

        unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                        (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((double)converted != interpreted);

        break;
    }
    case 20:
    {
        unimplemented = true;
        break;
    }
    case 21:
    {
        unimplemented = true;
        break;
    }
    default:
        unimplemented = true;
        break;
    }
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};
void CEILLfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    int64_t converted;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        converted = (int64_t)std::ceil(interpreted);
        op.result = (uint64_t)converted;
        unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((float)converted != interpreted);

        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        converted = (int64_t)std::ceil(interpreted);
        op.result = (uint64_t)converted;
        unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((double)converted != interpreted);
        break;
    }
    case 20:
    {
        unimplemented = true;
        break;
    }
    case 21:
    {
        unimplemented = true;
        break;
    }
    default:
        unimplemented = true;
        break;
    }
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};
void CEILWfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    int32_t converted;
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        float rounded = std::ceil(interpreted);
        converted = (int32_t)rounded;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);

        unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                        (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((float)converted != interpreted);

        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        double rounded = std::ceil(interpreted);
        converted = (int32_t)rounded;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);

        unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                        (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((double)converted != interpreted);

        break;
    }
    case 20:
    {
        unimplemented = true;
        break;
    }
    case 21:
    {
        unimplemented = true;
        break;
    }
    default:
        unimplemented = true;
        break;
    }
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};
void FLOORLfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    int64_t converted;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        converted = (int64_t)std::floor(interpreted);
        op.result = (uint64_t)converted;
        unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((float)converted != interpreted);

        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        converted = (int64_t)std::floor(interpreted);
        op.result = (uint64_t)converted;
        unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((double)converted != interpreted);
        break;
    }
    case 20:
    {
        unimplemented = true;
        break;
    }
    case 21:
    {
        unimplemented = true;
        break;
    }
    default:
        unimplemented = true;
        break;
    }
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};
void FLOORWfmt(VR4300 &cpu)
{

    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    int32_t converted;
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        float rounded = std::floor(interpreted);
        converted = (int32_t)rounded;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);

        unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                        (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((float)converted != interpreted);

        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        double rounded = std::floor(interpreted);
        converted = (int32_t)rounded;
        op.result = (uint64_t)std::bit_cast<uint32_t>(converted);

        unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                        (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                        std::isnan(interpreted) ||
                        std::isinf(interpreted);
        if (unimplemented)
            break;
        inexact = ((double)converted != interpreted);

        break;
    }
    case 20:
    {
        unimplemented = true;
        break;
    }
    case 21:
    {
        unimplemented = true;
        break;
    }
    default:
        unimplemented = true;
        break;
    }
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};

// fpu computational
void ADDfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr1_val = op.rd_val;
    uint64_t fpr2_val = op.rt_val;
    uint8_t fmt = op.rs;
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    switch (fmt)
    {
    case 16:
    {
        float operand1 = std::bit_cast<float>((uint32_t)fpr1_val);
        float operand2 = std::bit_cast<float>((uint32_t)fpr2_val);

        feclearexcept(FE_INEXACT);
        float result = operand1 + operand2;
        inexact = (bool)fetestexcept(FE_INEXACT);
        op.result = (uint64_t)std::bit_cast<uint32_t>(result);

        underflow =
            (operand1 != 0.0 && operand2 != 0.0 && result == 0.0 && operand1 != -operand2) ||
            (std::fpclassify(result) == FP_SUBNORMAL);
        inexact = inexact || underflow;

        unimplemented = ((std::fpclassify(operand1) == FP_SUBNORMAL) ||
                         (std::fpclassify(operand2) == FP_SUBNORMAL) ||
                         (std::isnan(operand1) && !((fpr1_val >> 22) & 1)) ||
                         (std::isnan(operand2) && !((fpr2_val >> 22) & 1)) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && !FS_BIT) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && !FS_BIT));

        if (unimplemented)
        {
            inexact = 0;
            underflow = 0; // weird, i know
            break;
        }

        invalid = std::isnan(operand1) && ((fpr1_val >> 22) & 1) ||
                  std::isnan(operand2) && ((fpr2_val >> 22) & 1) ||
                  std::isinf(operand1) || std::isinf(operand2);
        if (invalid)
        {
            inexact = 0;
            underflow = 0;
            op.result = 0x7fbfffff;
            break;
        }

        if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
        {
            result = cpu.fpu.flush_float(result);
            op.result = (uint64_t)std::bit_cast<uint32_t>(result);
        }

        overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);
        break;
    }
    case 17:
    {
        double operand1 = std::bit_cast<double>((uint64_t)fpr1_val);
        double operand2 = std::bit_cast<double>((uint64_t)fpr2_val);

        feclearexcept(FE_INEXACT);
        double result = operand1 + operand2;
        inexact = (bool)fetestexcept(FE_INEXACT);
        op.result = (uint64_t)std::bit_cast<uint64_t>(result);

        underflow =
            (operand1 != 0.0 && operand2 != 0.0 && result == 0.0 && operand1 != -operand2) ||
            (std::fpclassify(result) == FP_SUBNORMAL);
        inexact = inexact || underflow;

        unimplemented = ((std::fpclassify(operand1) == FP_SUBNORMAL) ||
                         (std::fpclassify(operand2) == FP_SUBNORMAL) ||
                         (std::isnan(operand1) && !((fpr1_val >> 51) & 1)) ||
                         (std::isnan(operand2) && !((fpr2_val >> 51) & 1)) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && !FS_BIT) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && !FS_BIT));

        if (unimplemented)
        {
            inexact = 0;
            underflow = 0; // weird, i know
            break;
        }

        invalid = std::isnan(operand1) && ((fpr1_val >> 51) & 1) ||
                  std::isnan(operand2) && ((fpr2_val >> 51) & 1) ||
                  std::isinf(operand1) || std::isinf(operand2);
        if (invalid)
        {
            inexact = 0;
            underflow = 0;
            op.result = 0x7ff7ffffffffffff;
            break;
        }

        if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
        {
            result = cpu.fpu.flush_double(result);
            op.result = (uint64_t)std::bit_cast<uint64_t>(result);
        }

        overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);
        break;
    }
    case 20:
    {
        unimplemented = 1;
        break;
    }
    case 21:
    {
        unimplemented = 1;
        break;
    }
    default:
        unimplemented = 1;
        break;
    }
    cpu.fpu.set_cause(inexact, underflow, overflow, 0, invalid, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};
void SUBfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr1_val = op.rd_val;
    uint64_t fpr2_val = op.rt_val;
    uint8_t fmt = op.rs;
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    switch (fmt)
    {
    case 16:
    {
        float operand1 = std::bit_cast<float>((uint32_t)fpr1_val);
        float operand2 = std::bit_cast<float>((uint32_t)fpr2_val);

        feclearexcept(FE_INEXACT);
        float result = operand1 - operand2;
        inexact = (bool)fetestexcept(FE_INEXACT);
        op.result = (uint64_t)std::bit_cast<uint32_t>(result);

        underflow =
            (operand1 != 0.0 && operand2 != 0.0 && result == 0.0 && operand1 != operand2) ||
            (std::fpclassify(result) == FP_SUBNORMAL);
        inexact = inexact || underflow;

        unimplemented = ((std::fpclassify(operand1) == FP_SUBNORMAL) ||
                         (std::fpclassify(operand2) == FP_SUBNORMAL) ||
                         (std::isnan(operand1) && !((fpr1_val >> 22) & 1)) ||
                         (std::isnan(operand2) && !((fpr2_val >> 22) & 1)) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && !FS_BIT) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && !FS_BIT));

        if (unimplemented)
        {
            inexact = 0;
            underflow = 0; // weird, i know
            break;
        }

        invalid = std::isnan(operand1) && ((fpr1_val >> 22) & 1) ||
                  std::isnan(operand2) && ((fpr2_val >> 22) & 1) ||
                  std::isinf(operand1) || std::isinf(operand2);
        if (invalid)
        {
            inexact = 0;
            underflow = 0;
            op.result = 0x7fbfffff;
            break;
        }

        if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
        {
            result = cpu.fpu.flush_float(result);
            op.result = (uint64_t)std::bit_cast<uint32_t>(result);
        }

        overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);
        break;
    }
    case 17:
    {
        double operand1 = std::bit_cast<double>((uint64_t)fpr1_val);
        double operand2 = std::bit_cast<double>((uint64_t)fpr2_val);

        feclearexcept(FE_INEXACT);
        double result = operand1 - operand2;
        inexact = (bool)fetestexcept(FE_INEXACT);
        op.result = (uint64_t)std::bit_cast<uint64_t>(result);

        underflow =
            (operand1 != 0.0 && operand2 != 0.0 && result == 0.0 && operand1 != operand2) ||
            (std::fpclassify(result) == FP_SUBNORMAL);
        inexact = inexact || underflow;

        unimplemented = ((std::fpclassify(operand1) == FP_SUBNORMAL) ||
                         (std::fpclassify(operand2) == FP_SUBNORMAL) ||
                         (std::isnan(operand1) && !((fpr1_val >> 51) & 1)) ||
                         (std::isnan(operand2) && !((fpr2_val >> 51) & 1)) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && !FS_BIT) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && !FS_BIT));

        if (unimplemented)
        {
            inexact = 0;
            underflow = 0; // weird, i know
            break;
        }

        invalid = std::isnan(operand1) && ((fpr1_val >> 51) & 1) ||
                  std::isnan(operand2) && ((fpr2_val >> 51) & 1) ||
                  std::isinf(operand1) || std::isinf(operand2);
        if (invalid)
        {
            inexact = 0;
            underflow = 0;
            op.result = 0x7ff7ffffffffffff;
            break;
        }

        if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
        {
            result = cpu.fpu.flush_double(result);
            op.result = (uint64_t)std::bit_cast<uint64_t>(result);
        }

        overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);
        break;
    }
    case 20:
    {
        unimplemented = 1;
        break;
    }
    case 21:
    {
        unimplemented = 1;
        break;
    }
    default:
        unimplemented = 1;
        break;
    }
    cpu.fpu.set_cause(inexact, underflow, overflow, 0, invalid, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};
void MULfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr1_val = op.rd_val;
    uint64_t fpr2_val = op.rt_val;
    uint8_t fmt = op.rs;
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    switch (fmt)
    {
    case 16:
    {
        float operand1 = std::bit_cast<float>((uint32_t)fpr1_val);
        float operand2 = std::bit_cast<float>((uint32_t)fpr2_val);

        feclearexcept(FE_INEXACT);
        float result = operand1 * operand2;
        inexact = (bool)fetestexcept(FE_INEXACT);
        op.result = (uint64_t)std::bit_cast<uint32_t>(result);

        underflow =
            (operand1 != 0.0 && operand2 != 0.0 && result == 0.0) ||
            (std::fpclassify(result) == FP_SUBNORMAL);
        inexact = inexact || underflow;

        unimplemented = ((std::fpclassify(operand1) == FP_SUBNORMAL) ||
                         (std::fpclassify(operand2) == FP_SUBNORMAL) ||
                         (std::isnan(operand1) && !((fpr1_val >> 22) & 1)) ||
                         (std::isnan(operand2) && !((fpr2_val >> 22) & 1)) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && !FS_BIT) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && !FS_BIT));

        if (unimplemented)
        {
            inexact = 0;
            underflow = 0; // weird, i know
            break;
        }

        invalid = std::isnan(operand1) && ((fpr1_val >> 22) & 1) ||
                  std::isnan(operand2) && ((fpr2_val >> 22) & 1) ||
                  std::isinf(operand1) && (operand2 == 0) ||
                  std::isinf(operand2) && (operand1 == 0);
        if (invalid)
        {
            inexact = 0;
            underflow = 0;
            op.result = 0x7fbfffff;
            break;
        }

        if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
        {
            result = cpu.fpu.flush_float(result);
            op.result = (uint64_t)std::bit_cast<uint32_t>(result);
        }

        overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);
        break;
    }
    case 17:
    {
        double operand1 = std::bit_cast<double>((uint64_t)fpr1_val);
        double operand2 = std::bit_cast<double>((uint64_t)fpr2_val);

        feclearexcept(FE_INEXACT);
        double result = operand1 * operand2;
        inexact = (bool)fetestexcept(FE_INEXACT);
        op.result = (uint64_t)std::bit_cast<uint64_t>(result);

        underflow =
            (operand1 != 0.0 && operand2 != 0.0 && result == 0.0) ||
            (std::fpclassify(result) == FP_SUBNORMAL);
        inexact = inexact || underflow;

        unimplemented = ((std::fpclassify(operand1) == FP_SUBNORMAL) ||
                         (std::fpclassify(operand2) == FP_SUBNORMAL) ||
                         (std::isnan(operand1) && !((fpr1_val >> 51) & 1)) ||
                         (std::isnan(operand2) && !((fpr2_val >> 51) & 1)) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && !FS_BIT) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && !FS_BIT));

        if (unimplemented)
        {
            inexact = 0;
            underflow = 0; // weird, i know
            break;
        }

        invalid = std::isnan(operand1) && ((fpr1_val >> 51) & 1) ||
                  std::isnan(operand2) && ((fpr2_val >> 51) & 1) ||
                  std::isinf(operand1) && (operand2 == 0) ||
                  std::isinf(operand2) && (operand1 == 0);
        if (invalid)
        {
            inexact = 0;
            underflow = 0;
            op.result = 0x7ff7ffffffffffff;
            break;
        }

        if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
        {
            result = cpu.fpu.flush_double(result);
            op.result = (uint64_t)std::bit_cast<uint64_t>(result);
        }

        overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);
        break;
    }
    case 20:
    {
        unimplemented = 1;
        break;
    }
    case 21:
    {
        unimplemented = 1;
        break;
    }
    default:
        unimplemented = 1;
        break;
    }
    cpu.fpu.set_cause(inexact, underflow, overflow, 0, invalid, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};
void DIVfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr1_val = op.rd_val;
    uint64_t fpr2_val = op.rt_val;
    uint8_t fmt = op.rs;
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;
    bool zerodiv = false;

    switch (fmt)
    {
    case 16:
    {
        float operand1 = std::bit_cast<float>((uint32_t)fpr1_val);
        float operand2 = std::bit_cast<float>((uint32_t)fpr2_val);

        feclearexcept(FE_INEXACT);
        float result = operand1 / operand2;
        inexact = (bool)fetestexcept(FE_INEXACT);
        op.result = (uint64_t)std::bit_cast<uint32_t>(result);

        unimplemented = ((std::fpclassify(operand1) == FP_SUBNORMAL) ||
                         (std::fpclassify(operand2) == FP_SUBNORMAL) ||
                         (std::isnan(operand1) && !((fpr1_val >> 22) & 1)) ||
                         (std::isnan(operand2) && !((fpr2_val >> 22) & 1)) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && !FS_BIT) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && !FS_BIT));

        if (unimplemented)
        {
            inexact = 0;
            underflow = 0; // weird, i know
            break;
        }

        invalid = (std::isnan(operand1) && ((fpr1_val >> 22) & 1)) ||
                  (std::isnan(operand2) && ((fpr2_val >> 22) & 1)) ||
                  ((operand1 == 0.f) && (operand2 == 0.f)) ||
                  (std::isinf(operand2) && std::isinf(operand1));
        if (invalid)
        {
            inexact = 0;
            underflow = 0;
            op.result = 0x7fbfffff;
            break;
        }

        if (operand2 == 0)
        {
            zerodiv = true;
            break;
        }

        underflow =
            (operand1 != 0.0 && std::isfinite(operand2) && result == 0.0) ||
            (std::fpclassify(result) == FP_SUBNORMAL);
        inexact = inexact || underflow;

        if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
        {
            result = cpu.fpu.flush_float(result);
            op.result = (uint64_t)std::bit_cast<uint32_t>(result);
        }

        overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);
        break;
    }
    case 17:
    {
        double operand1 = std::bit_cast<double>((uint64_t)fpr1_val);
        double operand2 = std::bit_cast<double>((uint64_t)fpr2_val);

        feclearexcept(FE_INEXACT);
        double result = operand1 / operand2;
        inexact = (bool)fetestexcept(FE_INEXACT);
        op.result = (uint64_t)std::bit_cast<uint64_t>(result);

        unimplemented = ((std::fpclassify(operand1) == FP_SUBNORMAL) ||
                         (std::fpclassify(operand2) == FP_SUBNORMAL) ||
                         (std::isnan(operand1) && !((fpr1_val >> 51) & 1)) ||
                         (std::isnan(operand2) && !((fpr2_val >> 51) & 1)) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                         (underflow && !FS_BIT) ||
                         ((std::fpclassify(result) == FP_SUBNORMAL) && !FS_BIT));

        if (unimplemented)
        {
            inexact = 0;
            underflow = 0; // weird, i know
            break;
        }
        invalid = (std::isnan(operand1) && ((fpr1_val >> 51) & 1)) ||
                  (std::isnan(operand2) && ((fpr2_val >> 51) & 1)) ||
                  ((operand1 == 0.) && (operand2 == 0.)) ||
                  (std::isinf(operand2) && std::isinf(operand1));
        if (invalid)
        {
            inexact = 0;
            underflow = 0;
            op.result = 0x7ff7ffffffffffff;
            break;
        }

        if (operand2 == 0)
        {
            zerodiv = true;
            break;
        }

        underflow =
            (operand1 != 0.0 && std::isfinite(operand2) && result == 0.0) ||
            (std::fpclassify(result) == FP_SUBNORMAL);
        inexact = inexact || underflow;

        if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
        {
            result = cpu.fpu.flush_double(result);
            op.result = (uint64_t)std::bit_cast<uint64_t>(result);
        }

        overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);
        break;
    }
    case 20:
    {
        unimplemented = 1;
        break;
    }
    case 21:
    {
        unimplemented = 1;
        break;
    }
    default:
        unimplemented = 1;
        break;
    }
    cpu.fpu.set_cause(inexact, underflow, overflow, zerodiv, invalid, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};
void ABSfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    int32_t converted;
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;

    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        float absolute = std::abs(interpreted);
        op.result = (uint64_t)std::bit_cast<uint32_t>(absolute);

        invalid = std::isnan(interpreted) && ((fpr_val >> 22) & 1);
        if (invalid)
        {
            op.result = 0x7fbfffff;
            break;
        }

        unimplemented =
            (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
            std::isnan(interpreted);
        if (unimplemented)
            break;

        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        double absolute = std::abs(interpreted);
        op.result = (uint64_t)std::bit_cast<uint64_t>(absolute);

        invalid = std::isnan(interpreted) && ((fpr_val >> 51) & 1);
        if (invalid)
        {
            op.result = 0x7ff7ffffffffffff;
            break;
        }

        unimplemented =
            (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
            std::isnan(interpreted);
        if (unimplemented)
            break;

        break;
    }
    case 20:
    {
        unimplemented = true;
        break;
    }
    case 21:
    {
        unimplemented = true;
        break;
    }
    default:
        unimplemented = true;
        break;
    }
    cpu.fpu.set_cause(0, 0, 0, 0, invalid, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};

void MOVfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    op.result = op.rd_val;
};

void NEGfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    int32_t converted;
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;

    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
        float negated = -interpreted;
        op.result = (uint64_t)std::bit_cast<uint32_t>(negated);

        invalid = std::isnan(interpreted) && ((fpr_val >> 22) & 1);
        if (invalid)
        {
            op.result = 0x7fbfffff;
            break;
        }

        unimplemented =
            (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
            std::isnan(interpreted);
        if (unimplemented)
            break;

        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
        double negated = -interpreted;
        op.result = (uint64_t)std::bit_cast<uint64_t>(negated);

        invalid = std::isnan(interpreted) && ((fpr_val >> 51) & 1);
        if (invalid)
        {
            op.result = 0x7ff7ffffffffffff;
            break;
        }

        unimplemented =
            (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
            std::isnan(interpreted);
        if (unimplemented)
            break;

        break;
    }
    case 20:
    {
        unimplemented = true;
        break;
    }
    case 21:
    {
        unimplemented = true;
        break;
    }
    default:
        unimplemented = true;
        break;
    }
    cpu.fpu.set_cause(0, 0, 0, 0, invalid, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};

void SQRTfmt(VR4300 &cpu)
{
    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr_val = op.rd_val;
    uint8_t fmt = op.rs;
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;

    switch (fmt)
    {
    case 16:
    {
        float interpreted = std::bit_cast<float>((uint32_t)fpr_val);

        invalid = std::isnan(interpreted) && ((fpr_val >> 22) & 1) ||
                  (interpreted < 0 && !(std::fpclassify(interpreted) == FP_SUBNORMAL));
        if (invalid)
        {
            op.result = 0x7fbfffff;
            break;
        }

        unimplemented =
            (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
            std::isnan(interpreted);
        if (unimplemented)
            break;

        float root_result = std::sqrtf(interpreted);
        double accurate = std::sqrt((double)interpreted);
        op.result = (uint64_t)std::bit_cast<uint32_t>(root_result);

        inexact = ((double)root_result != accurate);

        break;
    }
    case 17:
    {
        double interpreted = std::bit_cast<double>((uint64_t)fpr_val);

        invalid = std::isnan(interpreted) && ((fpr_val >> 51) & 1) ||
                  (interpreted < 0 && !(std::fpclassify(interpreted) == FP_SUBNORMAL));
        if (invalid)
        {
            op.result = 0x7ff7ffffffffffff;
            break;
        }

        unimplemented =
            (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
            std::isnan(interpreted);
        if (unimplemented)
            break;

        double root_result = std::sqrt(interpreted);
        long double accurate = std::sqrtl((long double)interpreted); // maybe replace with checking fpu flags later.
        op.result = (uint64_t)std::bit_cast<uint64_t>(root_result);

        inexact = ((long double)root_result != accurate);

        break;
    }
    case 20:
    {
        unimplemented = true;
        break;
    }
    case 21:
    {
        unimplemented = true;
        break;
    }
    default:
        unimplemented = true;
        break;
    }
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};

// fpu compare
void Ccondfmt(VR4300 &cpu)
{

    VR4300::Operation &op = *cpu.EX_in.op;
    uint64_t fpr1_val = op.rd_val;
    uint64_t fpr2_val = op.rt_val;
    uint8_t fmt = op.rs;
    uint8_t cond = op.opcode & 0xF;
    bool condition = false;
    bool less = false;
    bool equal = false;
    bool unordered = false;

    cpu.fpu.clear_cause();

    bool unimplemented = false;
    bool invalid = false;

    switch (fmt)
    {
    case 16:
    {
        float operand1 = std::bit_cast<float>((uint32_t)fpr1_val);
        float operand2 = std::bit_cast<float>((uint32_t)fpr2_val);

        if (std::isnan(operand1) || std::isnan(operand2))
        {
            less = false;
            equal = false;
            unordered = true;
            invalid = (cond & 0x8) ||
                      (std::isnan(operand1) && ((fpr1_val >> 22) & 1)) ||
                      (std::isnan(operand2) && ((fpr2_val >> 22) & 1));
        }
        else
        {
            less = operand1 < operand2;
            equal = operand1 == operand2;
            unordered = false;
        }
        condition =
            ((cond & 0x4) && less) ||
            ((cond & 0x2) && equal) ||
            ((cond & 0x1) && unordered);
        break;
    }
    case 17:
    {
        double operand1 = std::bit_cast<double>((uint64_t)fpr1_val);
        double operand2 = std::bit_cast<double>((uint64_t)fpr2_val);

        if (std::isnan(operand1) || std::isnan(operand2))
        {
            less = false;
            equal = false;
            unordered = true;
            invalid = (cond & 0x8) ||
                      (std::isnan(operand1) && ((fpr1_val >> 51) & 1)) ||
                      (std::isnan(operand2) && ((fpr2_val >> 51) & 1));
        }
        else
        {
            less = operand1 < operand2;
            equal = operand1 == operand2;
            unordered = false;
        }
        condition =
            ((cond & 0x4) && less) ||
            ((cond & 0x2) && equal) ||
            ((cond & 0x1) && unordered);
        break;
    }
    case 20:
    {
        unimplemented = 1;
        break;
    }
    case 21:
    {
        unimplemented = 1;
        break;
    }
    default:
        unimplemented = 1;
        break;
    }
    cpu.update_conditional = 1;
    cpu.EX_in.op->conditional_val = condition;
    cpu.fpu.set_cause(0, 0, 0, 0, invalid, unimplemented);
    if ((cpu.fpu.FCR31 >> 12) & 0x3F)
        cpu.EX_in.op->fire_fpu_exception = 1;
};

VR4300::VR4300() : cp0(), rcp(rcp), fpu(cp0),op_storage{*this,*this,*this,*this},
noptmplt{NOP, 0, 0, 0, 0, OpType::NOP, *this},
primary_op_lut{
    /*00*/ {nullptr, 0, 0, 0, 0, OpType::SPECIAL, *this}, // SPECIAL
    /*01*/ {nullptr, 0, 0, 0, 0, OpType::REGIMM, *this},  // REGIMM
    /*02*/ {J, CAUSES_BRANCH_DELAY, 0, 0, 0, OpType::J, *this},
    /*03*/ {JAL, CAUSES_BRANCH_DELAY | STORES_IN_31 | WRITES_REG, 0, 0, 0, OpType::JAL, *this},
    /*04*/ {BEQ, CAUSES_BRANCH_DELAY, 0, 0, 0, OpType::BEQ, *this},
    /*05*/ {BNE, CAUSES_BRANCH_DELAY, 0, 0, 0, OpType::BNE, *this},
    /*06*/ {BLEZ, CAUSES_BRANCH_DELAY, 0, 0, 0, OpType::BLEZ, *this},
    /*07*/ {BGTZ, CAUSES_BRANCH_DELAY, 0, 0, 0, OpType::BGTZ, *this},
    /*08*/ {ADDI, WRITES_REG | STORES_IN_RT | CAUSES_OVERFLOW_EXCEPTION, 0, 0, 0, OpType::ADDI, *this},
    /*09*/ {ADDIU, WRITES_REG | STORES_IN_RT, 0, 0, 0, OpType::ADDIU, *this},
    /*0A*/ {SLTI, WRITES_REG | STORES_IN_RT, 0, 0, 0, OpType::SLTI, *this},
    /*0B*/ {SLTIU, WRITES_REG | STORES_IN_RT, 0, 0, 0, OpType::SLTIU, *this},
    /*0C*/ {ANDI, WRITES_REG | STORES_IN_RT, 0, 0, 0, OpType::ANDI, *this},
    /*0D*/ {ORI, WRITES_REG | STORES_IN_RT, 0, 0, 0, OpType::ORI, *this},
    /*0E*/ {XORI, WRITES_REG | STORES_IN_RT, 0, 0, 0, OpType::XORI, *this},
    /*0F*/ {LUI, WRITES_REG | STORES_IN_RT, 0, 0, 0, OpType::LUI, *this}, // LUI reads no GPR (rs unused)
    /*10*/ {COPz, 0, 0, 0, 0, OpType::COPz, *this},
    /*11*/ {COPz, 0, 0, 0, 1, OpType::COPz, *this},
    /*12*/ {},
    /*13*/ {},
    /*14*/ {BEQL, CAUSES_BRANCH_DELAY, 0, 0, 0, OpType::BEQL, *this},
    /*15*/ {BNEL, CAUSES_BRANCH_DELAY, 0, 0, 0, OpType::BNEL, *this},
    /*16*/ {BLEZL, CAUSES_BRANCH_DELAY, 0, 0, 0, OpType::BLEZL, *this},
    /*17*/ {BGTZL, CAUSES_BRANCH_DELAY, 0, 0, 0, OpType::BGTZL, *this},
    /*18*/ {DADDI, WRITES_REG | STORES_IN_RT | CAUSES_OVERFLOW_EXCEPTION, 0, 0, 0, OpType::DADDI, *this},
    /*19*/ {DADDIU, WRITES_REG | STORES_IN_RT, 0, 0, 0, OpType::DADDIU, *this},
    /*1A*/ {LDL, IS_LOAD | LEFT_ACCESS | WRITES_REG | STORES_IN_RT, 0, 8, 0, OpType::LDL, *this},
    /*1B*/ {LDR, IS_LOAD | RIGHT_ACCESS | WRITES_REG | STORES_IN_RT, 0, 8, 0, OpType::LDR, *this},
    /*1C*/ {},
    /*1D*/ {},
    /*1E*/ {},
    /*1F*/ {},
    /*20*/ {LB, IS_LOAD | SIGN_EXTENDED | WRITES_REG | STORES_IN_RT | SIGN_EXTENDED, 0, 1, 0, OpType::LB, *this},
    /*21*/ {LH, IS_LOAD | SIGN_EXTENDED | WRITES_REG | STORES_IN_RT | SIGN_EXTENDED, 0, 2, 0, OpType::LH, *this},
    /*22*/ {LWL, IS_LOAD | WRITES_REG | STORES_IN_RT | LEFT_ACCESS | SIGN_EXTENDED, 0, 4, 0, OpType::LWL, *this},
    /*23*/ {LW, IS_LOAD | SIGN_EXTENDED | WRITES_REG | STORES_IN_RT, 0, 4, 0, OpType::LW, *this},
    /*24*/ {LBU, IS_LOAD | WRITES_REG | STORES_IN_RT, 0, 1, 0, OpType::LBU, *this},
    /*25*/ {LHU, IS_LOAD | WRITES_REG | STORES_IN_RT, 0, 2, 0, OpType::LHU, *this},
    /*26*/ {LWR, IS_LOAD | WRITES_REG | STORES_IN_RT | RIGHT_ACCESS | SIGN_EXTENDED, 0, 4, 0, OpType::LWR, *this},
    /*27*/ {LWU, IS_LOAD | WRITES_REG | STORES_IN_RT, 0, 4, 0, OpType::LWU, *this},
    /*28*/ {SB, IS_STORE, 0, 1, 0, OpType::SB, *this},
    /*29*/ {SH, IS_STORE, 0, 2, 0, OpType::SH, *this},
    /*2A*/ {SWL, IS_STORE | LEFT_ACCESS, 0, 4, 0, OpType::SWL, *this},
    /*2B*/ {SW, IS_STORE, 0, 4, 0, OpType::SW, *this},
    /*2C*/ {SDL, IS_STORE | LEFT_ACCESS, 0, 8, 0, OpType::SDL, *this},
    /*2D*/ {SDR, IS_STORE | RIGHT_ACCESS, 0, 8, 0, OpType::SDR, *this},
    /*2E*/ {SWR, IS_STORE | RIGHT_ACCESS, 0, 4, 0, OpType::SWR, *this},
    /*2F*/ {CACHE, 0, 0, 0, 0, OpType::CACHE, *this}, // CACHE uses base (rs)
    /*30*/ {LL, IS_LOAD | WRITES_REG | STORES_IN_RT | ATOMIC | SIGN_EXTENDED, 0, 4, 0, OpType::LL, *this},
    /*31*/ {LWCz, IS_LOAD | WRITES_REG | SIGN_EXTENDED | STORES_IN_RT | WRITES_CP, 0, 4, 1, OpType::LWCz, *this},
    /*32*/ {LWCz, IS_LOAD | WRITES_REG | SIGN_EXTENDED | STORES_IN_RT | WRITES_CP, 0, 4, 2, OpType::LWCz, *this},
    /*33*/ {},
    /*34*/ {LLD, IS_LOAD | WRITES_REG | STORES_IN_RT | ATOMIC, 0, 8, 0, OpType::LLD, *this},
    /*35*/ {LDCz, WRITES_REG | IS_LOAD | SIGN_EXTENDED | WRITES_CP | STORES_IN_RT, 0, 8, 1, OpType::LDCz, *this},
    /*36*/ {LDCz, WRITES_REG | IS_LOAD | SIGN_EXTENDED | WRITES_CP | STORES_IN_RT, 0, 8, 2, OpType::LDCz, *this},
    /*37*/ {LD, IS_LOAD | SIGN_EXTENDED | WRITES_REG | STORES_IN_RT, 0, 8, 0, OpType::LD, *this},
    /*38*/ {SC, IS_STORE | STORES_IN_RT | ATOMIC, 0, 4, 0, OpType::SC, *this},
    /*39*/ {SWCz, IS_STORE | READS_CP, 0, 4, 1, OpType::SWCz, *this},
    /*3A*/ {SWCz, IS_STORE | READS_CP, 0, 4, 2, OpType::SWCz, *this},
    /*3B*/ {},
    /*3C*/ {SCD, IS_STORE | STORES_IN_RT | ATOMIC, 0, 8, 0, OpType::SCD, *this},
    /*3D*/ {SDCz, IS_STORE | READS_CP, 0, 8, 1, OpType::SDCz, *this},
    /*3E*/ {SDCz, IS_STORE | READS_CP, 0, 8, 2, OpType::SDCz, *this},
    /*3F*/ {SD, IS_STORE, 0, 8, 0, OpType::SD, *this},
},
special_op_lut{
    /*00*/ {SLL, WRITES_REG | STORES_IN_RD, 0, 0, 0, OpType::SLL, *this},
    /*01*/ {},
    /*02*/ {SRL, WRITES_REG | STORES_IN_RD, 0, 0, 0, OpType::SRL, *this},
    /*03*/ {SRA, WRITES_REG | STORES_IN_RD, 0, 0, 0, OpType::SRA, *this},
    /*04*/ {SLLV, WRITES_REG | STORES_IN_RD, 0, 0, 0, OpType::SLLV, *this},
    /*05*/ {},
    /*06*/ {SRLV, WRITES_REG | STORES_IN_RD, 0, 0, 0, OpType::SRLV, *this},
    /*07*/ {SRAV, WRITES_REG | STORES_IN_RD, 0, 0, 0, OpType::SRAV, *this},
    /*08*/ {JR, CAUSES_BRANCH_DELAY, 0, 0, 0, OpType::JR, *this},
    /*09*/ {JALR, CAUSES_BRANCH_DELAY | STORES_IN_RD | WRITES_REG, 0, 0, 0, OpType::JALR, *this},
    /*0A*/ {},
    /*0B*/ {},
    /*0C*/ {SYSCALL, CAUSED_EXCEPTION, 0, 0, 0, OpType::SYSCALL, *this},
    /*0D*/ {BREAK, CAUSED_EXCEPTION, 0, 0, 0, OpType::BREAK, *this},
    /*0E*/ {},
    /*0F*/ {SYNC, 0, 0, 0, 0, OpType::SYNC, *this},
    /*10*/ {MFHI, WRITES_REG | STORES_IN_RD, 0, 0, 0, OpType::MFHI, *this},
    /*11*/ {MTHI, WRITES_HI, 0, 0, 0, OpType::MTHI, *this},
    /*12*/ {MFLO, WRITES_REG | STORES_IN_RD, 0, 0, 0, OpType::MFLO, *this},
    /*13*/ {MTLO, WRITES_LO, 0, 0, 0, OpType::MTLO, *this},
    /*14*/ {DSLLV, WRITES_REG | STORES_IN_RD, 0, 0, 0, OpType::DSLLV, *this},
    /*15*/ {},
    /*16*/ {DSRLV, WRITES_REG | STORES_IN_RD, 0, 0, 0, OpType::DSRLV, *this},
    /*17*/ {DSRAV, WRITES_REG | STORES_IN_RD, 0, 0, 0, OpType::DSRAV, *this},
    /*18*/ {MULT, WRITES_HI | WRITES_LO, 5, 0, 0, OpType::MULT, *this},
    /*19*/ {MULTU, WRITES_HI | WRITES_LO, 5, 0, 0, OpType::MULTU, *this},
    /*1A*/ {DIV, WRITES_HI | WRITES_LO, 37, 0, 0, OpType::DIV, *this},
    /*1B*/ {DIVU, WRITES_HI | WRITES_LO, 37, 0, 0, OpType::DIVU, *this},
    /*1C*/ {DMULT, WRITES_HI | WRITES_LO, 8, 0,0, OpType::DMULT, *this},
    /*1D*/ {DMULTU, WRITES_HI | WRITES_LO, 8, 0,0, OpType::DMULTU, *this},
    /*1E*/ {DDIV, WRITES_HI | WRITES_LO, 69, 0,0, OpType::DDIV, *this},
    /*1F*/ {DDIVU, WRITES_HI | WRITES_LO, 69, 0,0, OpType::DDIVU, *this},
    /*20*/ {ADD, WRITES_REG | STORES_IN_RD | CAUSES_OVERFLOW_EXCEPTION, 0, 0,0, OpType::ADD, *this},
    /*21*/ {ADDU, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::ADDU, *this},
    /*22*/ {SUB, WRITES_REG | STORES_IN_RD | CAUSES_OVERFLOW_EXCEPTION, 0, 0,0, OpType::SUB, *this},
    /*23*/ {SUBU, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::SUBU, *this},
    /*24*/ {AND, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::AND, *this},
    /*25*/ {OR, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::OR, *this},
    /*26*/ {XOR, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::XOR, *this},
    /*27*/ {NOR, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::NOR, *this},
    /*28*/ {},
    /*29*/ {},
    /*2A*/ {SLT, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::SLT, *this},
    /*2B*/ {SLTU, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::SLTU, *this},
    /*2C*/ {DADD, WRITES_REG | STORES_IN_RD | CAUSES_OVERFLOW_EXCEPTION, 0, 0,0, OpType::DADD, *this},
    /*2D*/ {DADDU, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::DADDU, *this},
    /*2E*/ {DSUB, WRITES_REG | STORES_IN_RD | CAUSES_OVERFLOW_EXCEPTION, 0, 0,0, OpType::DSUB, *this},
    /*2F*/ {DSUBU, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::DSUBU, *this},
    /*30*/ {TGE, IS_TRAP, 0, 0,0, OpType::TGE, *this},
    /*31*/ {TGEU, IS_TRAP, 0, 0,0, OpType::TGEU, *this},
    /*32*/ {TLT, IS_TRAP, 0, 0,0, OpType::TLT, *this},
    /*33*/ {TLTU, IS_TRAP, 0, 0,0, OpType::TLTU, *this},
    /*34*/ {TEQ, IS_TRAP, 0, 0,0, OpType::TEQ, *this},
    /*35*/ {},
    /*36*/ {TNE, IS_TRAP, 0, 0,0, OpType::TNE, *this},
    /*37*/ {},
    /*38*/ {DSLL, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::DSLL, *this},
    /*39*/ {},
    /*3A*/ {DSRL, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::DSRL, *this},
    /*3B*/ {DSRA, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::DSRA, *this},
    /*3C*/ {DSLL32, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::DSLL32, *this},
    /*3D*/ {},
    /*3E*/ {DSRL32, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::DSRL32, *this},
    /*3F*/ {DSRA32, WRITES_REG | STORES_IN_RD, 0, 0,0, OpType::DSRA32, *this},
},
regimm_op_lut{
    /*00*/ {BLTZ, CAUSES_BRANCH_DELAY, 0, 0,0, OpType::BLTZ, *this},
    /*01*/ {BGEZ, CAUSES_BRANCH_DELAY, 0, 0,0, OpType::BGEZ, *this},
    /*02*/ {BLTZL, CAUSES_BRANCH_DELAY, 0, 0,0, OpType::BLTZL, *this},
    /*03*/ {BGEZL, CAUSES_BRANCH_DELAY, 0, 0,0, OpType::BGEZL, *this},
    /*04*/ {},
    /*05*/ {},
    /*06*/ {},
    /*07*/ {},
    /*08*/ {TGEI, IS_TRAP, 0, 0,0, OpType::TGEI, *this},
    /*09*/ {TGEIU, IS_TRAP, 0, 0,0, OpType::TGEIU, *this},
    /*0A*/ {TLTI, IS_TRAP, 0, 0,0, OpType::TLTI, *this},
    /*0B*/ {TLTIU, IS_TRAP, 0, 0,0, OpType::TLTIU, *this},
    /*0C*/ {TEQI, IS_TRAP, 0, 0,0, OpType::TEQI, *this},
    /*0D*/ {},
    /*0E*/ {TNEI, IS_TRAP, 0, 0,0, OpType::TNEI, *this},
    /*0F*/ {},
    /*10*/ {BLTZAL, CAUSES_BRANCH_DELAY | STORES_IN_31 | WRITES_REG, 0, 0,0,OpType::BLTZAL, *this},
    /*11*/ {BGEZAL, CAUSES_BRANCH_DELAY | STORES_IN_31 | WRITES_REG, 0, 0,0,OpType::BGEZAL, *this},
    /*12*/ {BLTZALL, CAUSES_BRANCH_DELAY | STORES_IN_31 | WRITES_REG, 0, 0,0, OpType::BLTZALL, *this},
    /*13*/ {BGEZALL, CAUSES_BRANCH_DELAY | STORES_IN_31 | WRITES_REG, 0, 0,0, OpType::BGEZALL, *this},
         },
COP0rs_op_lut{
    /*00*/ {MFCz, WRITES_REG | STORES_IN_RT | READS_CP | CPZ, 0, 4,0, OpType::MFCz, *this},
    /*01*/ {DMFCz, WRITES_REG | STORES_IN_RT | READS_CP | CPZ, 0, 8,0, OpType::DMFCz, *this},
    /*02*/ {CFCz, WRITES_REG | STORES_IN_RT | READS_CP | CPControl | CPZ, 0, 0,0, OpType::CFCz, *this},
    /*03*/ {},
    /*04*/ {MTCz, WRITES_REG | WRITES_CP | STORES_IN_RD | CPZ, 0, 4,0, OpType::MTCz, *this},
    /*05*/ {DMTCz, WRITES_REG | WRITES_CP | STORES_IN_RD | CPZ, 0, 8,0, OpType::DMTCz, *this},
    /*06*/ {CTCz, WRITES_REG | WRITES_CP | CPControl | CPZ, 0, 0,0, OpType::CTCz, *this},
    /*07*/ {},
    /*08*/ {},
    /*09*/ {},
    /*0A*/ {},
    /*0B*/ {},
    /*0C*/ {},
    /*0D*/ {},
    /*0E*/ {},
    /*0F*/ {},
},
COP1rs_op_lut{
    /*00*/ {MFCz, WRITES_REG | STORES_IN_RT | READS_CP | CPZ, 0, 4,1, OpType::MFCz, *this},
    /*01*/ {DMFCz, WRITES_REG | STORES_IN_RT | READS_CP | CPZ, 0, 8,1, OpType::DMFCz, *this},
    /*02*/ {CFCz, WRITES_REG | STORES_IN_RT | READS_CP | CPControl | CPZ, 0, 0,1, OpType::CFCz, *this},
    /*03*/ {},
    /*04*/ {MTCz, WRITES_REG | WRITES_CP | STORES_IN_RD | CPZ, 0, 4,1, OpType::MTCz, *this}, // (unchanged, already correct)
    /*05*/ {DMTCz, WRITES_REG | WRITES_CP | STORES_IN_RD | CPZ, 0, 8,1, OpType::DMTCz, *this},
    /*06*/ {CTCz, WRITES_REG | WRITES_CP | CPControl | CPZ, 0, 0,1, OpType::CTCz, *this},
    /*07*/ {},
    /*08*/ {},
    /*09*/ {},
    /*0A*/ {},
    /*0B*/ {},
    /*0C*/ {},
    /*0D*/ {},
    /*0E*/ {},
    /*0F*/ {},
},
COP0rt_op_lut{
    /*00*/ {BCzF, CAUSES_BRANCH_DELAY | CPZ, 0, 0,0, OpType::BCzF, *this},
    /*01*/ {BCzT, CAUSES_BRANCH_DELAY | CPZ, 0, 0,0, OpType::BCzT, *this},
    /*02*/ {BCzFL, CAUSES_BRANCH_DELAY | CPZ, 0, 0,0, OpType::BCzFL, *this},
    /*03*/ {BCzTL, CAUSES_BRANCH_DELAY | CPZ, 0, 0,0, OpType::BCzTL, *this},
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
    /*1F*/ {},
},
COP1rt_op_lut{
    /*00*/ {BCzF, CAUSES_BRANCH_DELAY | CPZ, 0, 0,1, OpType::BCzF, *this},
    /*01*/ {BCzT, CAUSES_BRANCH_DELAY | CPZ, 0, 0,1, OpType::BCzT, *this},
    /*02*/ {BCzFL, CAUSES_BRANCH_DELAY | CPZ, 0, 0,1, OpType::BCzFL, *this},
    /*03*/ {BCzTL, CAUSES_BRANCH_DELAY | CPZ, 0, 0,1, OpType::BCzTL, *this},
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
    /*1F*/ {},
},
CP0_op_lut{
    /*00*/ {},
    /*01*/ {TLBR, CPZ, 0, 0,0, OpType::TLBR, *this},
    /*02*/ {TLBWI, CPZ, 0, 0,0, OpType::TLBWI, *this},
    /*03*/ {},
    /*04*/ {},
    /*05*/ {},
    /*06*/ {TLBWR, CPZ, 0, 0,0, OpType::TLBWR, *this},
    /*07*/ {},
    /*08*/ {TLBP, WRITES_REG | WRITES_CP | CPZ, 0, 0,0, OpType::TLBP, *this},
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
    /*18*/ {ERET, CPZ, 0, 0,0, OpType::ERET, *this},
    /*19*/ {},
    /*1A*/ {},
    /*1B*/ {},
    /*1C*/ {},
    /*1D*/ {},
    /*1E*/ {},
    /*1F*/ {},
},
CP1_op_lut{
    /*00*/ {ADDfmt, READS_CP | WRITES_CP | WRITES_REG | STORES_IN_SA | CPZ, 3, 8,1, OpType::ADDfmt, *this},
    /*01*/ {SUBfmt, READS_CP | WRITES_CP | WRITES_REG | STORES_IN_SA | CPZ, 3, 8,1, OpType::SUBfmt, *this},
    /*02*/ {MULfmt, READS_CP | WRITES_CP | WRITES_REG | STORES_IN_SA | CPZ, 5, 8,1, OpType::MULfmt, *this},
    /*03*/ {DIVfmt, READS_CP | WRITES_CP | WRITES_REG | STORES_IN_SA | CPZ, 29, 8,1, OpType::DIVfmt, *this},
    /*04*/ {SQRTfmt, READS_CP | WRITES_CP | WRITES_REG | STORES_IN_SA | CPZ, 29, 8,1, OpType::SQRTfmt, *this},
    /*05*/ {ABSfmt, READS_CP | WRITES_CP | WRITES_REG | STORES_IN_SA | CPZ, 0, 8,1, OpType::ABSfmt, *this},
    /*06*/ {MOVfmt, READS_CP | WRITES_CP | WRITES_REG | STORES_IN_SA | CPZ, 0, 8,1, OpType::MOVfmt, *this},
    /*07*/ {NEGfmt, READS_CP | WRITES_CP | WRITES_REG | STORES_IN_SA | CPZ, 0, 8,1, OpType::NEGfmt, *this},
    /*08*/ {ROUNDLfmt, WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | CPZ, 5, 8,1, OpType::CVTSfmt, *this},
    /*09*/ {TRUNCLfmt, WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | CPZ, 5, 8,1, OpType::CVTSfmt, *this},
    /*0A*/ {CEILLfmt, WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | CPZ, 5, 8,1, OpType::CVTSfmt, *this},
    /*0B*/ {FLOORLfmt, WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | CPZ, 5, 8,1, OpType::CVTSfmt, *this},
    /*0C*/ {ROUNDWfmt, WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | CPZ, 5, 8,1, OpType::CVTSfmt, *this},
    /*0D*/ {TRUNCWfmt, WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | CPZ, 5, 8,1, OpType::CVTSfmt, *this},
    /*0E*/ {CEILWfmt, WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | CPZ, 5, 8,1, OpType::CVTSfmt, *this},
    /*0F*/ {FLOORWfmt, WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | CPZ, 5, 8,1, OpType::CVTSfmt, *this},
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
    /*1F*/ {},
    /*20*/ {CVTSfmt, WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | CPZ, 5, 8,1, OpType::CVTSfmt, *this},
    /*21*/ {CVTDfmt, WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | CPZ, 5, 8,1, OpType::CVTDfmt, *this},
    /*22*/ {},
    /*23*/ {},
    /*24*/ {CVTWfmt, WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | CPZ, 5, 8,1, OpType::CVTWfmt, *this},
    /*25*/ {CVTLfmt, WRITES_REG | WRITES_CP | STORES_IN_SA | READS_CP | CPZ, 5, 8,1, OpType::CVTLfmt, *this},
    /*26*/ {},
    /*27*/ {},
    /*28*/ {},
    /*29*/ {},
    /*2A*/ {},
    /*2B*/ {},
    /*2C*/ {},
    /*2D*/ {},
    /*2E*/ {},
    /*2F*/ {},
    /*30*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*31*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*32*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*33*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*34*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*35*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*36*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*37*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*38*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*39*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*3A*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*3B*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*3C*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*3D*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*3E*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
    /*3F*/ {Ccondfmt, READS_CP | CPZ, 0, 8,1, OpType::Ccondfmt, *this},
}
{
    discard_bd = true;
    WB_in.op = op_storage;
    DC_in.op = op_storage + 1;
    EX_in.op = op_storage + 2;
    RF_in.op = op_storage + 3;
}