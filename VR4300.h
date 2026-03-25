#include <cstdint>
#include "CP0.h"
#include "FPU.h"
#include "Bus.h"

#pragma once

#define CACHE_OP_STALL_TIME 2
#define DCACHE_STALL_TIME 2
#define ICACHE_STALL_TIME 2

enum OpTypes : uint32_t {
    SPECIAL_I,
    REGIMM_I,
    LB_I,
    LBU_I,
    LH_I,
    LHU_I,
    LW_I,
    LWL_I,
    LWR_I,
    SB_I,
    SH_I,
    SW_I,
    SWL_I,
    SWR_I,
    LD_I,
    LDL_I,
    LDR_I,
    LL_I,
    LLD_I,
    LWU_I,
    SC_I,
    SCD_I,
    SD_I,
    SDL_I,
    SDR_I,
    ADDI_I,
    ADDIU_I,
    SLTI_I,
    SLTIU_I,
    ANDI_I,
    ORI_I,
    XORI_I,
    LUI_I,
    DADDI_I,
    DADDIU_I,
    ADD_I,
    ADDU_I,
    SUB_I,
    SUBU_I,
    SLT_I,
    SLTU_I,
    AND_I,
    OR_I,
    XOR_I,
    NOR_I,
    DADD_I,
    DADDU_I,
    DSUB_I,
    DSUBU_I,
    SLL_I,
    SRL_I,
    SRA_I,
    SLLV_I,
    SRLV_I,
    SRAV_I,
    DSLL_I,
    DSRL_I,
    DSRA_I,
    DSLLV_I,
    DSRLV_I,
    DSRAV_I,
    DSLL32_I,
    DSRL32_I,
    DSRA32_I,
    MULT_I,
    MULTU_I,
    DIV_I,
    DIVU_I,
    MFHI_I,
    MFLO_I,
    MTHI_I,
    MTLO_I,
    DMULT_I,
    DMULTU_I,
    DDIV_I,
    DDIVU_I,
    J_I,
    JAL_I,
    JR_I,
    JALR_I,
    BEQ_I,
    BNE_I,
    BLEZ_I,
    BGTZ_I,
    BLTZ_I,
    BGEZ_I,
    BLTZAL_I,
    BGEZAL_I,
    BEQL_I,
    BNEL_I,
    BLEZL_I,
    BGTZL_I,
    BLTZL_I,
    BGEZL_I,
    BLTZALL_I,
    BGEZALL_I,
    SYNC_I,
    SYSCALL_I,
    BREAK_I,
    TGE_I,
    TGEU_I,
    TLT_I,
    TLTU_I,
    TEQ_I,
    TNE_I,
    TGEI_I,
    TGEIU_I,
    TLTI_I,
    TLTIU_I,
    TEQI_I,
    TNEI_I,
    LWCz_I,
    SWCz_I,
    MTCz_I,
    MFCz_I,
    CTCz_I,
    CFCz_I,
    COPz_I,
    BCzT_I,
    BCzF_I,
    DMTCz_I,
    DMFCz_I,
    LDCz_I,
    SDCz_I,
    BCzTL_I,
    BCzFL_I,
    MTC0_I,
    MFC0_I,
    DMTC0_I,
    DMFC0_I,
    TLBR_I,
    TLBWI_I,
    TLBWR_I,
    TLBP_I,
    ERET_I,
    CACHE_I,
    NOP_I,
};

//these are a mess but it's fine...
enum OpFlags:uint32_t{
    ACCESSES_BYTE = 1<<0,
    ACCESSES_HALF_WORD = 1<<1,
    ACCESSES_WORD = 1<<2,
    ACCESSES_DOUBLE_WORD = 1<<3,
    WRITES_REG = 1<<4,
    CAUSED_EXCEPTION = 1<<5,
    READS_CP0 = 1<<6,
    RIGHT_ACCESS = 1<<7,
    IS_STORE = 1<<8,
    IS_LOAD = 1<<9,
    STORES_IN_RT = 1 << 10,
    STORES_IN_RD = 1 << 11,
    IS_IN_BRANCH_DELAY = 1<<12,
    CAUSES_BRANCH_DELAY = 1<<13,
    SIGN_EXTENDED = 1 << 14,
    LEFT_ACCESS = 1 << 15,
    CAUSES_OVERFLOW_EXCEPTION = 1<<16,
    WRITES_LO = 1<<17,
    STORES_IN_31 = 1<<18,
    IS_TRAP = 1<<19,
    ACCESSES_CP = 1 <<20,
    WRITES_HI = 1<<21
};

class VR4300
{
public:
    VR4300(Bus& bus);
    ~VR4300();

    FPU fpu;
    CP0 cp0;
    Bus& bus;

    uint64_t PC = 0xFFFFFFFFA0000000;
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
        uint8_t data[16];
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
        OpTypes instruction_type = NOP_I;
    };
    
    struct Operation : OperationTemplate{
        Operation();
        uint8_t rs = 0;//reg numebr
        uint8_t rt = 0;//reg number
        uint8_t rd = 0;//reg number
        uint8_t sa = 0;//
        uint8_t CPz = 0;//for cp instructions
        uint8_t dest_reg = 0;
        uint16_t immediate = 0;//needed during operation, also doubles as offset during operation
        uint32_t target = 0;//for jumps
        uint32_t result_entryHI = 0;//for tlb instructions
        uint32_t result_entryLO0 = 0;//for tlb instructions
        uint32_t result_entryLO1 = 0;//for tlb instructions
        uint32_t result_pagemask = 0;//for tlb instructions
        uint64_t data_addr = 0; //doubles as virtual address and reg written to
        uint64_t data_addr_p = 0; //write dest but physical
        uint64_t rs_val = 0;//needed during operation
        uint64_t rt_val = 0;//needed during operation
        uint64_t cp_val = 0;//for all the cp operations
        uint64_t result = 0; //what is stored
        uint64_t result_HI = 0;//for multiplying and division
        uint64_t result_LO = 0;//for multiplying and division
        uint64_t PC = 0;//virtual address of instruction
        uint64_t dcache_index = 0;//for cache writing
    };
    
    void decode_op(uint32_t word);

    void abort_pipeline();

    void handle_tlb_miss_exception(uint64_t addr, const Operation &op, ExceptionCode cause);

    void handle_general_exception(const Operation &op, ExceptionCode cause);

    void set_tlb_context(uint64_t addr);

    struct WB_DC{
        Operation op;
        bool cacheable;
        bool CP0I_triggered;
    };

    struct EX_DC{
        Operation op;
        bool DCB_triggered;
        bool uncacheable_stall_triggered;
    };
    struct RF_EX{
        Operation op;
        bool MCI_triggered;
        bool LDI_triggered;
    };
    bool next_op_bd;
    struct IC_RF{
        Operation op;
        bool uncacheable_stall_triggered;
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
    void read_access_size(uint8_t *address, VR4300::Operation &op);
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

