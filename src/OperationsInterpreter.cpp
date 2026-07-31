#include "VR4300Interpreter.h"
#include "OperationsInterpreter.h"
#include <bit>
#include <cmath>
#include <fenv.h>
#include "RCP.h"

#pragma STDC FENV_ACCESS ON

constexpr uint32_t rs(uint32_t op)    { return (op >> 21) & 0x1F; }
constexpr uint32_t rt(uint32_t op)    { return (op >> 16) & 0x1F; }
constexpr uint32_t rd(uint32_t op)    { return (op >> 11) & 0x1F; }
constexpr uint32_t sa(uint32_t op)    { return (op >>  6) & 0x1F; }
constexpr uint32_t funct(uint32_t op) { return op & 0x3F; }
constexpr uint32_t imm(uint32_t op)   { return op & 0xFFFF; }
constexpr uint32_t target(uint32_t op){ return op & 0x03FFFFFF; }
constexpr uint32_t cz(uint32_t op)    { return (op >> 26) & 0x3; }

void NOP(VR4300Interpreter &cpu, uint32_t op_code) {};

// Load/Store
void LB(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    int8_t byte = cpu.rcp->read_size(*p_addr,1);
    cpu.GPR[rt(op_code)] = byte;
}

void LBU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    uint8_t byte = cpu.rcp->read_size(*p_addr,1);
    cpu.GPR[rt(op_code)] = byte;
}

void LH(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    int16_t halfword = cpu.rcp->read_size(*p_addr,2);
    cpu.GPR[rt(op_code)] = halfword;
}

void LHU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    uint16_t halfword = cpu.rcp->read_size(*p_addr,2);
    cpu.GPR[rt(op_code)] = halfword;
}

void LW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    int32_t word = cpu.rcp->read_size(*p_addr,4);
    cpu.GPR[rt(op_code)] = word;
}

void LWL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];
    uint32_t alignment = data_addr & 3;
    uint64_t aligned_addr = data_addr & ~3;

    std::optional<uint32_t> p_addr = cpu.translate_address(aligned_addr);
    if (!p_addr)
        return;

    uint32_t word = cpu.rcp->read_size(*p_addr, 4);
    uint32_t current_rt32 = (uint32_t)cpu.GPR[rt(op_code)];

    static const uint32_t lwl_mask[4]  = { 0x00000000, 0x000000FF, 0x0000FFFF, 0x00FFFFFF };
    static const uint32_t lwl_shift[4] = { 0,          8,          16,         24         };

    int32_t merged = (current_rt32 & lwl_mask[alignment]) | (word << lwl_shift[alignment]);
    
    cpu.GPR[rt(op_code)] = merged;
}

void LWR(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];
    uint32_t alignment = data_addr & 3;
    uint64_t aligned_addr = data_addr & ~3;

    std::optional<uint32_t> p_addr = cpu.translate_address(aligned_addr);
    if (!p_addr)
        return;

    uint32_t word = cpu.rcp->read_size(*p_addr, 4);
    uint32_t current_rt32 = (uint32_t)cpu.GPR[rt(op_code)];

    static const uint32_t lwr_mask[4]  = { 0xFFFFFF00, 0xFFFF0000, 0xFF000000, 0x00000000 };
    static const uint32_t lwr_shift[4] = { 24,         16,         8,          0          };

    int32_t merged = (current_rt32 & lwr_mask[alignment]) | (word >> lwr_shift[alignment]);
    cpu.GPR[rt(op_code)] = merged;
}

void SB(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    uint8_t byte = cpu.GPR[rt(op_code)];

    cpu.rcp->write_size(*p_addr,byte,1);
}

void SH(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    uint16_t halfword = cpu.GPR[rt(op_code)];

    cpu.rcp->write_size(*p_addr,halfword,2);
}

void SW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    uint32_t word = cpu.GPR[rt(op_code)];

    cpu.rcp->write_size(*p_addr,word,4);
}

void SWL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];
    uint32_t alignment = data_addr & 3;
    uint64_t aligned_addr = data_addr & ~3;

    std::optional<uint32_t> p_addr = cpu.translate_address(aligned_addr);
    if (!p_addr)
        return;

    uint32_t mem_word = cpu.rcp->read_size(*p_addr, 4);
    uint32_t reg_word = (uint32_t)cpu.GPR[rt(op_code)];

    static const uint32_t swl_mem_mask[4]  = { 0x00000000, 0xFF000000, 0xFFFF0000, 0xFFFFFF00 };
    static const uint32_t swl_reg_shift[4] = { 0,          8,          16,         24         };

    uint32_t updated_word = (mem_word & swl_mem_mask[alignment]) | (reg_word >> swl_reg_shift[alignment]);

    cpu.rcp->write_size(*p_addr, updated_word, 4);
}

void SWR(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];
    uint32_t alignment = data_addr & 3;
    uint64_t aligned_addr = data_addr & ~3;

    std::optional<uint32_t> p_addr = cpu.translate_address(aligned_addr);
    if (!p_addr)
        return;

    uint32_t mem_word = cpu.rcp->read_size(*p_addr, 4);
    uint32_t reg_word = (uint32_t)cpu.GPR[rt(op_code)];

    static const uint32_t swr_mem_mask[4]  = { 0x00FFFFFF, 0x0000FFFF, 0x000000FF, 0x00000000 };
    static const uint32_t swr_reg_shift[4] = { 24,         16,         8,          0          };

    uint32_t updated_word = (mem_word & swr_mem_mask[alignment]) | (reg_word << swr_reg_shift[alignment]);

    cpu.rcp->write_size(*p_addr, updated_word, 4);
}

void LD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    int64_t doubleword = cpu.rcp->read_size(*p_addr,8);
    cpu.GPR[rt(op_code)] = doubleword;
}
void LDL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];
    uint32_t alignment = data_addr & 7;
    uint64_t aligned_addr = data_addr & ~7;

    std::optional<uint32_t> p_addr = cpu.translate_address(aligned_addr);
    if (!p_addr)
        return;

    uint64_t doubleword = cpu.rcp->read_size(*p_addr, 8);
    uint64_t current_rt = cpu.GPR[rt(op_code)];

    static const uint64_t ldl_mask[8] = {
        0x0000000000000000ULL, 0x00000000000000FFULL, 0x000000000000FFFFULL, 0x0000000000FFFFFFULL,
        0x00000000FFFFFFFFULL, 0x000000FFFFFFFFFFULL, 0x0000FFFFFFFFFFFFULL, 0x00FFFFFFFFFFFFFFULL
    };
    static const uint32_t ldl_shift[8] = { 0, 8, 16, 24, 32, 40, 48, 56 };

    uint64_t merged = (current_rt & ldl_mask[alignment]) | (doubleword << ldl_shift[alignment]);
    
    cpu.GPR[rt(op_code)] = merged;
}

void LDR(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];
    uint32_t alignment = data_addr & 7;
    uint64_t aligned_addr = data_addr & ~7;

    std::optional<uint32_t> p_addr = cpu.translate_address(aligned_addr);
    if (!p_addr)
        return;

    uint64_t doubleword = cpu.rcp->read_size(*p_addr, 8);
    uint64_t current_rt = cpu.GPR[rt(op_code)];

    static const uint64_t ldr_mask[8] = {
        0xFFFFFFFFFFFFFF00ULL, 0xFFFFFFFFFFFF0000ULL, 0xFFFFFFFFFF000000ULL, 0xFFFFFFFF00000000ULL,
        0xFFFFFF0000000000ULL, 0xFFFF000000000000ULL, 0xFF00000000000000ULL, 0x0000000000000000ULL
    };
    static const uint32_t ldr_shift[8] = { 56, 48, 40, 32, 24, 16, 8, 0 };

    uint64_t merged = (current_rt & ldr_mask[alignment]) | (doubleword >> ldr_shift[alignment]);
    
    cpu.GPR[rt(op_code)] = merged;
}
void LL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    int32_t word = cpu.rcp->read_size(*p_addr,4);
    cpu.GPR[rt(op_code)] = word;
    cpu.LLBit = 1;
}
void LLD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    int64_t doubleword = cpu.rcp->read_size(*p_addr,8);
    cpu.GPR[rt(op_code)] = doubleword;
    cpu.LLBit = 1;
}
void LWU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    uint32_t data = cpu.rcp->read_size(*p_addr,4);
    cpu.GPR[rt(op_code)] = data;
}
void SC(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    if(!cpu.LLBit){
        cpu.GPR[rt(op_code)] = 0;
        return;
    }

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    uint32_t word = cpu.GPR[rt(op_code)];

    cpu.rcp->write_size(*p_addr,word,4);

    cpu.GPR[rt(op_code)] = 1;

}

void SCD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    if(!cpu.LLBit){
        cpu.GPR[rt(op_code)] = 0;
        return;
    }

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    uint64_t doubleword = cpu.GPR[rt(op_code)];

    cpu.rcp->write_size(*p_addr,doubleword,8);

    cpu.GPR[rt(op_code)] = 1;

}
void SD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint8_t rt_n = rt(op_code);
    uint64_t rt_val = cpu.GPR[rt_n];

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    uint64_t doubleword = rt_val;

    cpu.rcp->write_size(*p_addr,doubleword,8);
}

void SDL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];
    uint32_t alignment = data_addr & 7;
    uint64_t aligned_addr = data_addr & ~7; 

    std::optional<uint32_t> p_addr = cpu.translate_address(aligned_addr);
    if (!p_addr)
        return;

    uint64_t mem_dword = cpu.rcp->read_size(*p_addr, 8);
    uint64_t reg_dword = cpu.GPR[rt(op_code)];

    static const uint64_t sdl_mem_mask[8] = {
        0x0000000000000000ULL, 0xFF00000000000000ULL, 0xFFFF000000000000ULL, 0xFFFFFF0000000000ULL,
        0xFFFFFFFF00000000ULL, 0xFFFFFFFFFF000000ULL, 0xFFFFFFFFFFFF0000ULL, 0xFFFFFFFFFFFFFF00ULL
    };
    static const uint32_t sdl_reg_shift[8] = { 0, 8, 16, 24, 32, 40, 48, 56 };

    uint64_t updated_dword = (mem_dword & sdl_mem_mask[alignment]) | (reg_dword >> sdl_reg_shift[alignment]);

    cpu.rcp->write_size(*p_addr, updated_dword, 8);
}

void SDR(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];
    uint32_t alignment = data_addr & 7;
    uint64_t aligned_addr = data_addr & ~7;

    std::optional<uint32_t> p_addr = cpu.translate_address(aligned_addr);
    if (!p_addr)
        return;

    uint64_t mem_dword = cpu.rcp->read_size(*p_addr, 8);
    uint64_t reg_dword = cpu.GPR[rt(op_code)];

    static const uint64_t sdr_mem_mask[8] = {
        0x00FFFFFFFFFFFFFFULL, 0x0000FFFFFFFFFFFFULL, 0x000000FFFFFFFFFFULL, 0x00000000FFFFFFFFULL,
        0x0000000000FFFFFFULL, 0x000000000000FFFFULL, 0x00000000000000FFULL, 0x0000000000000000ULL
    };
    static const uint32_t sdr_reg_shift[8] = { 56, 48, 40, 32, 24, 16, 8, 0 };

    uint64_t updated_dword = (mem_dword & sdr_mem_mask[alignment]) | (reg_dword << sdr_reg_shift[alignment]);

    cpu.rcp->write_size(*p_addr, updated_dword, 8);
}

// Computational
void ADDI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    int32_t result = (int16_t)(imm(op_code)) + rs_val;

    cpu.GPR[rt(op_code)] = result;
}

void ADDIU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    int32_t result = (int16_t)(imm(op_code)) + rs_val;

    cpu.GPR[rt(op_code)] = result;
}

void SLTI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t result = (int64_t)(int16_t)(imm(op_code)) > (int64_t)rs_val;

    cpu.GPR[rt(op_code)] = result;
}

void SLTIU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t result = (uint64_t)(int16_t)(imm(op_code)) > (uint64_t)rs_val;

    cpu.GPR[rt(op_code)] = result;
}

void ANDI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t result = ((imm(op_code)) & rs_val);

    cpu.GPR[rt(op_code)] = result;
}

void ORI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t result = ((imm(op_code)) | rs_val);

    cpu.GPR[rt(op_code)] = result;
}

void XORI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t result = ((imm(op_code)) ^ rs_val);

    cpu.GPR[rt(op_code)] = result;
}

void LUI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t result = (int32_t)((imm(op_code)) << 16);

    cpu.GPR[rt(op_code)] = result;
}

void DADDI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    int64_t result = (int16_t)(imm(op_code)) + rs_val;

    cpu.GPR[rt(op_code)] = result;
}

void DADDIU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    int64_t result = (int16_t)(imm(op_code)) + rs_val;

    cpu.GPR[rt(op_code)] = result;
}

void ADD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    int32_t result = rs_val + rt_val;

    cpu.GPR[rd(op_code)] = result;
}

void ADDU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    int32_t result = rs_val + rt_val;

    cpu.GPR[rd(op_code)] = result;
}

void SUB(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    int32_t result = rs_val - rt_val;

    cpu.GPR[rd(op_code)] = result;
}

void SUBU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    int32_t result = rs_val - rt_val;

    cpu.GPR[rd(op_code)] = result;
}

void SLT(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = (int64_t)rt_val > (int64_t)rs_val;

    cpu.GPR[rd(op_code)] = result;
}

void SLTU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = (uint64_t)rt_val > (uint64_t)rs_val;

    cpu.GPR[rd(op_code)] = result;
}

void AND(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = rt_val & rs_val;

    cpu.GPR[rd(op_code)] = result;
}

void OR(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = rt_val | rs_val;

    cpu.GPR[rd(op_code)] = result;
}

void XOR(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = rt_val ^ rs_val;

    cpu.GPR[rd(op_code)] = result;
}
void NOR(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = ~(rt_val | rs_val);

    cpu.GPR[rd(op_code)] = result;
}

void DADD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = rs_val + rt_val;

    cpu.GPR[rd(op_code)] = result;
}

void DADDU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = rs_val + rt_val;

    cpu.GPR[rd(op_code)] = result;
}

void DSUB(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = rs_val - rt_val;

    cpu.GPR[rd(op_code)] = result;
}

void DSUBU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = rs_val - rt_val;

    cpu.GPR[rd(op_code)] = result;
}

void SLL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rt_val = cpu.GPR[rt(op_code)];

    int32_t result =  rt_val << sa(op_code);

    cpu.GPR[rd(op_code)] = result;
}

void SRL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rt_val = cpu.GPR[rt(op_code)];

    int32_t result =  (uint32_t)rt_val >> sa(op_code);

    cpu.GPR[rd(op_code)] = result;
}

void SRA(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rt_val = cpu.GPR[rt(op_code)];

    int32_t result =  rt_val >> sa(op_code);

    cpu.GPR[rd(op_code)] = result;
}

void SLLV(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    int32_t result = rt_val << (rs_val & 0x1F);

    cpu.GPR[rd(op_code)] = result;
}
void SRLV(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    int32_t result = (uint32_t)rt_val >> (rs_val & 0x1F);

    cpu.GPR[rd(op_code)] = result;
}

void SRAV(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    int32_t result = rt_val >> (rs_val & 0x1F);

    cpu.GPR[rd(op_code)] = result;
}

void DSLL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result =  rt_val << sa(op_code);

    cpu.GPR[rd(op_code)] = result;
}
void DSRL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result =  rt_val >> sa(op_code);

    cpu.GPR[rd(op_code)] = result;
}

void DSRA(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result =  (int64_t)rt_val >> sa(op_code);

    cpu.GPR[rd(op_code)] = result;
}

void DSLLV(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = rt_val << (rs_val & 0x3F);

    cpu.GPR[rd(op_code)] = result;
}

void DSRLV(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = rt_val >> (rs_val & 0x3F);

    cpu.GPR[rd(op_code)] = result;
}

void DSRAV(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = (int64_t)rt_val >> (rs_val & 0x3F);

    cpu.GPR[rd(op_code)] = result;
}

void DSLL32(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result =  rt_val << (32 + sa(op_code));

    cpu.GPR[rd(op_code)] = result;
}
void DSRL32(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result =  rt_val >> (32 + sa(op_code));

    cpu.GPR[rd(op_code)] = result;
}

void DSRA32(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);
    
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result =  (int64_t)rt_val >> (32 + sa(op_code));

    cpu.GPR[rd(op_code)] = result;
}

void MULT(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    int64_t result = (int64_t)(int32_t)rs_val * (int64_t)(int32_t)rt_val;
    
    cpu.LO = (int32_t)(result);
    cpu.HI = (int32_t)(result >> 32);
}

void MULTU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t result = (uint64_t)(uint32_t)rs_val * (uint64_t)(uint32_t)rt_val;

    cpu.LO = (int32_t)(result);
    cpu.HI = (int32_t)(result >> 32);
}

void DIV(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(37);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    if (rt_val == 0)
    {
        cpu.LO = ((int32_t)rs_val < 0) ? 1 : -1;
        cpu.HI = rs_val;
        return;
    }

    if (rs_val == (int32_t)0x80000000 && rt_val == -1)
    {
        cpu.LO = (int32_t)0x80000000;
        cpu.HI = 0;
        return;
    }

    int32_t quotient = (int32_t)rs_val / (int32_t)rt_val;
    int32_t remainder = (int32_t)rs_val % (int32_t)rt_val;
    cpu.LO = quotient;
    cpu.HI = remainder;
}

void DIVU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(37);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    if (rt_val == 0)
    {
        cpu.LO = -1;
        cpu.HI = rs_val;
        return;
    }

    int32_t quotient = (uint32_t)rs_val / (uint32_t)rt_val;
    int32_t remainder = (uint32_t)rs_val % (uint32_t)rt_val;
    cpu.LO = quotient;
    cpu.HI = remainder;
}

void MFHI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    cpu.GPR[rd(op_code)] = cpu.HI;
}

void MFLO(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    cpu.GPR[rd(op_code)] = cpu.LO;
}

void MTHI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);
    
    uint64_t rs_val = cpu.GPR[rs(op_code)];
    cpu.HI = rs_val;
}
void MTLO(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    cpu.LO = rs_val;
}
void DMULT(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(8);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    __int128_t result = (__int128_t)(int64_t)rs_val * (__int128_t)(int64_t)rt_val;

    cpu.LO = result;
    cpu.HI = result >> 64;
}
void DMULTU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(8);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    __uint128_t result = (__uint128_t)(uint64_t)rs_val * (__uint128_t)(uint64_t)rt_val;

    cpu.LO = result;
    cpu.HI = result >> 64;
}
void DDIV(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(69);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    if (rt_val == 0)
    {
        cpu.LO = ((int64_t)rs_val < 0) ? 1 : -1;
        cpu.HI = rs_val;
        return;
    }

    if (rs_val == (int64_t)0x8000000000000000 && rt_val == -1)
    {
        cpu.LO = (int64_t)0x8000000000000000;
        cpu.HI = 0;
        return;
    }

    int64_t quotient = (int64_t)rs_val / (int64_t)rt_val;
    int64_t remainder = (int64_t)rs_val % (int64_t)rt_val;
    cpu.LO = quotient;
    cpu.HI = remainder;
}
void DDIVU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(69);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    if (rt_val == 0)
    {
        cpu.LO = -1;
        cpu.HI = rs_val;
        return;
    }

    int64_t quotient = (uint64_t)rs_val / (uint64_t)rt_val;
    int64_t remainder = (uint64_t)rs_val % (uint64_t)rt_val;
    cpu.LO = quotient;
    cpu.HI = remainder;
}

// Jump/Branch
void J(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    cpu.next_PC = ((cpu.PC + 4) & (~0xFFFFFFF)) | (target(op_code) << 2);

    if(!cpu.validate_PC_align(cpu.next_PC)) return;
    cpu.do_bd();
}

void JAL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    cpu.next_PC = ((cpu.PC + 4) & (~0xFFFFFFF)) | (target(op_code) << 2);
    cpu.GPR[31] = cpu.PC + 8;

    if(!cpu.validate_PC_align(cpu.next_PC)) return;
    cpu.do_bd();
}

void JR(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    cpu.next_PC = rs_val;

    if(!cpu.validate_PC_align(cpu.next_PC)) return;
    cpu.do_bd();
}

void JALR(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    cpu.next_PC = rs_val;
    
    uint32_t dest = rd(op_code);
    cpu.GPR[dest?dest:31] = cpu.PC + 8;

    if(!cpu.validate_PC_align(cpu.next_PC)) return;
    cpu.do_bd();
}

void BEQ(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if (rs_val == rt_val)
        cpu.next_PC = target;
    else
        cpu.next_PC = cpu.PC + 8;

    if(!cpu.validate_PC_align(cpu.next_PC)) return;
    cpu.do_bd();
}

void BNE(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if (rs_val != rt_val)
        cpu.next_PC = target;
    else
        cpu.next_PC = cpu.PC + 8;

    if(!cpu.validate_PC_align(cpu.next_PC)) return;
    cpu.do_bd();
}

void BLEZ(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((int64_t)rs_val <= 0)
        cpu.next_PC = target;
    else
        cpu.next_PC = cpu.PC + 8;

    if(!cpu.validate_PC_align(cpu.next_PC)) return;
    cpu.do_bd();
}

void BGTZ(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((int64_t)rs_val > 0)
        cpu.next_PC = target;
    else
        cpu.next_PC = cpu.PC + 8;

    if(!cpu.validate_PC_align(cpu.next_PC)) return;
    cpu.do_bd();
}

void BLTZ(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((int64_t)rs_val < 0)
        cpu.next_PC = target;
    else
        cpu.next_PC = cpu.PC + 8;

    if(!cpu.validate_PC_align(cpu.next_PC)) return;
    cpu.do_bd();
}

void BGEZ(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((int64_t)rs_val >= 0)
        cpu.next_PC = target;
    else
        cpu.next_PC = cpu.PC + 8;

    if(!cpu.validate_PC_align(cpu.next_PC)) return;
    cpu.do_bd();
}

void BLTZAL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((int64_t)rs_val < 0)
        cpu.next_PC = target;
    else
        cpu.next_PC = cpu.PC + 8;

    cpu.GPR[31] = cpu.PC + 8;

    if(!cpu.validate_PC_align(cpu.next_PC)) return;
    cpu.do_bd();
}

void BGEZAL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((int64_t)rs_val >= 0)
        cpu.next_PC = target;
    else
        cpu.next_PC = cpu.PC + 8;

    cpu.GPR[31] = cpu.PC + 8;

    if(!cpu.validate_PC_align(cpu.next_PC)) return;
    cpu.do_bd();
}

void BEQL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if (rs_val == rt_val){
        cpu.next_PC = target;
        if(!cpu.validate_PC_align(cpu.next_PC)) return;
        cpu.do_bd();
    }
    else
        cpu.next_PC = cpu.PC + 8;

}

void BNEL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if (rs_val != rt_val){
        cpu.next_PC = target;
        if(!cpu.validate_PC_align(cpu.next_PC)) return;
        cpu.do_bd();
    }
    else
        cpu.next_PC = cpu.PC + 8;

}
void BLEZL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((int64_t)rs_val <= 0){
        cpu.next_PC = target;
        if(!cpu.validate_PC_align(cpu.next_PC)) return;
        cpu.do_bd();
    }
    else
        cpu.next_PC = cpu.PC + 8;

}
void BGTZL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((int64_t)rs_val > 0){
        cpu.next_PC = target;
        if(!cpu.validate_PC_align(cpu.next_PC)) return;
        cpu.do_bd();
    }
    else
        cpu.next_PC = cpu.PC + 8;
}
void BLTZL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((int64_t)rs_val < 0){
        cpu.next_PC = target;
        if(!cpu.validate_PC_align(cpu.next_PC)) return;
        cpu.do_bd();
    }
    else
        cpu.next_PC = cpu.PC + 8;
}
void BGEZL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((int64_t)rs_val >= 0){
        cpu.next_PC = target;
        if(!cpu.validate_PC_align(cpu.next_PC)) return;
        cpu.do_bd();
    }
    else
        cpu.next_PC = cpu.PC + 8;
}

void BLTZALL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];

    cpu.GPR[31] = cpu.PC + 8;

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((int64_t)rs_val < 0){
        cpu.next_PC = target;
        if(!cpu.validate_PC_align(cpu.next_PC)) return;
        cpu.do_bd();
    }
    else
        cpu.next_PC = cpu.PC + 8;

}

void BGEZALL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    
    cpu.GPR[31] = cpu.PC + 8;

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((int64_t)rs_val >= 0){
        cpu.next_PC = target;
        if(!cpu.validate_PC_align(cpu.next_PC)) return;
        cpu.do_bd();
    }
    else
        cpu.next_PC = cpu.PC + 8;

}

// Special
void SYNC(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    NOP(cpu,op_code); // good thing i checked...
}

void SYSCALL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    cpu.handle_general_exception(cpu.PC, Sys);
}

void BREAK(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    cpu.handle_general_exception(cpu.PC, Bp);
}

void TGE(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];
    if ((int64_t)rs_val >= (int64_t)rt_val)
        cpu.handle_general_exception(cpu.PC, Tr);
}

void TGEU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];
    if (rs_val >= rt_val)
        cpu.handle_general_exception(cpu.PC, Tr);
}

void TLT(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];
    if ((int64_t)rs_val < (int64_t)rt_val)
        cpu.handle_general_exception(cpu.PC, Tr);
}

void TLTU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];
    if (rs_val < rt_val)
        cpu.handle_general_exception(cpu.PC, Tr);
}

void TEQ(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];
    if (rs_val == rt_val)
        cpu.handle_general_exception(cpu.PC, Tr);
}

void TNE(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    uint64_t rt_val = cpu.GPR[rt(op_code)];
    if (rs_val != rt_val)
        cpu.handle_general_exception(cpu.PC, Tr);
}

void TGEI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    if ((int64_t)rs_val >= (int16_t)(op_code & 0xFFFF))
        cpu.handle_general_exception(cpu.PC, Tr);
}

void TGEIU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    if (rs_val >= (op_code & 0xFFFF))
        cpu.handle_general_exception(cpu.PC, Tr);
}

void TLTI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    if ((int64_t)rs_val < (int16_t)(op_code & 0xFFFF))
        cpu.handle_general_exception(cpu.PC, Tr);
}

void TLTIU(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    if (rs_val < (op_code & 0xFFFF))
        cpu.handle_general_exception(cpu.PC, Tr);
}

// immidiate is sign extended here, it doesn't say in general description, good i checked...
void TEQI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    if (rs_val == (uint64_t)(int16_t)(op_code & 0xFFFF))
        cpu.handle_general_exception(cpu.PC, Tr);
}

void TNEI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint64_t rs_val = cpu.GPR[rs(op_code)];
    if (rs_val == (uint64_t)(int16_t)(op_code & 0xFFFF))
        cpu.handle_general_exception(cpu.PC, Tr);
}

void LWCz(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    int32_t word = cpu.rcp->read_size(*p_addr,4);

    if(cz(op_code) == 0){
        cpu.cp0.set_reg(word,rt(op_code));
    }else if(cz(op_code) == 1){
        cpu.fpu.set_fpr(word,rt(op_code), 4);
    }
}

void SWCz(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    uint32_t word = 0;

    if(cz(op_code) == 0){
        word = cpu.cp0.get_reg(rt(op_code));
    }else if(cz(op_code) == 1){
        word = cpu.fpu.get_fpr(rt(op_code), 4);
    }

    cpu.rcp->write_size(*p_addr,word,4);
}

void MTCz(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t rt_val = cpu.GPR[rt(op_code)];
    if(cz(op_code) == 0){
        cpu.cp0.set_reg(rt_val,rd(op_code));
    }else if(cz(op_code) == 1){
        cpu.fpu.set_fpr(rt_val,rd(op_code), 4);
    }
}

void MFCz(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t val = 0;

    if(cz(op_code) == 0){
        val = cpu.cp0.get_reg(rd(op_code));
    }else if(cz(op_code) == 1){
        val = cpu.fpu.get_fpr(rd(op_code), 4);
    }

    cpu.GPR[rt(op_code)] = (int32_t)val;
}

void CTCz(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t rt_val = cpu.GPR[rt(op_code)];

    if(cz(op_code) == 1){
        cpu.fpu.set_control(rt_val, rd(op_code));
    }
}

void CFCz(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);
    
    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t result = 0;
    
    if(cz(op_code) == 1){
        result = cpu.fpu.get_control(rd(op_code));
        cpu.GPR[rt(op_code)] = (int32_t)result;
    }
}

void COPz(VR4300Interpreter &cpu, uint32_t op_code)
{
}

void BCzT(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((cpu.fpu.FCR31 & (1<<23)))
        cpu.next_PC = target;
    else
        cpu.next_PC = cpu.PC + 8;

    cpu.do_bd();
}


void BCzF(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if (!(cpu.fpu.FCR31 & (1<<23)))
        cpu.next_PC = target;
    else
        cpu.next_PC = cpu.PC + 8;

    cpu.do_bd();
}

void DMTCz(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t rt_val = cpu.GPR[rt(op_code)];
    if(cz(op_code) == 0){
        cpu.cp0.set_reg(rt_val,rd(op_code));
    }else if(cz(op_code) == 1){
        cpu.fpu.set_fpr(rt_val,rd(op_code), 8);
    }
}
void DMFCz(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t val = 0;

    if(cz(op_code) == 0){
        val = cpu.cp0.get_reg(rd(op_code));
    }else if(cz(op_code) == 1){
        val = cpu.fpu.get_fpr(rd(op_code), 8);
    }

    cpu.GPR[rt(op_code)] = val;
}

void LDCz(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    int64_t word = cpu.rcp->read_size(*p_addr,8);

    if(cz(op_code) == 0){
        cpu.cp0.set_reg(word,rt(op_code));
    }else if(cz(op_code) == 1){
        cpu.fpu.set_fpr(word,rt(op_code), 8);
    }
}
void SDCz(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t data_addr = (int16_t)(imm(op_code)) + cpu.GPR[rs(op_code)];

    std::optional<uint32_t> p_addr = cpu.translate_address(data_addr);

    if(!p_addr)
        return;

    uint64_t doubleword = 0;

    if(cz(op_code) == 0){
        doubleword = cpu.cp0.get_reg(rt(op_code));
    }else if(cz(op_code) == 1){
        doubleword = cpu.fpu.get_fpr(rt(op_code), 8);
    }

    cpu.rcp->write_size(*p_addr,doubleword,8);
}

void BCzTL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;
    
    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if ((cpu.fpu.FCR31 & (1<<23))){
        cpu.next_PC = target;
        cpu.do_bd();
    }
    else
        cpu.next_PC = cpu.PC + 8;
}

void BCzFL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;
    
    uint64_t target = cpu.PC + 4 + ((int16_t)imm(op_code) << 2);
    if (!(cpu.fpu.FCR31 & (1<<23))){
        cpu.next_PC = target;
        cpu.do_bd();
    }
    else
        cpu.next_PC = cpu.PC + 8;
}

void TLBR(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint8_t tlb_index = cpu.cp0.index & 0x3F;
    uint64_t G = (cpu.cp0.TLB[tlb_index][1] >> 12) & 1;
    cpu.cp0.pageMask = cpu.cp0.TLB[tlb_index][0];
    cpu.cp0.entryHi = cpu.cp0.TLB[tlb_index][1] & ~(1 << 12);
    cpu.cp0.entryLo0 = cpu.cp0.TLB[tlb_index][2] + G;
    cpu.cp0.entryLo1 = cpu.cp0.TLB[tlb_index][3] + G;
}

void TLBWI(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint8_t tlb_index = cpu.cp0.index & 0x3F;
    uint64_t G = (cpu.cp0.entryLo0 & 1) && (cpu.cp0.entryLo1 & 1);
    uint64_t pagemask_high_bits = cpu.cp0.pageMask & (0b101010101010ULL << 13);
    cpu.cp0.TLB[tlb_index][0] = pagemask_high_bits | (pagemask_high_bits >> 1);
    cpu.cp0.TLB[tlb_index][1] = cpu.cp0.entryHi & ~cpu.cp0.TLB[tlb_index][0] & ~(G << 12) | (G << 12);
    cpu.cp0.TLB[tlb_index][2] = cpu.cp0.entryLo0 & (0x3fffffe);
    cpu.cp0.TLB[tlb_index][3] = cpu.cp0.entryLo1 & (0x3fffffe);
}

void TLBWR(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    uint8_t tlb_index = cpu.cp0.random & 0x3F;
    uint64_t G = (cpu.cp0.entryLo0 & 1) && (cpu.cp0.entryLo1 & 1);
    uint64_t pagemask_high_bits = cpu.cp0.pageMask & (0b101010101010ULL << 13);
    cpu.cp0.TLB[tlb_index][0] = pagemask_high_bits | (pagemask_high_bits >> 1);
    cpu.cp0.TLB[tlb_index][1] = cpu.cp0.entryHi & ~cpu.cp0.TLB[tlb_index][0] & ~(G << 12) | (G << 12);
    cpu.cp0.TLB[tlb_index][2] = cpu.cp0.entryLo0 & (0x3fffffe);
    cpu.cp0.TLB[tlb_index][3] = cpu.cp0.entryLo1 & (0x3fffffe);
}

// thsi will need to write the index reg
void TLBP(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

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
            cpu.cp0.index = i & 0x3F;
            return;
        }
    }
    cpu.cp0.index = 1 << 31;
}

void ERET(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if (cpu.cp0.status & STATUS_ERL_MASK)
    {
        cpu.next_PC = cpu.cp0.errorEPC;
        cpu.cp0.status = cpu.cp0.set_bits(cpu.cp0.status, STATUS_ERL_MASK, 0);
        cpu.cp0.stash_status();
    }
    else
    {
        cpu.next_PC = cpu.cp0.EPC;
        cpu.cp0.status = cpu.cp0.set_bits(cpu.cp0.status, STATUS_EXL_MASK, 0);
        cpu.cp0.stash_status();
    }
    cpu.LLBit = 0;
}

void CACHE(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);
}

void CVTSS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();

    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);

    if ((cpu.fpu.FCR31 >> 12) & 0x3F);

    cpu.check_fpu_exception(cpu.PC);
}

void CVTSD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(2);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
    float converted = (float)interpreted;
    uint64_t result = (uint64_t)std::bit_cast<uint32_t>(converted);

    invalid = std::isnan(interpreted) && ((fpr_val >> 51) & 1);
    if (invalid)
    {
        result = 0x7fbfffff;
        goto finish;
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
        goto finish;
    }
    if ((underflow || (std::fpclassify(converted) == FP_SUBNORMAL)) && FS_BIT)
    {
        converted = cpu.fpu.flush_float(converted);
        result = (uint64_t)std::bit_cast<uint32_t>(converted);
    }

    inexact = (converted != interpreted);
    overflow = std::isinf(converted) && std::isfinite(interpreted);

finish:

    cpu.fpu.set_cause(inexact, underflow, overflow, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void CVTSW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    int32_t interpreted = (int32_t)fpr_val;
    float converted =  (float)interpreted;
    uint64_t result = (uint64_t)std::bit_cast<uint32_t>(converted);
    inexact = ((int32_t)converted != interpreted);

    cpu.fpu.set_cause(inexact, underflow, overflow, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void CVTSL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    int64_t interpreted = (int64_t)fpr_val;
    float converted = (float)interpreted;
    uint64_t result = (uint64_t)std::bit_cast<uint32_t>(converted);
    unimplemented = interpreted != -(1LL << 55) && (std::abs(interpreted) >= (1ULL << 55));
    
    if (unimplemented)
        goto finish;

    inexact = ((int64_t)converted != interpreted);
    overflow = std::fabs(interpreted) > std::numeric_limits<float>::max();

finish:

    cpu.fpu.set_cause(inexact, underflow, overflow, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}


void CVTDS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;
    bool invalid = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
    double converted = (double)interpreted;
    uint64_t result = std::bit_cast<uint64_t>(converted);

    invalid = std::isnan(interpreted) && ((fpr_val >> 22) & 1);
    if (invalid)
    {
        result = 0x7ff7ffffffffffff;
        goto finish;
    }

    unimplemented = ((std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                     std::isnan(interpreted) && !((fpr_val >> 22) & 1) ||
                     ((std::fpclassify(converted) == FP_SUBNORMAL) && FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                     (FS_BIT && (cpu.fpu.underflow_enabled() || cpu.fpu.inexact_enabled())) ||
                     ((std::fpclassify(converted) == FP_SUBNORMAL) && !FS_BIT));

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void CVTDD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void CVTDW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);
    
    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;
    bool invalid = false;

    int32_t interpreted = (int32_t)fpr_val;
    double converted = (double)interpreted;
    uint64_t result = std::bit_cast<uint64_t>(converted);
    inexact = ((int32_t)converted != interpreted);

    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void CVTDL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;
    bool invalid = false;

    int64_t interpreted = (int64_t)fpr_val;
    double converted = (double)interpreted;
    uint64_t result = std::bit_cast<uint64_t>(converted);

    unimplemented = interpreted != -(1LL << 55) && (std::abs(interpreted) >= (1ULL << 55));
    if (unimplemented)
        goto finish;

    inexact = ((int64_t)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void CVTLS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
    float rounded = std::nearbyintf(interpreted);
    int64_t converted = (int64_t)rounded;
    uint64_t result = (uint64_t)converted;

    unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((float)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void CVTLD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
    double rounded = std::nearbyintl(interpreted);
    int64_t converted = (int64_t)rounded;
    uint64_t result = (uint64_t)converted;

    unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((double)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void CVTLW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void CVTLL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void CVTWS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
    float rounded = std::nearbyintf(interpreted);
    int32_t converted = (int32_t)rounded;
    uint64_t result = (uint64_t)converted;

    unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                    (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((float)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void CVTWD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
    double rounded = std::nearbyintl(interpreted);
    int32_t converted = (int32_t)rounded;
    uint64_t result = (uint64_t)converted;

    unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                    (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((double)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void CVTWW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void CVTWL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}
void ROUNDLS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
    int64_t converted = (int64_t)roundevenf(interpreted);
    uint64_t result = (uint64_t)converted;

    unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((float)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void ROUNDLD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
    int64_t converted = (int64_t)roundevenl(interpreted);
    uint64_t result = (uint64_t)converted;

    unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((double)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void ROUNDLW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void ROUNDLL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void ROUNDWS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
    float rounded = roundevenf(interpreted);
    int32_t converted = (int32_t)rounded;
    uint64_t result = (uint64_t)std::bit_cast<uint32_t>(converted);

    unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                    (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((float)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void ROUNDWD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
    double rounded = roundevenl(interpreted);
    int32_t converted = (int32_t)rounded;
    uint64_t result = (uint64_t)std::bit_cast<uint32_t>(converted);

    unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                    (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((double)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void ROUNDWW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void ROUNDWL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void TRUNCLS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
    int64_t converted = (int64_t)std::round(interpreted);
    uint64_t result = (uint64_t)converted;

    unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((float)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void TRUNCLD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
    int64_t converted = (int64_t)std::round(interpreted);
    uint64_t result = (uint64_t)converted;

    unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((double)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void TRUNCLW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void TRUNCLL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}
void TRUNCWS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
    float rounded = std::trunc(interpreted);
    int32_t converted = (int32_t)rounded;
    uint64_t result = (uint64_t)std::bit_cast<uint32_t>(converted);

    unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                    (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((float)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void TRUNCWD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
    double rounded = std::trunc(interpreted);
    int32_t converted = (int32_t)rounded;
    uint64_t result = (uint64_t)std::bit_cast<uint32_t>(converted);

    unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                    (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((double)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void TRUNCWW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void TRUNCWL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}
void CEILLS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
    int64_t converted = (int64_t)std::ceil(interpreted);
    uint64_t result = (uint64_t)converted;

    unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((float)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void CEILLD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
    int64_t converted = (int64_t)std::ceil(interpreted);
    uint64_t result = (uint64_t)converted;

    unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((double)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void CEILLW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void CEILLL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void CEILWS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
    float rounded = std::ceil(interpreted);
    int32_t converted = (int32_t)rounded;
    uint64_t result = (uint64_t)std::bit_cast<uint32_t>(converted);

    unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                    (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((float)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void CEILWD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
    double rounded = std::ceil(interpreted);
    int32_t converted = (int32_t)rounded;
    uint64_t result = (uint64_t)std::bit_cast<uint32_t>(converted);

    unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                    (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((double)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void CEILWW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void CEILWL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void FLOORLS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
    int64_t converted = (int64_t)std::floor(interpreted);
    uint64_t result = (uint64_t)converted;

    unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((float)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void FLOORLD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool unimplemented = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
    int64_t converted = (int64_t)std::floor(interpreted);
    uint64_t result = (uint64_t)converted;

    unimplemented = (std::abs(converted) >= (1ULL << 53)) ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((double)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, 0, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void FLOORLW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void FLOORLL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}
void FLOORWS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
    float rounded = std::floor(interpreted);
    int32_t converted = (int32_t)rounded;
    uint64_t result = (uint64_t)std::bit_cast<uint32_t>(converted);

    unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                    (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((float)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void FLOORWD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;
    bool overflow = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
    double rounded = std::floor(interpreted);
    int32_t converted = (int32_t)rounded;
    uint64_t result = (uint64_t)std::bit_cast<uint32_t>(converted);

    unimplemented = (double)rounded > (double)std::numeric_limits<int32_t>::max() ||
                    (double)rounded < (double)std::numeric_limits<int32_t>::min() ||
                    (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
                    std::isnan(interpreted) ||
                    std::isinf(interpreted);
    if (unimplemented)
        goto finish;

    inexact = ((double)converted != interpreted);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void FLOORWW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void FLOORWL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

// fpu computational
void ADDS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(3);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr1_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint64_t fpr2_val = cpu.fpu.get_fpr(rt(op_code), 8);
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    float operand1 = std::bit_cast<float>((uint32_t)fpr1_val);
    float operand2 = std::bit_cast<float>((uint32_t)fpr2_val);

    feclearexcept(FE_INEXACT);
    float result = operand1 + operand2;
    inexact = (bool)fetestexcept(FE_INEXACT);
    uint64_t result_bit = (uint64_t)std::bit_cast<uint32_t>(result);

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
        goto finish;
    }

    invalid = std::isnan(operand1) && ((fpr1_val >> 22) & 1) ||
              std::isnan(operand2) && ((fpr2_val >> 22) & 1) ||
              std::isinf(operand1) && std::isinf(operand2);
    if (invalid)
    {
        inexact = 0;
        underflow = 0;
        result_bit = 0x7fbfffff;
        goto finish;
    }

    if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
    {
        result = cpu.fpu.flush_float(result);
        result_bit = (uint64_t)std::bit_cast<uint32_t>(result);
    }

    overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);

finish:
    cpu.fpu.set_cause(inexact, underflow, overflow, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result_bit, sa(op_code), 8);
}

void ADDD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(3);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr1_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint64_t fpr2_val = cpu.fpu.get_fpr(rt(op_code), 8);
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    double operand1 = std::bit_cast<double>((uint64_t)fpr1_val);
    double operand2 = std::bit_cast<double>((uint64_t)fpr2_val);

    feclearexcept(FE_INEXACT);
    double result = operand1 + operand2;
    inexact = (bool)fetestexcept(FE_INEXACT);
    uint64_t result_bit = (uint64_t)std::bit_cast<uint64_t>(result);

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
        goto finish;
    }

    invalid = std::isnan(operand1) && ((fpr1_val >> 51) & 1) ||
              std::isnan(operand2) && ((fpr2_val >> 51) & 1) ||
              std::isinf(operand1) && std::isinf(operand2);
    if (invalid)
    {
        inexact = 0;
        underflow = 0;
        result_bit = 0x7ff7ffffffffffff;
        goto finish;
    }

    if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
    {
        result = cpu.fpu.flush_double(result);
        result_bit = (uint64_t)std::bit_cast<uint64_t>(result);
    }

    overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);

finish:
    cpu.fpu.set_cause(inexact, underflow, overflow, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result_bit, sa(op_code), 8);
}

void ADDW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void ADDL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void SUBS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(3);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr1_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint64_t fpr2_val = cpu.fpu.get_fpr(rt(op_code), 8);
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    float operand1 = std::bit_cast<float>((uint32_t)fpr1_val);
    float operand2 = std::bit_cast<float>((uint32_t)fpr2_val);

    feclearexcept(FE_INEXACT);
    float result = operand1 - operand2;
    inexact = (bool)fetestexcept(FE_INEXACT);
    uint64_t result_bit = (uint64_t)std::bit_cast<uint32_t>(result);

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
        underflow = 0;
        goto finish;
    }

    invalid = std::isnan(operand1) && ((fpr1_val >> 22) & 1) ||
              std::isnan(operand2) && ((fpr2_val >> 22) & 1) ||
              std::isinf(operand1) && std::isinf(operand2);
    if (invalid)
    {
        inexact = 0;
        underflow = 0;
        result_bit = 0x7fbfffff;
        goto finish;
    }

    if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
    {
        result = cpu.fpu.flush_float(result);
        result_bit = (uint64_t)std::bit_cast<uint32_t>(result);
    }

    overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);

finish:
    cpu.fpu.set_cause(inexact, underflow, overflow, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result_bit, sa(op_code), 8);
}

void SUBD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(3);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr1_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint64_t fpr2_val = cpu.fpu.get_fpr(rt(op_code), 8);
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    double operand1 = std::bit_cast<double>((uint64_t)fpr1_val);
    double operand2 = std::bit_cast<double>((uint64_t)fpr2_val);

    feclearexcept(FE_INEXACT);
    double result = operand1 - operand2;
    inexact = (bool)fetestexcept(FE_INEXACT);
    uint64_t result_bit = (uint64_t)std::bit_cast<uint64_t>(result);

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
        underflow = 0;
        goto finish;
    }

    invalid = std::isnan(operand1) && ((fpr1_val >> 51) & 1) ||
              std::isnan(operand2) && ((fpr2_val >> 51) & 1) ||
              std::isinf(operand1) && std::isinf(operand2);
    if (invalid)
    {
        inexact = 0;
        underflow = 0;
        result_bit = 0x7ff7ffffffffffff;
        goto finish;
    }

    if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
    {
        result = cpu.fpu.flush_double(result);
        result_bit = (uint64_t)std::bit_cast<uint64_t>(result);
    }

    overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);

finish:
    cpu.fpu.set_cause(inexact, underflow, overflow, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result_bit, sa(op_code), 8);
}

void SUBW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void SUBL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}
void MULS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(5);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr1_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint64_t fpr2_val = cpu.fpu.get_fpr(rt(op_code), 8);
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    float operand1 = std::bit_cast<float>((uint32_t)fpr1_val);
    float operand2 = std::bit_cast<float>((uint32_t)fpr2_val);

    feclearexcept(FE_INEXACT);
    float result = operand1 * operand2;
    inexact = (bool)fetestexcept(FE_INEXACT);
    uint64_t result_bit = (uint64_t)std::bit_cast<uint32_t>(result);

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
        underflow = 0;
        goto finish;
    }

    invalid = std::isnan(operand1) && ((fpr1_val >> 22) & 1) ||
              std::isnan(operand2) && ((fpr2_val >> 22) & 1) ||
              std::isinf(operand1) && (operand2 == 0) ||
              std::isinf(operand2) && (operand1 == 0);
    if (invalid)
    {
        inexact = 0;
        underflow = 0;
        result_bit = 0x7fbfffff;
        goto finish;
    }

    if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
    {
        result = cpu.fpu.flush_float(result);
        result_bit = (uint64_t)std::bit_cast<uint32_t>(result);
    }

    overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);

finish:
    cpu.fpu.set_cause(inexact, underflow, overflow, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result_bit, sa(op_code), 8);
}

void MULD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(8);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr1_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint64_t fpr2_val = cpu.fpu.get_fpr(rt(op_code), 8);
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;

    double operand1 = std::bit_cast<double>((uint64_t)fpr1_val);
    double operand2 = std::bit_cast<double>((uint64_t)fpr2_val);

    feclearexcept(FE_INEXACT);
    double result = operand1 * operand2;
    inexact = (bool)fetestexcept(FE_INEXACT);
    uint64_t result_bit = (uint64_t)std::bit_cast<uint64_t>(result);

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
        underflow = 0;
        goto finish;
    }

    invalid = std::isnan(operand1) && ((fpr1_val >> 51) & 1) ||
              std::isnan(operand2) && ((fpr2_val >> 51) & 1) ||
              std::isinf(operand1) && (operand2 == 0) ||
              std::isinf(operand2) && (operand1 == 0);
    if (invalid)
    {
        inexact = 0;
        underflow = 0;
        result_bit = 0x7ff7ffffffffffff;
        goto finish;
    }

    if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
    {
        result = cpu.fpu.flush_double(result);
        result_bit = (uint64_t)std::bit_cast<uint64_t>(result);
    }

    overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);

finish:
    cpu.fpu.set_cause(inexact, underflow, overflow, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result_bit, sa(op_code), 8);
}

void MULW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void MULL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}
void DIVS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(29);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr1_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint64_t fpr2_val = cpu.fpu.get_fpr(rt(op_code), 8);
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;
    bool zerodiv = false;

    float operand1 = std::bit_cast<float>((uint32_t)fpr1_val);
    float operand2 = std::bit_cast<float>((uint32_t)fpr2_val);

    feclearexcept(FE_INEXACT);
    float result = operand1 / operand2;
    inexact = (bool)fetestexcept(FE_INEXACT);
    uint64_t result_bit = (uint64_t)std::bit_cast<uint32_t>(result);

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
        underflow = 0;
        goto finish;
    }

    invalid = (std::isnan(operand1) && ((fpr1_val >> 22) & 1)) ||
              (std::isnan(operand2) && ((fpr2_val >> 22) & 1)) ||
              ((operand1 == 0.f) && (operand2 == 0.f)) ||
              (std::isinf(operand2) && std::isinf(operand1));
    if (invalid)
    {
        inexact = 0;
        underflow = 0;
        result_bit = 0x7fbfffff;
        goto finish;
    }

    if (operand2 == 0.f)
    {
        zerodiv = true;
        goto finish;
    }

    underflow =
        (operand1 != 0.0 && std::isfinite(operand2) && result == 0.0) ||
        (std::fpclassify(result) == FP_SUBNORMAL);
    inexact = inexact || underflow;

    if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
    {
        result = cpu.fpu.flush_float(result);
        result_bit = (uint64_t)std::bit_cast<uint32_t>(result);
    }

    overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);

finish:
    cpu.fpu.set_cause(inexact, underflow, overflow, zerodiv, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result_bit, sa(op_code), 8);
}

void DIVD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(58);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr1_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint64_t fpr2_val = cpu.fpu.get_fpr(rt(op_code), 8);
    uint8_t FS_BIT = (cpu.fpu.FCR31 >> CONTROL_FS_SHIFT) & CONTROL_FS_MASK;
    cpu.fpu.clear_cause();

    bool inexact = false;
    bool overflow = false;
    bool underflow = false;
    bool unimplemented = false;
    bool invalid = false;
    bool zerodiv = false;

    double operand1 = std::bit_cast<double>((uint64_t)fpr1_val);
    double operand2 = std::bit_cast<double>((uint64_t)fpr2_val);

    feclearexcept(FE_INEXACT);
    double result = operand1 / operand2;
    inexact = (bool)fetestexcept(FE_INEXACT);
    uint64_t result_bit = (uint64_t)std::bit_cast<uint64_t>(result);

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
        underflow = 0;
        goto finish;
    }

    invalid = (std::isnan(operand1) && ((fpr1_val >> 51) & 1)) ||
              (std::isnan(operand2) && ((fpr2_val >> 51) & 1)) ||
              ((operand1 == 0.) && (operand2 == 0.)) ||
              (std::isinf(operand2) && std::isinf(operand1));
    if (invalid)
    {
        inexact = 0;
        underflow = 0;
        result_bit = 0x7ff7ffffffffffff;
        goto finish;
    }

    if (operand2 == 0.)
    {
        zerodiv = true;
        goto finish;
    }

    underflow =
        (operand1 != 0.0 && std::isfinite(operand2) && result == 0.0) ||
        (std::fpclassify(result) == FP_SUBNORMAL);
    inexact = inexact || underflow;

    if ((underflow || (std::fpclassify(result) == FP_SUBNORMAL)) && FS_BIT)
    {
        result = cpu.fpu.flush_double(result);
        result_bit = (uint64_t)std::bit_cast<uint64_t>(result);
    }

    overflow = std::isinf(result) && std::isfinite(operand1) && std::isfinite(operand2);

finish:
    cpu.fpu.set_cause(inexact, underflow, overflow, zerodiv, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result_bit, sa(op_code), 8);
}

void DIVW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void DIVL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}
void ABSS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
    float absolute = std::abs(interpreted);
    uint64_t result = (uint64_t)std::bit_cast<uint32_t>(absolute);

    invalid = std::isnan(interpreted) && ((fpr_val >> 22) & 1);
    if (invalid)
    {
        result = 0x7fbfffff;
        goto finish;
    }

    unimplemented =
        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
        std::isnan(interpreted);

finish:
    cpu.fpu.set_cause(0, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void ABSD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
    double absolute = std::abs(interpreted);
    uint64_t result = (uint64_t)std::bit_cast<uint64_t>(absolute);

    invalid = std::isnan(interpreted) && ((fpr_val >> 51) & 1);
    if (invalid)
    {
        result = 0x7ff7ffffffffffff;
        goto finish;
    }

    unimplemented =
        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
        std::isnan(interpreted);

finish:
    cpu.fpu.set_cause(0, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void ABSW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void ABSL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}
void MOVS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.set_fpr(fpr_val, sa(op_code), 8);
}

void MOVD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.set_fpr(fpr_val, sa(op_code), 8);
}

void MOVW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void MOVL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void NEGS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);
    float negated = -interpreted;
    uint64_t result = (uint64_t)std::bit_cast<uint32_t>(negated);

    invalid = std::isnan(interpreted) && ((fpr_val >> 22) & 1);
    if (invalid)
    {
        result = 0x7fbfffff;
        goto finish;
    }

    unimplemented =
        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
        std::isnan(interpreted);

finish:
    cpu.fpu.set_cause(0, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void NEGD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;
        
    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);
    double negated = -interpreted;
    uint64_t result = (uint64_t)std::bit_cast<uint64_t>(negated);

    invalid = std::isnan(interpreted) && ((fpr_val >> 51) & 1);
    if (invalid)
    {
        result = 0x7ff7ffffffffffff;
        goto finish;
    }

    unimplemented =
        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
        std::isnan(interpreted);

finish:
    cpu.fpu.set_cause(0, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void NEGW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void NEGL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}
void SQRTS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(29);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    float root_result = 0;
    double accurate = 0;
    uint64_t result = 0;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;

    float interpreted = std::bit_cast<float>((uint32_t)fpr_val);

    invalid = (std::isnan(interpreted) && ((fpr_val >> 22) & 1)) ||
              (interpreted < 0 && !(std::fpclassify(interpreted) == FP_SUBNORMAL));
    if (invalid)
    {
        result = 0x7fbfffff;
        goto finish;
    }

    unimplemented =
        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
        std::isnan(interpreted);
    if (unimplemented)
        goto finish;

    root_result = std::sqrtf(interpreted);
    accurate = std::sqrt((double)interpreted);
    result = (uint64_t)std::bit_cast<uint32_t>(root_result);

    inexact = ((double)root_result != accurate);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void SQRTD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(58);

    if(!cpu.cp_usable(cz(op_code)))
        return;
    
    double root_result = 0;
    long double accurate = 0;
    uint64_t result = 0;

    uint64_t fpr_val = cpu.fpu.get_fpr(rd(op_code), 8);
    cpu.fpu.clear_cause();

    bool invalid = false;
    bool unimplemented = false;
    bool inexact = false;

    double interpreted = std::bit_cast<double>((uint64_t)fpr_val);

    invalid = (std::isnan(interpreted) && ((fpr_val >> 51) & 1)) ||
              (interpreted < 0 && !(std::fpclassify(interpreted) == FP_SUBNORMAL));
    if (invalid)
    {
        result = 0x7ff7ffffffffffff;
        goto finish;
    }

    unimplemented =
        (std::fpclassify(interpreted) == FP_SUBNORMAL) ||
        std::isnan(interpreted);
    if (unimplemented)
        goto finish;

    root_result = std::sqrt(interpreted);
    accurate = std::sqrtl((long double)interpreted);
    result = (uint64_t)std::bit_cast<uint64_t>(root_result);

    inexact = ((long double)root_result != accurate);

finish:
    cpu.fpu.set_cause(inexact, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)) 
        cpu.fpu.set_fpr(result, sa(op_code), 8);
}

void SQRTW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;
    
    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void SQRTL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;
        
    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}
// fpu compare
void CcondS(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr1_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint64_t fpr2_val = cpu.fpu.get_fpr(rt(op_code), 8);
    uint8_t cond = op_code & 0xF;
    bool condition = false;
    bool less = false;
    bool equal = false;
    bool unordered = false;

    cpu.fpu.clear_cause();

    bool unimplemented = false;
    bool invalid = false;

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

    cpu.fpu.set_cause(0, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)){
        cpu.fpu.FCR31 = (cpu.fpu.FCR31 & ~(1<<23)) | (condition << 23);
    }
}

void CcondD(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    uint64_t fpr1_val = cpu.fpu.get_fpr(rd(op_code), 8);
    uint64_t fpr2_val = cpu.fpu.get_fpr(rt(op_code), 8);
    uint8_t cond = op_code & 0xF;
    bool condition = false;
    bool less = false;
    bool equal = false;
    bool unordered = false;

    cpu.fpu.clear_cause();

    bool unimplemented = false;
    bool invalid = false;

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

    cpu.fpu.set_cause(0, 0, 0, 0, invalid, unimplemented);

    if (!cpu.check_fpu_exception(cpu.PC)){
        cpu.fpu.FCR31 = (cpu.fpu.FCR31 & ~(1<<23)) | (condition << 23);
    }
}

void CcondW(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

void CcondL(VR4300Interpreter &cpu, uint32_t op_code)
{
    cpu.inc_cycles(1);

    if(!cpu.cp_usable(cz(op_code)))
        return;

    cpu.fpu.clear_cause();
    cpu.fpu.set_cause(0, 0, 0, 0, 0, 1);
    cpu.check_fpu_exception(cpu.PC);
}

const Instruction i_primary_op_lut[64]{
    /*00*/ nullptr,
    /*01*/ nullptr,
    /*02*/ J, 
    /*03*/ JAL,
    /*04*/ BEQ,
    /*05*/ BNE,
    /*06*/ BLEZ, 
    /*07*/ BGTZ, 
    /*08*/ ADDI, 
    /*09*/ ADDIU,
    /*0A*/ SLTI, 
    /*0B*/ SLTIU,
    /*0C*/ ANDI,
    /*0D*/ ORI, 
    /*0E*/ XORI,
    /*0F*/ LUI, 
    /*10*/ COPz,
    /*11*/ COPz,
    /*12*/ COPz,
    /*13*/ nullptr,
    /*14*/ BEQL, 
    /*15*/ BNEL, 
    /*16*/ BLEZL,
    /*17*/ BGTZL,
    /*18*/ DADDI,
    /*19*/ DADDIU,
    /*1A*/ LDL,
    /*1B*/ LDR,
    /*1C*/ nullptr,
    /*1D*/ nullptr,
    /*1E*/ nullptr,
    /*1F*/ nullptr,
    /*20*/ LB, 
    /*21*/ LH, 
    /*22*/ LWL,
    /*23*/ LW, 
    /*24*/ LBU,
    /*25*/ LHU,
    /*26*/ LWR,
    /*27*/ LWU,
    /*28*/ SB,
    /*29*/ SH,
    /*2A*/ SWL,
    /*2B*/ SW, 
    /*2C*/ SDL,
    /*2D*/ SDR,
    /*2E*/ SWR,
    /*2F*/ CACHE,
    /*30*/ LL, 
    /*31*/ LWCz,
    /*32*/ LWCz,
    /*33*/ nullptr,
    /*34*/ LLD, 
    /*35*/ LDCz,
    /*36*/ LDCz,
    /*37*/ LD, 
    /*38*/ SC, 
    /*39*/ SWCz,
    /*3A*/ SWCz,
    /*3B*/ nullptr,
    /*3C*/ SCD, 
    /*3D*/ SDCz,
    /*3E*/ SDCz,
    /*3F*/ SD, 
};
const Instruction i_special_op_lut[64]{
    /*00*/ SLL,
    /*01*/ nullptr,
    /*02*/ SRL, 
    /*03*/ SRA, 
    /*04*/ SLLV,
    /*05*/ nullptr,
    /*06*/ SRLV,
    /*07*/ SRAV,
    /*08*/ JR, 
    /*09*/ JALR,
    /*0A*/ nullptr,
    /*0B*/ nullptr,
    /*0C*/ SYSCALL,
    /*0D*/ BREAK, 
    /*0E*/ nullptr,
    /*0F*/ SYNC, 
    /*10*/ MFHI, 
    /*11*/ MTHI, 
    /*12*/ MFLO, 
    /*13*/ MTLO, 
    /*14*/ DSLLV,
    /*15*/ nullptr,
    /*16*/ DSRLV, 
    /*17*/ DSRAV, 
    /*18*/ MULT,
    /*19*/ MULTU, 
    /*1A*/ DIV, 
    /*1B*/ DIVU, 
    /*1C*/ DMULT, 
    /*1D*/ DMULTU,
    /*1E*/ DDIV, 
    /*1F*/ DDIVU, 
    /*20*/ ADD, 
    /*21*/ ADDU,
    /*22*/ SUB, 
    /*23*/ SUBU,
    /*24*/ AND, 
    /*25*/ OR, 
    /*26*/ XOR,
    /*27*/ NOR, 
    /*28*/ nullptr,
    /*29*/ nullptr,
    /*2A*/ SLT, 
    /*2B*/ SLTU, 
    /*2C*/ DADD, 
    /*2D*/ DADDU,
    /*2E*/ DSUB, 
    /*2F*/ DSUBU,
    /*30*/ TGE, 
    /*31*/ TGEU,
    /*32*/ TLT, 
    /*33*/ TLTU,
    /*34*/ TEQ, 
    /*35*/ nullptr,
    /*36*/ TNE,
    /*37*/ nullptr,
    /*38*/ DSLL, 
    /*39*/ nullptr,
    /*3A*/ DSRL, 
    /*3B*/ DSRA, 
    /*3C*/ DSLL32,
    /*3D*/ nullptr,
    /*3E*/ DSRL32,
    /*3F*/ DSRA32,
};
const Instruction i_regimm_op_lut[32]{
    /*00*/ BLTZ, 
    /*01*/ BGEZ, 
    /*02*/ BLTZL,
    /*03*/ BGEZL,
    /*04*/ nullptr,
    /*05*/ nullptr,
    /*06*/ nullptr,
    /*07*/ nullptr,
    /*08*/ TGEI, 
    /*09*/ TGEIU,
    /*0A*/ TLTI, 
    /*0B*/ TLTIU,
    /*0C*/ TEQI, 
    /*0D*/ nullptr,
    /*0E*/ TNEI, 
    /*0F*/ nullptr,
    /*10*/ BLTZAL, 
    /*11*/ BGEZAL, 
    /*12*/ BLTZALL,
    /*13*/ BGEZALL,
};
const Instruction i_COP0rs_op_lut[32]{
    /*00*/ MFCz, 
    /*01*/ DMFCz,
    /*02*/ CFCz, 
    /*03*/ nullptr,
    /*04*/ MTCz, 
    /*05*/ DMTCz,
    /*06*/ CTCz, 
    /*07*/ nullptr,
    /*08*/ nullptr,
    /*09*/ nullptr,
    /*0A*/ nullptr,
    /*0B*/ nullptr,
    /*0C*/ nullptr,
    /*0D*/ nullptr,
    /*0E*/ nullptr,
    /*0F*/ nullptr,
};
const Instruction i_COP1rs_op_lut[32]{
    /*00*/ MFCz, 
    /*01*/ DMFCz,
    /*02*/ CFCz, 
    /*03*/ nullptr,
    /*04*/ MTCz, 
    /*05*/ DMTCz,
    /*06*/ CTCz, 
    /*07*/ nullptr,
    /*08*/ nullptr,
    /*09*/ nullptr,
    /*0A*/ nullptr,
    /*0B*/ nullptr,
    /*0C*/ nullptr,
    /*0D*/ nullptr,
    /*0E*/ nullptr,
    /*0F*/ nullptr,
};

const Instruction i_COP2rs_op_lut[32]{
    /*00*/ MFCz, 
    /*01*/ DMFCz,
    /*02*/ CFCz, 
    /*03*/ nullptr,
    /*04*/ MTCz, 
    /*05*/ DMTCz,
    /*06*/ CTCz, 
    /*07*/ nullptr,
    /*08*/ nullptr,
    /*09*/ nullptr,
    /*0A*/ nullptr,
    /*0B*/ nullptr,
    /*0C*/ nullptr,
    /*0D*/ nullptr,
    /*0E*/ nullptr,
    /*0F*/ nullptr,
};
const Instruction i_COP0rt_op_lut[32]{
    /*00*/ BCzF, 
    /*01*/ BCzT, 
    /*02*/ BCzFL,
    /*03*/ BCzTL,
    /*04*/ nullptr,
    /*05*/ nullptr,
    /*06*/ nullptr,
    /*07*/ nullptr,
    /*08*/ nullptr,
    /*09*/ nullptr,
    /*0A*/ nullptr,
    /*0B*/ nullptr,
    /*0C*/ nullptr,
    /*0D*/ nullptr,
    /*0E*/ nullptr,
    /*0F*/ nullptr,
    /*10*/ nullptr,
    /*11*/ nullptr,
    /*12*/ nullptr,
    /*13*/ nullptr,
    /*14*/ nullptr,
    /*15*/ nullptr,
    /*16*/ nullptr,
    /*17*/ nullptr,
    /*18*/ nullptr,
    /*19*/ nullptr,
    /*1A*/ nullptr,
    /*1B*/ nullptr,
    /*1C*/ nullptr,
    /*1D*/ nullptr,
    /*1E*/ nullptr,
    /*1F*/ nullptr,
};
const Instruction i_COP1rt_op_lut[32]{
    /*00*/ BCzF, 
    /*01*/ BCzT, 
    /*02*/ BCzFL,
    /*03*/ BCzTL,
    /*04*/ nullptr,
    /*05*/ nullptr,
    /*06*/ nullptr,
    /*07*/ nullptr,
    /*08*/ nullptr,
    /*09*/ nullptr,
    /*0A*/ nullptr,
    /*0B*/ nullptr,
    /*0C*/ nullptr,
    /*0D*/ nullptr,
    /*0E*/ nullptr,
    /*0F*/ nullptr,
    /*10*/ nullptr,
    /*11*/ nullptr,
    /*12*/ nullptr,
    /*13*/ nullptr,
    /*14*/ nullptr,
    /*15*/ nullptr,
    /*16*/ nullptr,
    /*17*/ nullptr,
    /*18*/ nullptr,
    /*19*/ nullptr,
    /*1A*/ nullptr,
    /*1B*/ nullptr,
    /*1C*/ nullptr,
    /*1D*/ nullptr,
    /*1E*/ nullptr,
    /*1F*/ nullptr,
};

const Instruction i_COP2rt_op_lut[32]{
    /*00*/ BCzF, 
    /*01*/ BCzT, 
    /*02*/ BCzFL,
    /*03*/ BCzTL,
    /*04*/ nullptr,
    /*05*/ nullptr,
    /*06*/ nullptr,
    /*07*/ nullptr,
    /*08*/ nullptr,
    /*09*/ nullptr,
    /*0A*/ nullptr,
    /*0B*/ nullptr,
    /*0C*/ nullptr,
    /*0D*/ nullptr,
    /*0E*/ nullptr,
    /*0F*/ nullptr,
    /*10*/ nullptr,
    /*11*/ nullptr,
    /*12*/ nullptr,
    /*13*/ nullptr,
    /*14*/ nullptr,
    /*15*/ nullptr,
    /*16*/ nullptr,
    /*17*/ nullptr,
    /*18*/ nullptr,
    /*19*/ nullptr,
    /*1A*/ nullptr,
    /*1B*/ nullptr,
    /*1C*/ nullptr,
    /*1D*/ nullptr,
    /*1E*/ nullptr,
    /*1F*/ nullptr,
};

const Instruction i_CP0_op_lut[32]{
    /*00*/ nullptr,
    /*01*/ TLBR, 
    /*02*/ TLBWI,
    /*03*/ nullptr,
    /*04*/ nullptr,
    /*05*/ nullptr,
    /*06*/ TLBWR,
    /*07*/ nullptr,
    /*08*/ TLBP,
    /*09*/ nullptr,
    /*0A*/ nullptr,
    /*0B*/ nullptr,
    /*0C*/ nullptr,
    /*0D*/ nullptr,
    /*0E*/ nullptr,
    /*0F*/ nullptr,
    /*10*/ nullptr,
    /*11*/ nullptr,
    /*12*/ nullptr,
    /*13*/ nullptr,
    /*14*/ nullptr,
    /*15*/ nullptr,
    /*16*/ nullptr,
    /*17*/ nullptr,
    /*18*/ ERET,
    /*19*/ nullptr,
    /*1A*/ nullptr,
    /*1B*/ nullptr,
    /*1C*/ nullptr,
    /*1D*/ nullptr,
    /*1E*/ nullptr,
    /*1F*/ nullptr,
};
const Instruction i_CP1_op_lut_S[64]{
    /*00*/ ADDS, 
    /*01*/ SUBS, 
    /*02*/ MULS, 
    /*03*/ DIVS, 
    /*04*/ SQRTS,
    /*05*/ ABSS, 
    /*06*/ MOVS, 
    /*07*/ NEGS, 
    /*08*/ ROUNDLS,
    /*09*/ TRUNCLS,
    /*0A*/ CEILLS, 
    /*0B*/ FLOORLS,
    /*0C*/ ROUNDWS,
    /*0D*/ TRUNCWS,
    /*0E*/ CEILWS, 
    /*0F*/ FLOORWS,
    /*10*/ nullptr,
    /*11*/ nullptr,
    /*12*/ nullptr,
    /*13*/ nullptr,
    /*14*/ nullptr,
    /*15*/ nullptr,
    /*16*/ nullptr,
    /*17*/ nullptr,
    /*18*/ nullptr,
    /*19*/ nullptr,
    /*1A*/ nullptr,
    /*1B*/ nullptr,
    /*1C*/ nullptr,
    /*1D*/ nullptr,
    /*1E*/ nullptr,
    /*1F*/ nullptr,
    /*20*/ CVTSS,
    /*21*/ CVTDS,
    /*22*/ nullptr,
    /*23*/ nullptr,
    /*24*/ CVTWS,
    /*25*/ CVTLS,
    /*26*/ nullptr,
    /*27*/ nullptr,
    /*28*/ nullptr,
    /*29*/ nullptr,
    /*2A*/ nullptr,
    /*2B*/ nullptr,
    /*2C*/ nullptr,
    /*2D*/ nullptr,
    /*2E*/ nullptr,
    /*2F*/ nullptr,
    /*30*/ CcondS,
    /*31*/ CcondS,
    /*32*/ CcondS,
    /*33*/ CcondS,
    /*34*/ CcondS,
    /*35*/ CcondS,
    /*36*/ CcondS,
    /*37*/ CcondS,
    /*38*/ CcondS,
    /*39*/ CcondS,
    /*3A*/ CcondS,
    /*3B*/ CcondS,
    /*3C*/ CcondS,
    /*3D*/ CcondS,
    /*3E*/ CcondS,
    /*3F*/ CcondS,
};

const Instruction i_CP1_op_lut_D[64]{
    /*00*/ ADDD, 
    /*01*/ SUBD, 
    /*02*/ MULD, 
    /*03*/ DIVD, 
    /*04*/ SQRTD,
    /*05*/ ABSD, 
    /*06*/ MOVD, 
    /*07*/ NEGD, 
    /*08*/ ROUNDLD,
    /*09*/ TRUNCLD,
    /*0A*/ CEILLD, 
    /*0B*/ FLOORLD,
    /*0C*/ ROUNDWD,
    /*0D*/ TRUNCWD,
    /*0E*/ CEILWD, 
    /*0F*/ FLOORWD,
    /*10*/ nullptr,
    /*11*/ nullptr,
    /*12*/ nullptr,
    /*13*/ nullptr,
    /*14*/ nullptr,
    /*15*/ nullptr,
    /*16*/ nullptr,
    /*17*/ nullptr,
    /*18*/ nullptr,
    /*19*/ nullptr,
    /*1A*/ nullptr,
    /*1B*/ nullptr,
    /*1C*/ nullptr,
    /*1D*/ nullptr,
    /*1E*/ nullptr,
    /*1F*/ nullptr,
    /*20*/ CVTSD,
    /*21*/ CVTDD,
    /*22*/ nullptr,
    /*23*/ nullptr,
    /*24*/ CVTWD,
    /*25*/ CVTLD,
    /*26*/ nullptr,
    /*27*/ nullptr,
    /*28*/ nullptr,
    /*29*/ nullptr,
    /*2A*/ nullptr,
    /*2B*/ nullptr,
    /*2C*/ nullptr,
    /*2D*/ nullptr,
    /*2E*/ nullptr,
    /*2F*/ nullptr,
    /*30*/ CcondD,
    /*31*/ CcondD,
    /*32*/ CcondD,
    /*33*/ CcondD,
    /*34*/ CcondD,
    /*35*/ CcondD,
    /*36*/ CcondD,
    /*37*/ CcondD,
    /*38*/ CcondD,
    /*39*/ CcondD,
    /*3A*/ CcondD,
    /*3B*/ CcondD,
    /*3C*/ CcondD,
    /*3D*/ CcondD,
    /*3E*/ CcondD,
    /*3F*/ CcondD,
};

const Instruction i_CP1_op_lut_W[64]{
    /*00*/ ADDW,
    /*01*/ SUBW,
    /*02*/ MULW,
    /*03*/ DIVW,
    /*04*/ SQRTW,
    /*05*/ ABSW,
    /*06*/ MOVW,
    /*07*/ NEGW,
    /*08*/ ROUNDLW,
    /*09*/ TRUNCLW,
    /*0A*/ CEILLW, 
    /*0B*/ FLOORLW,
    /*0C*/ ROUNDWW,
    /*0D*/ TRUNCWW,
    /*0E*/ CEILWW, 
    /*0F*/ FLOORWW,
    /*10*/ nullptr,
    /*11*/ nullptr,
    /*12*/ nullptr,
    /*13*/ nullptr,
    /*14*/ nullptr,
    /*15*/ nullptr,
    /*16*/ nullptr,
    /*17*/ nullptr,
    /*18*/ nullptr,
    /*19*/ nullptr,
    /*1A*/ nullptr,
    /*1B*/ nullptr,
    /*1C*/ nullptr,
    /*1D*/ nullptr,
    /*1E*/ nullptr,
    /*1F*/ nullptr,
    /*20*/ CVTSW,
    /*21*/ CVTDW,
    /*22*/ nullptr,
    /*23*/ nullptr,
    /*24*/ CVTWW,
    /*25*/ CVTLW,
    /*26*/ nullptr,
    /*27*/ nullptr,
    /*28*/ nullptr,
    /*29*/ nullptr,
    /*2A*/ nullptr,
    /*2B*/ nullptr,
    /*2C*/ nullptr,
    /*2D*/ nullptr,
    /*2E*/ nullptr,
    /*2F*/ nullptr,
    /*30*/ CcondW,
    /*31*/ CcondW,
    /*32*/ CcondW,
    /*33*/ CcondW,
    /*34*/ CcondW,
    /*35*/ CcondW,
    /*36*/ CcondW,
    /*37*/ CcondW,
    /*38*/ CcondW,
    /*39*/ CcondW,
    /*3A*/ CcondW,
    /*3B*/ CcondW,
    /*3C*/ CcondW,
    /*3D*/ CcondW,
    /*3E*/ CcondW,
    /*3F*/ CcondW,
};

const Instruction i_CP1_op_lut_L[64]{
    /*00*/ ADDL,
    /*01*/ SUBL,
    /*02*/ MULL,
    /*03*/ DIVL,
    /*04*/ SQRTL,
    /*05*/ ABSL,
    /*06*/ MOVL,
    /*07*/ NEGL,
    /*08*/ ROUNDLL,
    /*09*/ TRUNCLL,
    /*0A*/ CEILLL, 
    /*0B*/ FLOORLL,
    /*0C*/ ROUNDWL,
    /*0D*/ TRUNCWL,
    /*0E*/ CEILWL, 
    /*0F*/ FLOORWL,
    /*10*/ nullptr,
    /*11*/ nullptr,
    /*12*/ nullptr,
    /*13*/ nullptr,
    /*14*/ nullptr,
    /*15*/ nullptr,
    /*16*/ nullptr,
    /*17*/ nullptr,
    /*18*/ nullptr,
    /*19*/ nullptr,
    /*1A*/ nullptr,
    /*1B*/ nullptr,
    /*1C*/ nullptr,
    /*1D*/ nullptr,
    /*1E*/ nullptr,
    /*1F*/ nullptr,
    /*20*/ CVTSL,
    /*21*/ CVTDL,
    /*22*/ nullptr,
    /*23*/ nullptr,
    /*24*/ CVTWL,
    /*25*/ CVTLL,
    /*26*/ nullptr,
    /*27*/ nullptr,
    /*28*/ nullptr,
    /*29*/ nullptr,
    /*2A*/ nullptr,
    /*2B*/ nullptr,
    /*2C*/ nullptr,
    /*2D*/ nullptr,
    /*2E*/ nullptr,
    /*2F*/ nullptr,
    /*30*/ CcondL,
    /*31*/ CcondL,
    /*32*/ CcondL,
    /*33*/ CcondL,
    /*34*/ CcondL,
    /*35*/ CcondL,
    /*36*/ CcondL,
    /*37*/ CcondL,
    /*38*/ CcondL,
    /*39*/ CcondL,
    /*3A*/ CcondL,
    /*3B*/ CcondL,
    /*3C*/ CcondL,
    /*3D*/ CcondL,
    /*3E*/ CcondL,
    /*3F*/ CcondL,
};



const char* i_primary_op_name_lut[64]{
    /*00*/ " nullptr",
    /*01*/ " nullptr",
    /*02*/ " J", 
    /*03*/ " JAL",
    /*04*/ " BEQ",
    /*05*/ " BNE",
    /*06*/ " BLEZ", 
    /*07*/ " BGTZ", 
    /*08*/ " ADDI", 
    /*09*/ " ADDIU",
    /*0A*/ " SLTI", 
    /*0B*/ " SLTIU",
    /*0C*/ " ANDI",
    /*0D*/ " ORI", 
    /*0E*/ " XORI",
    /*0F*/ " LUI", 
    /*10*/ " COPz",
    /*11*/ " COPz",
    /*12*/ " COPz",
    /*13*/ " nullptr",
    /*14*/ " BEQL", 
    /*15*/ " BNEL", 
    /*16*/ " BLEZL",
    /*17*/ " BGTZL",
    /*18*/ " DADDI",
    /*19*/ " DADDIU",
    /*1A*/ " LDL",
    /*1B*/ " LDR",
    /*1C*/ " nullptr",
    /*1D*/ " nullptr",
    /*1E*/ " nullptr",
    /*1F*/ " nullptr",
    /*20*/ " LB", 
    /*21*/ " LH", 
    /*22*/ " LWL",
    /*23*/ " LW", 
    /*24*/ " LBU",
    /*25*/ " LHU",
    /*26*/ " LWR",
    /*27*/ " LWU",
    /*28*/ " SB",
    /*29*/ " SH",
    /*2A*/ " SWL",
    /*2B*/ " SW", 
    /*2C*/ " SDL",
    /*2D*/ " SDR",
    /*2E*/ " SWR",
    /*2F*/ " CACHE",
    /*30*/ " LL", 
    /*31*/ " LWCz",
    /*32*/ " LWCz",
    /*33*/ " nullptr",
    /*34*/ " LLD", 
    /*35*/ " LDCz",
    /*36*/ " LDCz",
    /*37*/ " LD", 
    /*38*/ " SC", 
    /*39*/ " SWCz",
    /*3A*/ " SWCz",
    /*3B*/ " nullptr",
    /*3C*/ " SCD", 
    /*3D*/ " SDCz",
    /*3E*/ " SDCz",
    /*3F*/ " SD", 
};
const char* i_special_op_name_lut[64]{
    /*00*/ " SLL",
    /*01*/ " nullptr",
    /*02*/ " SRL", 
    /*03*/ " SRA", 
    /*04*/ " SLLV",
    /*05*/ " nullptr",
    /*06*/ " SRLV",
    /*07*/ " SRAV",
    /*08*/ " JR", 
    /*09*/ " JALR",
    /*0A*/ " nullptr",
    /*0B*/ " nullptr",
    /*0C*/ " SYSCALL",
    /*0D*/ " BREAK", 
    /*0E*/ " nullptr",
    /*0F*/ " SYNC", 
    /*10*/ " MFHI", 
    /*11*/ " MTHI", 
    /*12*/ " MFLO", 
    /*13*/ " MTLO", 
    /*14*/ " DSLLV",
    /*15*/ " nullptr",
    /*16*/ " DSRLV", 
    /*17*/ " DSRAV", 
    /*18*/ " MULT",
    /*19*/ " MULTU", 
    /*1A*/ " DIV", 
    /*1B*/ " DIVU", 
    /*1C*/ " DMULT", 
    /*1D*/ " DMULTU",
    /*1E*/ " DDIV", 
    /*1F*/ " DDIVU", 
    /*20*/ " ADD", 
    /*21*/ " ADDU",
    /*22*/ " SUB", 
    /*23*/ " SUBU",
    /*24*/ " AND", 
    /*25*/ " OR", 
    /*26*/ " XOR",
    /*27*/ " NOR", 
    /*28*/ " nullptr",
    /*29*/ " nullptr",
    /*2A*/ " SLT", 
    /*2B*/ " SLTU", 
    /*2C*/ " DADD", 
    /*2D*/ " DADDU",
    /*2E*/ " DSUB", 
    /*2F*/ " DSUBU",
    /*30*/ " TGE", 
    /*31*/ " TGEU",
    /*32*/ " TLT", 
    /*33*/ " TLTU",
    /*34*/ " TEQ", 
    /*35*/ " nullptr",
    /*36*/ " TNE",
    /*37*/ " nullptr",
    /*38*/ " DSLL", 
    /*39*/ " nullptr",
    /*3A*/ " DSRL", 
    /*3B*/ " DSRA", 
    /*3C*/ " DSLL32",
    /*3D*/ " nullptr",
    /*3E*/ " DSRL32",
    /*3F*/ " DSRA32",
};
const char* i_regimm_op_name_lut[32]{
    /*00*/ " BLTZ", 
    /*01*/ " BGEZ", 
    /*02*/ " BLTZL",
    /*03*/ " BGEZL",
    /*04*/ " nullptr",
    /*05*/ " nullptr",
    /*06*/ " nullptr",
    /*07*/ " nullptr",
    /*08*/ " TGEI", 
    /*09*/ " TGEIU",
    /*0A*/ " TLTI", 
    /*0B*/ " TLTIU",
    /*0C*/ " TEQI", 
    /*0D*/ " nullptr",
    /*0E*/ " TNEI", 
    /*0F*/ " nullptr",
    /*10*/ " BLTZAL", 
    /*11*/ " BGEZAL", 
    /*12*/ " BLTZALL",
    /*13*/ " BGEZALL",
};
const char* i_COP0rs_op_name_lut[32]{
    /*00*/ " MFCz", 
    /*01*/ " DMFCz",
    /*02*/ " CFCz", 
    /*03*/ " nullptr",
    /*04*/ " MTCz", 
    /*05*/ " DMTCz",
    /*06*/ " CTCz", 
    /*07*/ " nullptr",
    /*08*/ " nullptr",
    /*09*/ " nullptr",
    /*0A*/ " nullptr",
    /*0B*/ " nullptr",
    /*0C*/ " nullptr",
    /*0D*/ " nullptr",
    /*0E*/ " nullptr",
    /*0F*/ " nullptr",
};
const char* i_COP1rs_op_name_lut[32]{
    /*00*/ " MFCz", 
    /*01*/ " DMFCz",
    /*02*/ " CFCz", 
    /*03*/ " nullptr",
    /*04*/ " MTCz", 
    /*05*/ " DMTCz",
    /*06*/ " CTCz", 
    /*07*/ " nullptr",
    /*08*/ " nullptr",
    /*09*/ " nullptr",
    /*0A*/ " nullptr",
    /*0B*/ " nullptr",
    /*0C*/ " nullptr",
    /*0D*/ " nullptr",
    /*0E*/ " nullptr",
    /*0F*/ " nullptr",
};

const char* i_COP2rs_op_name_lut[32]{
    /*00*/ " MFCz", 
    /*01*/ " DMFCz",
    /*02*/ " CFCz", 
    /*03*/ " nullptr",
    /*04*/ " MTCz", 
    /*05*/ " DMTCz",
    /*06*/ " CTCz", 
    /*07*/ " nullptr",
    /*08*/ " nullptr",
    /*09*/ " nullptr",
    /*0A*/ " nullptr",
    /*0B*/ " nullptr",
    /*0C*/ " nullptr",
    /*0D*/ " nullptr",
    /*0E*/ " nullptr",
    /*0F*/ " nullptr",
};
const char* i_COP0rt_op_name_lut[32]{
    /*00*/ " BCzF", 
    /*01*/ " BCzT", 
    /*02*/ " BCzFL",
    /*03*/ " BCzTL",
    /*04*/ " nullptr",
    /*05*/ " nullptr",
    /*06*/ " nullptr",
    /*07*/ " nullptr",
    /*08*/ " nullptr",
    /*09*/ " nullptr",
    /*0A*/ " nullptr",
    /*0B*/ " nullptr",
    /*0C*/ " nullptr",
    /*0D*/ " nullptr",
    /*0E*/ " nullptr",
    /*0F*/ " nullptr",
    /*10*/ " nullptr",
    /*11*/ " nullptr",
    /*12*/ " nullptr",
    /*13*/ " nullptr",
    /*14*/ " nullptr",
    /*15*/ " nullptr",
    /*16*/ " nullptr",
    /*17*/ " nullptr",
    /*18*/ " nullptr",
    /*19*/ " nullptr",
    /*1A*/ " nullptr",
    /*1B*/ " nullptr",
    /*1C*/ " nullptr",
    /*1D*/ " nullptr",
    /*1E*/ " nullptr",
    /*1F*/ " nullptr",
};
const char* i_COP1rt_op_name_lut[32]{
    /*00*/ " BCzF", 
    /*01*/ " BCzT", 
    /*02*/ " BCzFL",
    /*03*/ " BCzTL",
    /*04*/ " nullptr",
    /*05*/ " nullptr",
    /*06*/ " nullptr",
    /*07*/ " nullptr",
    /*08*/ " nullptr",
    /*09*/ " nullptr",
    /*0A*/ " nullptr",
    /*0B*/ " nullptr",
    /*0C*/ " nullptr",
    /*0D*/ " nullptr",
    /*0E*/ " nullptr",
    /*0F*/ " nullptr",
    /*10*/ " nullptr",
    /*11*/ " nullptr",
    /*12*/ " nullptr",
    /*13*/ " nullptr",
    /*14*/ " nullptr",
    /*15*/ " nullptr",
    /*16*/ " nullptr",
    /*17*/ " nullptr",
    /*18*/ " nullptr",
    /*19*/ " nullptr",
    /*1A*/ " nullptr",
    /*1B*/ " nullptr",
    /*1C*/ " nullptr",
    /*1D*/ " nullptr",
    /*1E*/ " nullptr",
    /*1F*/ " nullptr",
};

const char* i_COP2rt_op_name_lut[32]{
    /*00*/ " BCzF", 
    /*01*/ " BCzT", 
    /*02*/ " BCzFL",
    /*03*/ " BCzTL",
    /*04*/ " nullptr",
    /*05*/ " nullptr",
    /*06*/ " nullptr",
    /*07*/ " nullptr",
    /*08*/ " nullptr",
    /*09*/ " nullptr",
    /*0A*/ " nullptr",
    /*0B*/ " nullptr",
    /*0C*/ " nullptr",
    /*0D*/ " nullptr",
    /*0E*/ " nullptr",
    /*0F*/ " nullptr",
    /*10*/ " nullptr",
    /*11*/ " nullptr",
    /*12*/ " nullptr",
    /*13*/ " nullptr",
    /*14*/ " nullptr",
    /*15*/ " nullptr",
    /*16*/ " nullptr",
    /*17*/ " nullptr",
    /*18*/ " nullptr",
    /*19*/ " nullptr",
    /*1A*/ " nullptr",
    /*1B*/ " nullptr",
    /*1C*/ " nullptr",
    /*1D*/ " nullptr",
    /*1E*/ " nullptr",
    /*1F*/ " nullptr",
};

const char* i_CP0_op_name_lut[32]{
    /*00*/ " nullptr",
    /*01*/ " TLBR", 
    /*02*/ " TLBWI",
    /*03*/ " nullptr",
    /*04*/ " nullptr",
    /*05*/ " nullptr",
    /*06*/ " TLBWR",
    /*07*/ " nullptr",
    /*08*/ " TLBP",
    /*09*/ " nullptr",
    /*0A*/ " nullptr",
    /*0B*/ " nullptr",
    /*0C*/ " nullptr",
    /*0D*/ " nullptr",
    /*0E*/ " nullptr",
    /*0F*/ " nullptr",
    /*10*/ " nullptr",
    /*11*/ " nullptr",
    /*12*/ " nullptr",
    /*13*/ " nullptr",
    /*14*/ " nullptr",
    /*15*/ " nullptr",
    /*16*/ " nullptr",
    /*17*/ " nullptr",
    /*18*/ " ERET",
    /*19*/ " nullptr",
    /*1A*/ " nullptr",
    /*1B*/ " nullptr",
    /*1C*/ " nullptr",
    /*1D*/ " nullptr",
    /*1E*/ " nullptr",
    /*1F*/ " nullptr",
};
const char* i_CP1_op_name_lut[64]{
    /*00*/ " ADDfmt", 
    /*01*/ " SUBfmt", 
    /*02*/ " MULfmt", 
    /*03*/ " DIVfmt", 
    /*04*/ " SQRTfmt",
    /*05*/ " ABSfmt", 
    /*06*/ " MOVfmt", 
    /*07*/ " NEGfmt", 
    /*08*/ " ROUNDLfmt",
    /*09*/ " TRUNCLfmt",
    /*0A*/ " CEILLfmt", 
    /*0B*/ " FLOORLfmt",
    /*0C*/ " ROUNDWfmt",
    /*0D*/ " TRUNCWfmt",
    /*0E*/ " CEILWfmt", 
    /*0F*/ " FLOORWfmt",
    /*10*/ " nullptr",
    /*11*/ " nullptr",
    /*12*/ " nullptr",
    /*13*/ " nullptr",
    /*14*/ " nullptr",
    /*15*/ " nullptr",
    /*16*/ " nullptr",
    /*17*/ " nullptr",
    /*18*/ " nullptr",
    /*19*/ " nullptr",
    /*1A*/ " nullptr",
    /*1B*/ " nullptr",
    /*1C*/ " nullptr",
    /*1D*/ " nullptr",
    /*1E*/ " nullptr",
    /*1F*/ " nullptr",
    /*20*/ " CVTSfmt",
    /*21*/ " CVTDfmt",
    /*22*/ " nullptr",
    /*23*/ " nullptr",
    /*24*/ " CVTWfmt",
    /*25*/ " CVTLfmt",
    /*26*/ " nullptr",
    /*27*/ " nullptr",
    /*28*/ " nullptr",
    /*29*/ " nullptr",
    /*2A*/ " nullptr",
    /*2B*/ " nullptr",
    /*2C*/ " nullptr",
    /*2D*/ " nullptr",
    /*2E*/ " nullptr",
    /*2F*/ " nullptr",
    /*30*/ " Ccondfmt",
    /*31*/ " Ccondfmt",
    /*32*/ " Ccondfmt",
    /*33*/ " Ccondfmt",
    /*34*/ " Ccondfmt",
    /*35*/ " Ccondfmt",
    /*36*/ " Ccondfmt",
    /*37*/ " Ccondfmt",
    /*38*/ " Ccondfmt",
    /*39*/ " Ccondfmt",
    /*3A*/ " Ccondfmt",
    /*3B*/ " Ccondfmt",
    /*3C*/ " Ccondfmt",
    /*3D*/ " Ccondfmt",
    /*3E*/ " Ccondfmt",
    /*3F*/ " Ccondfmt",
};
