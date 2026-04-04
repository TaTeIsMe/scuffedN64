
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
/*31*/ {LWCz, IS_LOAD | WRITES_REG | SIGN_EXTENDED | STORES_IN_RT | WRITES_CP,0, OpType::LWCz},
/*32*/ {LWCz, IS_LOAD | WRITES_REG | SIGN_EXTENDED | STORES_IN_RT | WRITES_CP,0, OpType::LWCz},
/*33*/ {},

/*34*/ {LLD, IS_LOAD | ACCESSES_DOUBLE_WORD | WRITES_REG | STORES_IN_RT | ATOMIC,0,OpType::LLD},
/*35*/ {LDCz, WRITES_REG | IS_LOAD | SIGN_EXTENDED | WRITES_CP | STORES_IN_RT ,0,OpType::LDCz},
/*36*/ {LDCz, WRITES_REG | IS_LOAD | SIGN_EXTENDED| WRITES_CP | STORES_IN_RT ,0,OpType::LDCz},
/*37*/ {LD, IS_LOAD | ACCESSES_DOUBLE_WORD | SIGN_EXTENDED | WRITES_REG | STORES_IN_RT,0,OpType::LD},

/*38*/ {SC, IS_STORE | ACCESSES_WORD | STORES_IN_RT | ATOMIC,0,OpType::SC}, //this technically writes reg but it's a special case since result has data to be stored
/*39*/ {SWCz, IS_STORE | READS_CP,0,OpType::SWCz},
/*3A*/ {SWCz, IS_STORE | READS_CP,0,OpType::SWCz},
/*3B*/ {},

/*3C*/ {SCD, IS_STORE | ACCESSES_DOUBLE_WORD | STORES_IN_RT | ATOMIC, 0, OpType::SCD}, //this technically writes reg but it's a s[ecial case since result has data to be stored
/*3D*/ {SDCz, IS_STORE | READS_CP,0,OpType::SDCz},
/*3E*/ {SDCz, IS_STORE | READS_CP,0,OpType::SDCz},
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
/*1C*/ {},
/*1D*/ {},
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
/*00*/ {MFCz, WRITES_REG | STORES_IN_RT | READS_CP,0,OpType::MFCz},
/*01*/ {DMFCz, WRITES_REG | STORES_IN_RT | READS_CP,0,OpType::DMFCz},
/*02*/ {CFCz, WRITES_REG | STORES_IN_RT | READS_CP,0,OpType::CFCz},
/*03*/ {},
/*04*/ {MTCz, WRITES_REG | WRITES_CP | STORES_IN_RD,0,OpType::MTCz},
/*05*/ {DMTCz, WRITES_REG | WRITES_CP | STORES_IN_RD,0,OpType::DMTCz},
/*06*/ {CTCz, WRITES_REG | WRITES_CP,0,OpType::CTCz},
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
/*00*/ {BCzF, CAUSES_BRANCH_DELAY,0,OpType::BCzF},
/*01*/ {BCzT, CAUSES_BRANCH_DELAY,0,OpType::BCzT},
/*02*/ {BCzFL, CAUSES_BRANCH_DELAY,0,OpType::BCzFL},
/*03*/ {BCzTL, CAUSES_BRANCH_DELAY,0,OpType::BCzTL},

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
/*01*/ {TLBR, 0,0,OpType::TLBR},
/*02*/ {TLBWI, 0,0,OpType::TLBWI},
/*03*/ {},
/*04*/ {},
/*05*/ {},
/*06*/ {TLBWR, 0,0,OpType::TLBWR},
/*07*/ {},

/*08*/ {TLBP, 0,0,OpType::TLBP},
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

/*18*/ {ERET, CAUSES_BRANCH_DELAY,0,OpType::ERET},
/*19*/ {},
/*1A*/ {},
/*1B*/ {},
/*1C*/ {},
/*1D*/ {},
/*1E*/ {},
/*1F*/ {}
};