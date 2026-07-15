
#pragma once
#include"VR4300Cycle.h"

void NOP(VR4300Cycle& cpu);

// Load/store instructions
void LB(VR4300Cycle& cpu);
void LBU(VR4300Cycle& cpu);
void LH(VR4300Cycle& cpu);
void LHU(VR4300Cycle& cpu);
void LW(VR4300Cycle& cpu);
void LWL(VR4300Cycle& cpu); 
void LWR(VR4300Cycle& cpu);
void SB(VR4300Cycle& cpu);
void SH(VR4300Cycle& cpu);
void SW(VR4300Cycle& cpu);
void SWL(VR4300Cycle& cpu);
void SWR(VR4300Cycle& cpu);
void LD(VR4300Cycle& cpu);
void LDL(VR4300Cycle& cpu);
void LDR(VR4300Cycle& cpu);
void LL(VR4300Cycle& cpu);
void LLD(VR4300Cycle& cpu);
void LWU(VR4300Cycle& cpu);
void SC(VR4300Cycle& cpu);
void SCD(VR4300Cycle& cpu);
void SD(VR4300Cycle& cpu);
void SDL(VR4300Cycle& cpu);
void SDR(VR4300Cycle& cpu);

//Computational instructions
void ADDI(VR4300Cycle& cpu);
void ADDIU(VR4300Cycle& cpu);
void SLTI(VR4300Cycle& cpu);
void SLTIU(VR4300Cycle& cpu);
void ANDI(VR4300Cycle& cpu);
void ORI(VR4300Cycle& cpu);
void XORI(VR4300Cycle& cpu);
void LUI(VR4300Cycle& cpu);
void DADDI(VR4300Cycle& cpu);
void DADDIU(VR4300Cycle& cpu);
void ADD(VR4300Cycle& cpu);
void ADDU(VR4300Cycle& cpu);
void SUB(VR4300Cycle& cpu);
void SUBU(VR4300Cycle& cpu);
void SLT(VR4300Cycle& cpu);
void SLTU(VR4300Cycle& cpu);
void AND(VR4300Cycle& cpu);
void OR(VR4300Cycle& cpu);
void XOR(VR4300Cycle& cpu);
void NOR(VR4300Cycle& cpu);
void DADD(VR4300Cycle& cpu);
void DADDU(VR4300Cycle& cpu);
void DSUB(VR4300Cycle& cpu);
void DSUBU(VR4300Cycle& cpu);

//Shift instrucitons
void SLL(VR4300Cycle& cpu);
void SRL(VR4300Cycle& cpu);
void SRA(VR4300Cycle& cpu);
void SLLV(VR4300Cycle& cpu);
void SRLV(VR4300Cycle& cpu);
void SRAV(VR4300Cycle& cpu);
void DSLL(VR4300Cycle& cpu);
void DSRL(VR4300Cycle& cpu);
void DSRA(VR4300Cycle& cpu);
void DSLLV(VR4300Cycle& cpu);
void DSRLV(VR4300Cycle& cpu);
void DSRAV(VR4300Cycle& cpu);
void DSLL32(VR4300Cycle& cpu);
void DSRL32(VR4300Cycle& cpu);
void DSRA32(VR4300Cycle& cpu);

//Multiply/Divide instructions
void MULT(VR4300Cycle& cpu);
void MULTU(VR4300Cycle& cpu);
void DIV(VR4300Cycle& cpu);
void DIVU(VR4300Cycle& cpu);
void MFHI(VR4300Cycle& cpu);
void MFLO(VR4300Cycle& cpu);
void MTHI(VR4300Cycle& cpu);
void MTLO(VR4300Cycle& cpu);
void DMULT(VR4300Cycle& cpu);
void DMULTU(VR4300Cycle& cpu);
void DDIV(VR4300Cycle& cpu);
void DDIVU(VR4300Cycle& cpu);

//Jump/branch instructions
void J(VR4300Cycle& cpu);
void JAL(VR4300Cycle& cpu);
void JR(VR4300Cycle& cpu);
void JALR(VR4300Cycle& cpu);
void BEQ(VR4300Cycle& cpu);
void BNE(VR4300Cycle& cpu);
void BLEZ(VR4300Cycle& cpu);
void BGTZ(VR4300Cycle& cpu);
void BLTZ(VR4300Cycle& cpu);
void BGEZ(VR4300Cycle& cpu);
void BLTZAL(VR4300Cycle& cpu);
void BGEZAL(VR4300Cycle& cpu);
void BEQL(VR4300Cycle& cpu);
void BNEL(VR4300Cycle& cpu);
void BLEZL(VR4300Cycle& cpu);
void BGTZL(VR4300Cycle& cpu);
void BLTZL(VR4300Cycle& cpu);
void BGEZL(VR4300Cycle& cpu);
void BLTZALL(VR4300Cycle& cpu);
void BGEZALL(VR4300Cycle& cpu);

//Special instructions
void SYNC(VR4300Cycle& cpu);
void SYSCALL(VR4300Cycle& cpu);
void BREAK(VR4300Cycle& cpu);
void TGE(VR4300Cycle& cpu);
void TGEU(VR4300Cycle& cpu);
void TLT(VR4300Cycle& cpu);
void TLTU(VR4300Cycle& cpu);
void TEQ(VR4300Cycle& cpu);
void TNE(VR4300Cycle& cpu);
void TGEI(VR4300Cycle& cpu);
void TGEIU(VR4300Cycle& cpu);
void TLTI(VR4300Cycle& cpu);
void TLTIU(VR4300Cycle& cpu);
void TEQI(VR4300Cycle& cpu);
void TNEI(VR4300Cycle& cpu);

//Coprocessor instructions
void LWCz(VR4300Cycle& cpu);
void SWCz(VR4300Cycle& cpu);
void MTCz(VR4300Cycle& cpu);
void MFCz(VR4300Cycle& cpu);
void CTCz(VR4300Cycle& cpu);
void CFCz(VR4300Cycle& cpu);
void COPz(VR4300Cycle& cpu);
void BCzT(VR4300Cycle& cpu);
void BCzF(VR4300Cycle& cpu);
void DMTCz(VR4300Cycle& cpu);
void DMFCz(VR4300Cycle& cpu);
void LDCz(VR4300Cycle& cpu);
void SDCz(VR4300Cycle& cpu);
void BCzTL(VR4300Cycle& cpu);
void BCzFL(VR4300Cycle& cpu);

//CP0 Instructions
void MTC0(VR4300Cycle& cpu);
void MFC0(VR4300Cycle& cpu);
void DMTC0(VR4300Cycle& cpu);
void DMFC0(VR4300Cycle& cpu);
void TLBR(VR4300Cycle& cpu);
void TLBWI(VR4300Cycle& cpu);
void TLBWR(VR4300Cycle& cpu);
void TLBP(VR4300Cycle& cpu);
void ERET(VR4300Cycle& cpu);
void CACHE(VR4300Cycle& cpu);

//fpu format
void CVTSfmt(VR4300Cycle& cpu);
void CVTDfmt(VR4300Cycle& cpu);
void CVTLfmt(VR4300Cycle& cpu);
void CVTWfmt(VR4300Cycle& cpu);
void ROUNDLfmt(VR4300Cycle& cpu);
void ROUNDWfmt(VR4300Cycle& cpu);
void TRUNCLfmt(VR4300Cycle& cpu);
void TRUNCWfmt(VR4300Cycle& cpu);
void CEILLfmt(VR4300Cycle& cpu);
void CEILWfmt(VR4300Cycle& cpu);
void FLOORLfmt(VR4300Cycle& cpu);
void FLOORWfmt(VR4300Cycle& cpu);

//fpu computational
void ADDfmt(VR4300Cycle& cpu);
void SUBfmt(VR4300Cycle& cpu);
void MULfmt(VR4300Cycle& cpu);
void DIVfmt(VR4300Cycle& cpu);
void ABSfmt(VR4300Cycle& cpu);
void MOVfmt(VR4300Cycle& cpu);
void NEGfmt(VR4300Cycle& cpu);
void SQRTfmt(VR4300Cycle& cpu);

//fpu compare
void Ccondfmt(VR4300Cycle& cpu);

extern const VR4300Cycle::OperationTemplate noptmplt;
extern const VR4300Cycle::OperationTemplate primary_op_lut[64];
extern const VR4300Cycle::OperationTemplate special_op_lut[64];
extern const VR4300Cycle::OperationTemplate regimm_op_lut[32];
extern const VR4300Cycle::OperationTemplate COP0rs_op_lut[32];
extern const VR4300Cycle::OperationTemplate COP0rt_op_lut[32];
extern const VR4300Cycle::OperationTemplate COP1rs_op_lut[32];
extern const VR4300Cycle::OperationTemplate COP1rt_op_lut[32];
extern const VR4300Cycle::OperationTemplate COP2rs_op_lut[32];
extern const VR4300Cycle::OperationTemplate COP2rt_op_lut[32];
extern const VR4300Cycle::OperationTemplate CP0_op_lut[32];
extern const VR4300Cycle::OperationTemplate CP1_op_lut[64];