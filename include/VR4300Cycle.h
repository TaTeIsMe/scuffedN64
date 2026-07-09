#pragma once
#include <cstdint>
#include "CP0.h"
#include "FPU.h"
#include<iostream>
#include "VR4300.h"

#define CACHE_OP_STALL_TIME 32
#define DCACHE_STALL_TIME 10
#define ICACHE_STALL_TIME 10

enum class OpType : uint32_t {
#   define X(a) a,
#   include "op_enum.def"
#   undef X
    OpTypeCount
};

char const* const optype_str[] = {
#   define X(a) #a,
#   include "op_enum.def"
#   undef X
    0
};

//these are a mess but it's fine...
enum OpFlags:uint32_t{
    WRITES_REG = 1<<4,
    CAUSED_EXCEPTION = 1<<5,
    READS_CP0 = 1<<6,
    RIGHT_ACCESS = 1<<7,
    IS_STORE = 1<<8,
    IS_LOAD = 1<<9,
    STORES_IN_RT = 1 << 10,
    STORES_IN_RD = 1 << 11,
    CAUSES_BRANCH_DELAY = 1<<13,
    SIGN_EXTENDED = 1 << 14,
    LEFT_ACCESS = 1 << 15,
    CAUSES_OVERFLOW_EXCEPTION = 1<<16,
    WRITES_LO = 1<<17,
    STORES_IN_31 = 1<<18,
    IS_TRAP = 1<<19,
    READS_CP = 1 <<20,
    WRITES_HI = 1<<21,
    ATOMIC = 1<<22,
    WRITES_CP = 1 <<23,
    CPControl = 1 << 24,
    STORES_IN_SA = 1 << 25,
    CPZ = 1 << 26
};

enum DestId{
    RT = 0,
    RD = 1,
    REG31 = 2,
    SA = 3
};

enum class RegFile{
    CPU_GPR,
    CP0_REGS,
    FPU_REGS,
    FPU_CONTROL_REGS
};

class VR4300Cycle : public VR4300
{
public:
    VR4300Cycle();
    
    FPU fpu;
    CP0 cp0;
    class RCP* rcp;
    uint64_t PC;
    uint64_t cycle_count;

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
            uint64_t HI;
            uint64_t LO;
        };
        uint64_t GPR[34];
    };
    
    struct OperationTemplate{
        OperationTemplate();
        OperationTemplate(void (*execute)(VR4300Cycle& cpu), uint32_t flags, uint8_t multicycle, uint8_t access_size,uint8_t CPz, OpType instruction_type);
        void (*execute)(VR4300Cycle& cpu) = nullptr;
        RegFile rt_source_reg_file;
        RegFile rd_source_reg_file;
        RegFile dest_reg_file;
        uint32_t flags = 0;
        uint8_t CPz = 0;
        uint8_t dest_id = 0;
        uint8_t multicycle = 0;
        uint8_t access_size = 0;
        OpType instruction_type = OpType::NOP;
        bool reads_cp = false;
        bool causes_bd = false;
        bool cp_control = false;
        bool writes_cp = false;
        bool writes_hi = false;
        bool writes_lo = false;
        bool writes_reg = false;
        bool right_access = false;
        bool left_access = false;
        bool stores_in_rt = false;
        bool stores_in_rd = false;
        bool stores_in_31 = false;
        bool stores_in_sa = false;
        bool atomic = false;
        bool is_cpz = false;
        bool is_load = false;
        bool is_store = false;
        bool sign_extended = false;
        bool is_trap = false;
    };
    
    struct Operation{
        const OperationTemplate* tmplt;
        Operation();
        uint64_t rs_val = 0;//needed during operation
        uint64_t rt_val = 0;//needed during operation
        uint64_t PC = 0;//virtual address of instruction

        
        union {
            // TLB instructions
            struct {
                uint64_t result_entryHI;
                uint64_t result_entryLO0;
                uint64_t result_entryLO1;
                uint64_t result_pagemask;
            };
            // Multiply and division
            struct {
                uint64_t result_HI;
                uint64_t result_LO;
            };
            // Standard ALU & Memory operations (Load/Store)
            struct {
                uint64_t result;
                uint64_t data_addr;
                uint64_t data_addr_p;
                uint64_t rd_val;
            };
        };
        uint64_t dcache_index = 0;
        uint32_t opcode = 0;
        uint32_t icache_index = 0;
        
        union{
            uint8_t dest_options[4]{0,0,31,0};
            struct{
                uint8_t rt;
                uint8_t rd;
                uint8_t reg31;
                uint8_t sa;
            };
        };
        bool cacheable = false;
        bool fire_fpu_exception = false;
        bool bd = false;
        uint8_t rs = 0;//reg numebr
        uint8_t source_reg = 0;
        uint8_t dest_reg = 0;
        uint8_t conditional_val = 0;
        const char* op_name() const;
        friend std::ostream& operator<<(std::ostream& os, const Operation& op);
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
        Operation data[8];
    };

    Dcache_line Dcache[512];
    Icache_line Icache[512];

    inline bool decode_op(uint32_t word, Operation& op);

    void abort_pipeline();

    void handle_tlb_miss_exception(uint64_t addr, const Operation& op, ExceptionCode cause);

    void handle_general_exception(const Operation& op, ExceptionCode cause);

    void update_hardware_interrupt(uint8_t enable, uint8_t value) override;

    void set_tlb_context(uint64_t addr);

    struct WB_DC{
        Operation* op;
        bool CP0I_triggered;
    };

    struct EX_DC{
        Operation* op;
        bool DCB_triggered;
        bool COp_triggered; // all these flags might have to be moved from ins to outs. That will also require them to be reset on submit pipeline
        bool uncacheable_stall_triggered;
    };
    bool update_conditional;
    struct RF_EX{
        Operation* op;
        bool MCI_triggered;
        bool LDI_triggered;
    };
    bool next_op_bd;
    bool discard_bd;
    struct IC_RF{
        Operation* op;
        bool uncacheable_stall_triggered;
        bool ICB_triggered;
    };

    uint16_t stall;
    uint8_t stall_depth;
    void on_pclock() override;

    Operation op_storage[4]{};
    //Pipeline stages.
    inline bool WB(); // write back
    WB_DC WB_in{};
    inline bool DC(); // data cache fetch
    EX_DC DC_in{};
    inline bool EX(); // execute
    RF_EX EX_in{};
    inline bool RF(); // register fetch
    void fetch_regs();
    IC_RF RF_in{};
    inline bool IC(); // instruction cache fetch

    
    inline void submit_pipeline();
    void dcache_write_size(Dcache_line &line, uint8_t offset, uint64_t value, uint8_t size);
    uint64_t dcache_read_size(const Dcache_line &line, uint8_t offset, uint8_t size);

    uint8_t handle_cache_op(const Operation& op);
    void dcache_write_back(Dcache_line& line, uint16_t index);

    inline uint64_t fetch_reg(RegFile reg_file, uint8_t reg_num);
    inline void forward_write (const VR4300Cycle::Operation& stage_op, VR4300Cycle::Operation& in_op);

};

