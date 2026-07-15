#pragma once
#include"VR4300Interpreter.h"

using Instruction = void (*)(VR4300Interpreter&, uint32_t op_code);

void NOP(VR4300Interpreter &cpu, uint32_t op_code);

// Load/store instructions
void LB(VR4300Interpreter &cpu, uint32_t op_code);
void LBU(VR4300Interpreter &cpu, uint32_t op_code);
void LH(VR4300Interpreter &cpu, uint32_t op_code);
void LHU(VR4300Interpreter &cpu, uint32_t op_code);
void LW(VR4300Interpreter &cpu, uint32_t op_code);
void LWL(VR4300Interpreter &cpu, uint32_t op_code); 
void LWR(VR4300Interpreter &cpu, uint32_t op_code);
void SB(VR4300Interpreter &cpu, uint32_t op_code);
void SH(VR4300Interpreter &cpu, uint32_t op_code);
void SW(VR4300Interpreter &cpu, uint32_t op_code);
void SWL(VR4300Interpreter &cpu, uint32_t op_code);
void SWR(VR4300Interpreter &cpu, uint32_t op_code);
void LD(VR4300Interpreter &cpu, uint32_t op_code);
void LDL(VR4300Interpreter &cpu, uint32_t op_code);
void LDR(VR4300Interpreter &cpu, uint32_t op_code);
void LL(VR4300Interpreter &cpu, uint32_t op_code);
void LLD(VR4300Interpreter &cpu, uint32_t op_code);
void LWU(VR4300Interpreter &cpu, uint32_t op_code);
void SC(VR4300Interpreter &cpu, uint32_t op_code);
void SCD(VR4300Interpreter &cpu, uint32_t op_code);
void SD(VR4300Interpreter &cpu, uint32_t op_code);
void SDL(VR4300Interpreter &cpu, uint32_t op_code);
void SDR(VR4300Interpreter &cpu, uint32_t op_code);

//Computational instructions
void ADDI(VR4300Interpreter &cpu, uint32_t op_code);
void ADDIU(VR4300Interpreter &cpu, uint32_t op_code);
void SLTI(VR4300Interpreter &cpu, uint32_t op_code);
void SLTIU(VR4300Interpreter &cpu, uint32_t op_code);
void ANDI(VR4300Interpreter &cpu, uint32_t op_code);
void ORI(VR4300Interpreter &cpu, uint32_t op_code);
void XORI(VR4300Interpreter &cpu, uint32_t op_code);
void LUI(VR4300Interpreter &cpu, uint32_t op_code);
void DADDI(VR4300Interpreter &cpu, uint32_t op_code);
void DADDIU(VR4300Interpreter &cpu, uint32_t op_code);
void ADD(VR4300Interpreter &cpu, uint32_t op_code);
void ADDU(VR4300Interpreter &cpu, uint32_t op_code);
void SUB(VR4300Interpreter &cpu, uint32_t op_code);
void SUBU(VR4300Interpreter &cpu, uint32_t op_code);
void SLT(VR4300Interpreter &cpu, uint32_t op_code);
void SLTU(VR4300Interpreter &cpu, uint32_t op_code);
void AND(VR4300Interpreter &cpu, uint32_t op_code);
void OR(VR4300Interpreter &cpu, uint32_t op_code);
void XOR(VR4300Interpreter &cpu, uint32_t op_code);
void NOR(VR4300Interpreter &cpu, uint32_t op_code);
void DADD(VR4300Interpreter &cpu, uint32_t op_code);
void DADDU(VR4300Interpreter &cpu, uint32_t op_code);
void DSUB(VR4300Interpreter &cpu, uint32_t op_code);
void DSUBU(VR4300Interpreter &cpu, uint32_t op_code);

//Shift instrucitons
void SLL(VR4300Interpreter &cpu, uint32_t op_code);
void SRL(VR4300Interpreter &cpu, uint32_t op_code);
void SRA(VR4300Interpreter &cpu, uint32_t op_code);
void SLLV(VR4300Interpreter &cpu, uint32_t op_code);
void SRLV(VR4300Interpreter &cpu, uint32_t op_code);
void SRAV(VR4300Interpreter &cpu, uint32_t op_code);
void DSLL(VR4300Interpreter &cpu, uint32_t op_code);
void DSRL(VR4300Interpreter &cpu, uint32_t op_code);
void DSRA(VR4300Interpreter &cpu, uint32_t op_code);
void DSLLV(VR4300Interpreter &cpu, uint32_t op_code);
void DSRLV(VR4300Interpreter &cpu, uint32_t op_code);
void DSRAV(VR4300Interpreter &cpu, uint32_t op_code);
void DSLL32(VR4300Interpreter &cpu, uint32_t op_code);
void DSRL32(VR4300Interpreter &cpu, uint32_t op_code);
void DSRA32(VR4300Interpreter &cpu, uint32_t op_code);

//Multiply/Divide instructions
void MULT(VR4300Interpreter &cpu, uint32_t op_code);
void MULTU(VR4300Interpreter &cpu, uint32_t op_code);
void DIV(VR4300Interpreter &cpu, uint32_t op_code);
void DIVU(VR4300Interpreter &cpu, uint32_t op_code);
void MFHI(VR4300Interpreter &cpu, uint32_t op_code);
void MFLO(VR4300Interpreter &cpu, uint32_t op_code);
void MTHI(VR4300Interpreter &cpu, uint32_t op_code);
void MTLO(VR4300Interpreter &cpu, uint32_t op_code);
void DMULT(VR4300Interpreter &cpu, uint32_t op_code);
void DMULTU(VR4300Interpreter &cpu, uint32_t op_code);
void DDIV(VR4300Interpreter &cpu, uint32_t op_code);
void DDIVU(VR4300Interpreter &cpu, uint32_t op_code);

//Jump/branch instructions
void J(VR4300Interpreter &cpu, uint32_t op_code);
void JAL(VR4300Interpreter &cpu, uint32_t op_code);
void JR(VR4300Interpreter &cpu, uint32_t op_code);
void JALR(VR4300Interpreter &cpu, uint32_t op_code);
void BEQ(VR4300Interpreter &cpu, uint32_t op_code);
void BNE(VR4300Interpreter &cpu, uint32_t op_code);
void BLEZ(VR4300Interpreter &cpu, uint32_t op_code);
void BGTZ(VR4300Interpreter &cpu, uint32_t op_code);
void BLTZ(VR4300Interpreter &cpu, uint32_t op_code);
void BGEZ(VR4300Interpreter &cpu, uint32_t op_code);
void BLTZAL(VR4300Interpreter &cpu, uint32_t op_code);
void BGEZAL(VR4300Interpreter &cpu, uint32_t op_code);
void BEQL(VR4300Interpreter &cpu, uint32_t op_code);
void BNEL(VR4300Interpreter &cpu, uint32_t op_code);
void BLEZL(VR4300Interpreter &cpu, uint32_t op_code);
void BGTZL(VR4300Interpreter &cpu, uint32_t op_code);
void BLTZL(VR4300Interpreter &cpu, uint32_t op_code);
void BGEZL(VR4300Interpreter &cpu, uint32_t op_code);
void BLTZALL(VR4300Interpreter &cpu, uint32_t op_code);
void BGEZALL(VR4300Interpreter &cpu, uint32_t op_code);

//Special instructions
void SYNC(VR4300Interpreter &cpu, uint32_t op_code);
void SYSCALL(VR4300Interpreter &cpu, uint32_t op_code);
void BREAK(VR4300Interpreter &cpu, uint32_t op_code);
void TGE(VR4300Interpreter &cpu, uint32_t op_code);
void TGEU(VR4300Interpreter &cpu, uint32_t op_code);
void TLT(VR4300Interpreter &cpu, uint32_t op_code);
void TLTU(VR4300Interpreter &cpu, uint32_t op_code);
void TEQ(VR4300Interpreter &cpu, uint32_t op_code);
void TNE(VR4300Interpreter &cpu, uint32_t op_code);
void TGEI(VR4300Interpreter &cpu, uint32_t op_code);
void TGEIU(VR4300Interpreter &cpu, uint32_t op_code);
void TLTI(VR4300Interpreter &cpu, uint32_t op_code);
void TLTIU(VR4300Interpreter &cpu, uint32_t op_code);
void TEQI(VR4300Interpreter &cpu, uint32_t op_code);
void TNEI(VR4300Interpreter &cpu, uint32_t op_code);

//Coprocessor instructions
void LWCz(VR4300Interpreter &cpu, uint32_t op_code);
void SWCz(VR4300Interpreter &cpu, uint32_t op_code);
void MTCz(VR4300Interpreter &cpu, uint32_t op_code);
void MFCz(VR4300Interpreter &cpu, uint32_t op_code);
void CTCz(VR4300Interpreter &cpu, uint32_t op_code);
void CFCz(VR4300Interpreter &cpu, uint32_t op_code);
void COPz(VR4300Interpreter &cpu, uint32_t op_code);
void BCzT(VR4300Interpreter &cpu, uint32_t op_code);
void BCzF(VR4300Interpreter &cpu, uint32_t op_code);
void DMTCz(VR4300Interpreter &cpu, uint32_t op_code);
void DMFCz(VR4300Interpreter &cpu, uint32_t op_code);
void LDCz(VR4300Interpreter &cpu, uint32_t op_code);
void SDCz(VR4300Interpreter &cpu, uint32_t op_code);
void BCzTL(VR4300Interpreter &cpu, uint32_t op_code);
void BCzFL(VR4300Interpreter &cpu, uint32_t op_code);

//CP0 Instructions
void MTC0(VR4300Interpreter &cpu, uint32_t op_code);
void MFC0(VR4300Interpreter &cpu, uint32_t op_code);
void DMTC0(VR4300Interpreter &cpu, uint32_t op_code);
void DMFC0(VR4300Interpreter &cpu, uint32_t op_code);
void TLBR(VR4300Interpreter &cpu, uint32_t op_code);
void TLBWI(VR4300Interpreter &cpu, uint32_t op_code);
void TLBWR(VR4300Interpreter &cpu, uint32_t op_code);
void TLBP(VR4300Interpreter &cpu, uint32_t op_code);
void ERET(VR4300Interpreter &cpu, uint32_t op_code);
void CACHE(VR4300Interpreter &cpu, uint32_t op_code);

//fpu format
void CVTSS(VR4300Interpreter &cpu, uint32_t op_code);
void CVTSD(VR4300Interpreter &cpu, uint32_t op_code);
void CVTSW(VR4300Interpreter &cpu, uint32_t op_code);
void CVTSL(VR4300Interpreter &cpu, uint32_t op_code);

void CVTDS(VR4300Interpreter &cpu, uint32_t op_code);
void CVTDD(VR4300Interpreter &cpu, uint32_t op_code);
void CVTDW(VR4300Interpreter &cpu, uint32_t op_code);
void CVTDL(VR4300Interpreter &cpu, uint32_t op_code);

void CVTLS(VR4300Interpreter &cpu, uint32_t op_code);
void CVTLD(VR4300Interpreter &cpu, uint32_t op_code);
void CVTLW(VR4300Interpreter &cpu, uint32_t op_code);
void CVTLL(VR4300Interpreter &cpu, uint32_t op_code);

void CVTWS(VR4300Interpreter &cpu, uint32_t op_code);
void CVTWD(VR4300Interpreter &cpu, uint32_t op_code);
void CVTWW(VR4300Interpreter &cpu, uint32_t op_code);
void CVTWL(VR4300Interpreter &cpu, uint32_t op_code);

void ROUNDLS(VR4300Interpreter &cpu, uint32_t op_code);
void ROUNDLD(VR4300Interpreter &cpu, uint32_t op_code);
void ROUNDLW(VR4300Interpreter &cpu, uint32_t op_code);
void ROUNDLL(VR4300Interpreter &cpu, uint32_t op_code);

void ROUNDWS(VR4300Interpreter &cpu, uint32_t op_code);
void ROUNDWD(VR4300Interpreter &cpu, uint32_t op_code);
void ROUNDWW(VR4300Interpreter &cpu, uint32_t op_code);
void ROUNDWL(VR4300Interpreter &cpu, uint32_t op_code);

void TRUNCLS(VR4300Interpreter &cpu, uint32_t op_code);
void TRUNCLD(VR4300Interpreter &cpu, uint32_t op_code);
void TRUNCLW(VR4300Interpreter &cpu, uint32_t op_code);
void TRUNCLL(VR4300Interpreter &cpu, uint32_t op_code);

void TRUNCWS(VR4300Interpreter &cpu, uint32_t op_code);
void TRUNCWD(VR4300Interpreter &cpu, uint32_t op_code);
void TRUNCWW(VR4300Interpreter &cpu, uint32_t op_code);
void TRUNCWL(VR4300Interpreter &cpu, uint32_t op_code);

void CEILLS(VR4300Interpreter &cpu, uint32_t op_code);
void CEILLD(VR4300Interpreter &cpu, uint32_t op_code);
void CEILLW(VR4300Interpreter &cpu, uint32_t op_code);
void CEILLL(VR4300Interpreter &cpu, uint32_t op_code);

void CEILWS(VR4300Interpreter &cpu, uint32_t op_code);
void CEILWD(VR4300Interpreter &cpu, uint32_t op_code);
void CEILWW(VR4300Interpreter &cpu, uint32_t op_code);
void CEILWL(VR4300Interpreter &cpu, uint32_t op_code);

void FLOORLS(VR4300Interpreter &cpu, uint32_t op_code);
void FLOORLD(VR4300Interpreter &cpu, uint32_t op_code);
void FLOORLW(VR4300Interpreter &cpu, uint32_t op_code);
void FLOORLL(VR4300Interpreter &cpu, uint32_t op_code);

void FLOORWS(VR4300Interpreter &cpu, uint32_t op_code);
void FLOORWD(VR4300Interpreter &cpu, uint32_t op_code);
void FLOORWW(VR4300Interpreter &cpu, uint32_t op_code);
void FLOORWL(VR4300Interpreter &cpu, uint32_t op_code);

//fpu computational
void ADDS(VR4300Interpreter &cpu, uint32_t op_code);
void ADDD(VR4300Interpreter &cpu, uint32_t op_code);
void ADDW(VR4300Interpreter &cpu, uint32_t op_code);
void ADDL(VR4300Interpreter &cpu, uint32_t op_code);

void SUBS(VR4300Interpreter &cpu, uint32_t op_code);
void SUBD(VR4300Interpreter &cpu, uint32_t op_code);
void SUBW(VR4300Interpreter &cpu, uint32_t op_code);
void SUBL(VR4300Interpreter &cpu, uint32_t op_code);

void MULS(VR4300Interpreter &cpu, uint32_t op_code);
void MULD(VR4300Interpreter &cpu, uint32_t op_code);
void MULW(VR4300Interpreter &cpu, uint32_t op_code);
void MULL(VR4300Interpreter &cpu, uint32_t op_code);

void DIVS(VR4300Interpreter &cpu, uint32_t op_code);
void DIVD(VR4300Interpreter &cpu, uint32_t op_code);
void DIVW(VR4300Interpreter &cpu, uint32_t op_code);
void DIVL(VR4300Interpreter &cpu, uint32_t op_code);

void ABSS(VR4300Interpreter &cpu, uint32_t op_code);
void ABSD(VR4300Interpreter &cpu, uint32_t op_code);
void ABSW(VR4300Interpreter &cpu, uint32_t op_code);
void ABSL(VR4300Interpreter &cpu, uint32_t op_code);

void MOVS(VR4300Interpreter &cpu, uint32_t op_code);
void MOVD(VR4300Interpreter &cpu, uint32_t op_code);
void MOVW(VR4300Interpreter &cpu, uint32_t op_code);
void MOVL(VR4300Interpreter &cpu, uint32_t op_code);

void NEGS(VR4300Interpreter &cpu, uint32_t op_code);
void NEGD(VR4300Interpreter &cpu, uint32_t op_code);
void NEGW(VR4300Interpreter &cpu, uint32_t op_code);
void NEGL(VR4300Interpreter &cpu, uint32_t op_code);

void SQRTS(VR4300Interpreter &cpu, uint32_t op_code);
void SQRTD(VR4300Interpreter &cpu, uint32_t op_code);
void SQRTW(VR4300Interpreter &cpu, uint32_t op_code);
void SQRTL(VR4300Interpreter &cpu, uint32_t op_code);

//fpu compare
void CcondS(VR4300Interpreter &cpu, uint32_t op_code);
void CcondD(VR4300Interpreter &cpu, uint32_t op_code);
void CcondW(VR4300Interpreter &cpu, uint32_t op_code);
void CcondL(VR4300Interpreter &cpu, uint32_t op_code);

extern const Instruction i_primary_op_lut[64];
extern const Instruction i_special_op_lut[64];
extern const Instruction i_regimm_op_lut[32];
extern const Instruction i_COP0rs_op_lut[32];
extern const Instruction i_COP0rt_op_lut[32];
extern const Instruction i_COP1rs_op_lut[32];
extern const Instruction i_COP1rt_op_lut[32];
extern const Instruction i_COP2rs_op_lut[32];
extern const Instruction i_COP2rt_op_lut[32];
extern const Instruction i_CP0_op_lut[32];
extern const Instruction i_CP1_op_lut_S[64];
extern const Instruction i_CP1_op_lut_D[64];
extern const Instruction i_CP1_op_lut_W[64];
extern const Instruction i_CP1_op_lut_L[64];

extern const char* i_primary_op_name_lut[64];
extern const char* i_special_op_name_lut[64];
extern const char* i_regimm_op_name_lut[32];
extern const char* i_COP0rs_op_name_lut[32];
extern const char* i_COP0rt_op_name_lut[32];
extern const char* i_COP1rs_op_name_lut[32];
extern const char* i_COP1rt_op_name_lut[32];
extern const char* i_COP2rs_op_name_lut[32];
extern const char* i_COP2rt_op_name_lut[32];
extern const char* i_CP0_op_name_lut[32];
extern const char* i_CP1_op_name_lut[64];
