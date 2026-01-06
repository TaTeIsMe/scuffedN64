#include <cstdint>
#include "CP0.h"
#include "FPU.h"

#pragma once

class VR4300
{
public:
    VR4300();
    ~VR4300();

    FPU fpu;
    CP0 cp0;

    uint64_t PC = 0x00000000;
    uint64_t HI = 0x00000000;
    uint64_t LO = 0x00000000;
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

    void on_clock();

    //Pipeline stages. They return the amount of cycles to be stalled
    uint8_t WB(); // write back
    uint8_t DC(); // data cache fetch
    uint8_t EX(); // execute
    uint8_t RF(); // register fetch
    uint8_t IC(); // instruction cache fetch

    // Load/store instructions
    void LB(uint32_t word);
    void LBU(uint32_t word);
    void LH(uint32_t word);
    void LHU(uint32_t word);
    void LW(uint32_t word);
    void LWL(uint32_t word);
    void LWR(uint32_t word);
    void SB(uint32_t word);
    void SH(uint32_t word);
    void SW(uint32_t word);
    void SWL(uint32_t word);
    void SWR(uint32_t word);
    void LD(uint32_t word);
    void LDL(uint32_t word);
    void LDR(uint32_t word);
    void LL(uint32_t word);
    void LLD(uint32_t word);
    void LWU(uint32_t word);
    void SC(uint32_t word);
    void SCD(uint32_t word);
    void SD(uint32_t word);
    void SDL(uint32_t word);
    void SDR(uint32_t word);

    //Computational instructions
    void ADDI(uint32_t word);
    void ADDIU(uint32_t word);
    void SLTI(uint32_t word);
    void SLTIU(uint32_t word);
    void ANDI(uint32_t word);
    void ORI(uint32_t word);
    void XORI(uint32_t word);
    void LUI(uint32_t word);
    void DADDI(uint32_t word);
    void DADDIU(uint32_t word);
    void ADD(uint32_t word);
    void ADDU(uint32_t word);
    void SUB(uint32_t word);
    void SUBU(uint32_t word);
    void SLT(uint32_t word);
    void SLTU(uint32_t word);
    void AND(uint32_t word);
    void OR(uint32_t word);
    void XOR(uint32_t word);
    void NOR(uint32_t word);
    void DADD(uint32_t word);
    void DADDU(uint32_t word);
    void DSUB(uint32_t word);
    void DSUBU(uint32_t word);

    //Shift instrucitons
    void SLL(uint32_t word);
    void SRL(uint32_t word);
    void SRA(uint32_t word);
    void SLLV(uint32_t word);
    void SRLV(uint32_t word);
    void SRAV(uint32_t word);
    void DSLL(uint32_t word);
    void DSRL(uint32_t word);
    void DSRA(uint32_t word);
    void DSLLV(uint32_t word);
    void DSRLV(uint32_t word);
    void DSRAV(uint32_t word);
    void DSLL32(uint32_t word);
    void DSRL32(uint32_t word);
    void DSRA32(uint32_t word);

    //Multiply/Divide instructions
    void MULT(uint32_t word);
    void MULTU(uint32_t word);
    void DIV(uint32_t word);
    void DIVU(uint32_t word);
    void MFHI(uint32_t word);
    void MFLO(uint32_t word);
    void MTHI(uint32_t word);
    void MTLO(uint32_t word);
    void DMULT(uint32_t word);
    void DMULTU(uint32_t word);
    void DDIV(uint32_t word);
    void DDIVU(uint32_t word);

    //Jump/branch instructions
    void J(uint32_t word);
    void JAL(uint32_t word);
    void JR(uint32_t word);
    void JALR(uint32_t word);
    void BEQ(uint32_t word);
    void BNE(uint32_t word);
    void BLEZ(uint32_t word);
    void BGTZ(uint32_t word);
    void BLTZ(uint32_t word);
    void BGEZ(uint32_t word);
    void BLTZAL(uint32_t word);
    void BGEZAL(uint32_t word);
    void BEQL(uint32_t word);
    void BNEL(uint32_t word);
    void BLEZL(uint32_t word);
    void BGTZL(uint32_t word);
    void BLTZL(uint32_t word);
    void BGEZL(uint32_t word);
    void BLTZALL(uint32_t word);
    void BGEZALL(uint32_t word);

    //Special instructions
    void SYNC(uint32_t word);
    void SYSCALL(uint32_t word);
    void BREAK(uint32_t word);
    void TGE(uint32_t word);
    void TGEU(uint32_t word);
    void TLT(uint32_t word);
    void TLTU(uint32_t word);
    void TEQ(uint32_t word);
    void TNE(uint32_t word);
    void TGEI(uint32_t word);
    void TGEIU(uint32_t word);
    void TLTI(uint32_t word);
    void TLTIU(uint32_t word);
    void TEQI(uint32_t word);
    void TNEI(uint32_t word);

    //Coprocessor instructions
    void LWCz(uint32_t word);
    void SWCz(uint32_t word);
    void MTCz(uint32_t word);
    void MFCz(uint32_t word);
    void CTCz(uint32_t word);
    void CFCz(uint32_t word);
    void COPz(uint32_t word);
    void BCzT(uint32_t word);
    void BCzF(uint32_t word);
    void DMTCz(uint32_t word);
    void DMFCz(uint32_t word);
    void LDCz(uint32_t word);
    void SDCz(uint32_t word);
    void BCzTL(uint32_t word);
    void BCzFL(uint32_t word);

    //CP0 Instructions
    void MTC0(uint32_t word);
    void MFC0(uint32_t word);
    void DMTC0(uint32_t word);
    void DMFC0(uint32_t word);
    void TLBR(uint32_t word);
    void TLBWI(uint32_t word);
    void TLBWR(uint32_t word);
    void TLBP(uint32_t word);
    void ERET(uint32_t word);
    void CACHE(uint32_t word);
private:

};
