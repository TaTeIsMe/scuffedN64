
#pragma once
#include"VR4300.h"

void NOP(VR4300& cpu);

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

//fpu format
void CVTSfmt(VR4300& cpu);
void CVTDfmt(VR4300& cpu);
void CVTLfmt(VR4300& cpu);
void CVTWfmt(VR4300& cpu);
void ROUNDLfmt(VR4300& cpu);
void ROUNDWfmt(VR4300& cpu);
void TRUNCLfmt(VR4300& cpu);
void TRUNCWfmt(VR4300& cpu);
void CEILLfmt(VR4300& cpu);
void CEILWfmt(VR4300& cpu);
void FLOORLfmt(VR4300& cpu);
void FLOORWfmt(VR4300& cpu);

//fpu computational
void ADDfmt(VR4300& cpu);
void SUBfmt(VR4300& cpu);
void MULfmt(VR4300& cpu);
void DIVfmt(VR4300& cpu);
void ABSfmt(VR4300& cpu);
void MOVfmt(VR4300& cpu);
void NEGfmt(VR4300& cpu);
void SQRTfmt(VR4300& cpu);

//fpu compare
void Ccondfmt(VR4300& cpu);

extern const VR4300::OperationTemplate primary_op_lut[64];
extern const VR4300::OperationTemplate special_op_lut[64];
extern const VR4300::OperationTemplate regimm_op_lut[32];
extern const VR4300::OperationTemplate COPzrs_op_lut[32];
extern const VR4300::OperationTemplate COPzrt_op_lut[32];
extern const VR4300::OperationTemplate CP0_op_lut[32];
extern const VR4300::OperationTemplate CP1_op_lut[64];