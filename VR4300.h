#include <cstdint>
#include "CP0.h"
#include "FPU.h"
#include "Bus.h"

#pragma once

#define CACHE_OP_STALL_TIME 2
#define DCACHE_STALL_TIME 2
#define ICACHE_STALL_TIME 2

enum OpTypes:uint32_t{
    NOP,
    ICACHE,
    DCACHE
};

enum OpFlags:uint32_t{
    ACCESSES_DOUBLE_WORD = 1<<0,
    ACCESSES_HALF_WORD = 1<<1,
    ACCESSES_WORD = 1<<2,
    WRITES_CACHE = 1<<3,
    WRITES_REG = 1<<4,
    CAUSED_EXCEPTION = 1<<5,
    READS_CP0 = 1<<6,
    READS_CACHE = 1<<7,
    IS_STORE = 1<<8,
    IS_LOAD = 1<<9,
    STORES_IN_RT = 1 << 10,
    STORES_IN_RD = 1 << 11,
    IS_IN_BRANCH_DELAY = 1<<12,
    CAUSES_BRANCH_DELAY = 1<<13
};

class VR4300
{
public:
    VR4300(Bus& bus);
    ~VR4300();

    FPU fpu;
    CP0 cp0;
    Bus& bus;

    uint64_t PC = 0;
    uint64_t HI = 0;
    uint64_t LO = 0;
    uint8_t LLBit = 0;

    //general purpose registers
    //r0 is always 0
    union {
        struct {
            uint64_t r0;
            uint64_t r1;
            uint64_t r2;
            uint64_t r3;
            uint64_t r4;
            uint64_t r5;
            uint64_t r6;
            uint64_t r7;
            uint64_t r8;
            uint64_t r9;
            uint64_t r10;
            uint64_t r11;
            uint64_t r12;
            uint64_t r13;
            uint64_t r14;
            uint64_t r15;
            uint64_t r16;
            uint64_t r17;
            uint64_t r18;
            uint64_t r19;
            uint64_t r20;
            uint64_t r21;
            uint64_t r22;
            uint64_t r23;
            uint64_t r24;
            uint64_t r25;
            uint64_t r26;
            uint64_t r27;
            uint64_t r28;
            uint64_t r29;
            uint64_t r30;
            uint64_t r31;
        };
        uint64_t GPR[32];
    };

    struct Dcache_line{
        bool valid = false;
        bool dirty = false;
        uint32_t tag;
        uint32_t data[4];
    };

    struct Icache_line{
        bool valid = false;
        uint32_t tag;
        uint32_t data[8];
    };

    Dcache_line Dcache[512]; 
    Icache_line Icache[512];
    
    struct OperationTemplate{
        void (*execute)(VR4300& cpu) = nullptr;
        uint32_t flags = 0;
        uint8_t multicycle = 0;
        uint16_t instruction_type = 0;
    };
    
    struct Operation : OperationTemplate{
        Operation();
        uint8_t rs = 0;
        uint8_t rt = 0;
        uint8_t rd = 0;
        uint8_t write_destination = 0;
        uint16_t immediate = 0;
        uint32_t write_offset = 0;
        uint64_t rs_val = 0;
        uint64_t rt_val = 0;
        uint64_t result = 0;
        uint64_t PC = 0;
        uint64_t dcache_index = 0;
    };

    const OperationTemplate primary_op_lut[64];
    const OperationTemplate special_op_lut[64];
    const OperationTemplate regimm_op_lut[64];

    
    void decode_op(uint32_t word);

    void abort_pipeline();

    void handle_tlb_miss_exception(uint64_t addr, const Operation &op, ExceptionCode cause);

    void handle_general_exception(const Operation &op, ExceptionCode cause);

    struct WB_DC{
        Operation op;
        bool CP0I_triggered;
    };

    struct EX_DC{
        Operation op;
        bool DCB_triggered;
        uint64_t data_addr;
    };
    struct RF_EX{
        Operation op;
        bool MCI_triggered;
        bool LDI_triggered;
    };
    struct IC_RF{
        bool next_op_bd;
        Operation op;
        bool ICB_triggered;
        uint32_t icache_index;
    };

    uint16_t stall;
    void on_clock();

    //Pipeline stages.
    bool WB(); // write back
    WB_DC WB_in{};
    WB_DC DC_out{};
    bool DC(); // data cache fetch
    EX_DC DC_in{};
    EX_DC EX_out{};
    bool EX(); // execute
    RF_EX EX_in{};
    RF_EX RF_out{};
    bool RF(); // register fetch
    IC_RF RF_in{};
    IC_RF IC_out{};
    bool IC(); // instruction cache fetch

    void submit_pipeline();

    private:
    
};


    // Load/store instructions
    void LB(VR4300& cpu);
    void LBU(VR4300& cpu);
    void LH(VR4300& cpu);
    void LHU(VR4300& cpu);
    void LW(VR4300& cpu);
    void LWL(VR4300& cpu); 
    void LWR(VR4300& cpu);
    void SB(VR4300& cpu);
    void SH(VR4300& cpu);
    void SW(VR4300& cpu);
    void SWL(VR4300& cpu);
    void SWR(VR4300& cpu);
    void LD(VR4300& cpu);
    void LDL(VR4300& cpu);
    void LDR(VR4300& cpu);
    void LL(VR4300& cpu);
    void LLD(VR4300& cpu);
    void LWU(VR4300& cpu);
    void SC(VR4300& cpu);
    void SCD(VR4300& cpu);
    void SD(VR4300& cpu);
    void SDL(VR4300& cpu);
    void SDR(VR4300& cpu);

    //Computational instructions
    void ADDI(VR4300& cpu);
    void ADDIU(VR4300& cpu);
    void SLTI(VR4300& cpu);
    void SLTIU(VR4300& cpu);
    void ANDI(VR4300& cpu);
    void ORI(VR4300& cpu);
    void XORI(VR4300& cpu);
    void LUI(VR4300& cpu);
    void DADDI(VR4300& cpu);
    void DADDIU(VR4300& cpu);
    void ADD(VR4300& cpu);
    void ADDU(VR4300& cpu);
    void SUB(VR4300& cpu);
    void SUBU(VR4300& cpu);
    void SLT(VR4300& cpu);
    void SLTU(VR4300& cpu);
    void AND(VR4300& cpu);
    void OR(VR4300& cpu);
    void XOR(VR4300& cpu);
    void NOR(VR4300& cpu);
    void DADD(VR4300& cpu);
    void DADDU(VR4300& cpu);
    void DSUB(VR4300& cpu);
    void DSUBU(VR4300& cpu);

    //Shift instrucitons
    void SLL(VR4300& cpu);
    void SRL(VR4300& cpu);
    void SRA(VR4300& cpu);
    void SLLV(VR4300& cpu);
    void SRLV(VR4300& cpu);
    void SRAV(VR4300& cpu);
    void DSLL(VR4300& cpu);
    void DSRL(VR4300& cpu);
    void DSRA(VR4300& cpu);
    void DSLLV(VR4300& cpu);
    void DSRLV(VR4300& cpu);
    void DSRAV(VR4300& cpu);
    void DSLL32(VR4300& cpu);
    void DSRL32(VR4300& cpu);
    void DSRA32(VR4300& cpu);

    //Multiply/Divide instructions
    void MULT(VR4300& cpu);
    void MULTU(VR4300& cpu);
    void DIV(VR4300& cpu);
    void DIVU(VR4300& cpu);
    void MFHI(VR4300& cpu);
    void MFLO(VR4300& cpu);
    void MTHI(VR4300& cpu);
    void MTLO(VR4300& cpu);
    void DMULT(VR4300& cpu);
    void DMULTU(VR4300& cpu);
    void DDIV(VR4300& cpu);
    void DDIVU(VR4300& cpu);

    //Jump/branch instructions
    void J(VR4300& cpu);
    void JAL(VR4300& cpu);
    void JR(VR4300& cpu);
    void JALR(VR4300& cpu);
    void BEQ(VR4300& cpu);
    void BNE(VR4300& cpu);
    void BLEZ(VR4300& cpu);
    void BGTZ(VR4300& cpu);
    void BLTZ(VR4300& cpu);
    void BGEZ(VR4300& cpu);
    void BLTZAL(VR4300& cpu);
    void BGEZAL(VR4300& cpu);
    void BEQL(VR4300& cpu);
    void BNEL(VR4300& cpu);
    void BLEZL(VR4300& cpu);
    void BGTZL(VR4300& cpu);
    void BLTZL(VR4300& cpu);
    void BGEZL(VR4300& cpu);
    void BLTZALL(VR4300& cpu);
    void BGEZALL(VR4300& cpu);

    //Special instructions
    void SYNC(VR4300& cpu);
    void SYSCALL(VR4300& cpu);
    void BREAK(VR4300& cpu);
    void TGE(VR4300& cpu);
    void TGEU(VR4300& cpu);
    void TLT(VR4300& cpu);
    void TLTU(VR4300& cpu);
    void TEQ(VR4300& cpu);
    void TNE(VR4300& cpu);
    void TGEI(VR4300& cpu);
    void TGEIU(VR4300& cpu);
    void TLTI(VR4300& cpu);
    void TLTIU(VR4300& cpu);
    void TEQI(VR4300& cpu);
    void TNEI(VR4300& cpu);

    //Coprocessor instructions
    void LWCz(VR4300& cpu);
    void SWCz(VR4300& cpu);
    void MTCz(VR4300& cpu);
    void MFCz(VR4300& cpu);
    void CTCz(VR4300& cpu);
    void CFCz(VR4300& cpu);
    void COPz(VR4300& cpu);
    void BCzT(VR4300& cpu);
    void BCzF(VR4300& cpu);
    void DMTCz(VR4300& cpu);
    void DMFCz(VR4300& cpu);
    void LDCz(VR4300& cpu);
    void SDCz(VR4300& cpu);
    void BCzTL(VR4300& cpu);
    void BCzFL(VR4300& cpu);

    //CP0 Instructions
    void MTC0(VR4300& cpu);
    void MFC0(VR4300& cpu);
    void DMTC0(VR4300& cpu);
    void DMFC0(VR4300& cpu);
    void TLBR(VR4300& cpu);
    void TLBWI(VR4300& cpu);
    void TLBWR(VR4300& cpu);
    void TLBP(VR4300& cpu);
    void ERET(VR4300& cpu);
    void CACHE(VR4300& cpu);