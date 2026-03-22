#include <cstdint>
#include "CP0.h"
#include "FPU.h"
#include "Bus.h"

#pragma once

#define CACHE_OP_STALL_TIME 2
#define DCACHE_STALL_TIME 2
#define ICACHE_STALL_TIME 2

enum OpTypes:uint32_t{
    NOP_I,
    ICACHE_I,
    DCACHE_I,
    SYSCALL_I,
    BREAK_I,
    ADDI_I,
    ADD_I
};

//these are a mess but it's fine...
enum OpFlags:uint32_t{
    ACCESSES_DOUBLE_WORD = 1<<0,
    ACCESSES_HALF_WORD = 1<<1,
    ACCESSES_WORD = 1<<2,
    WRITES_DATA = 1<<3,
    WRITES_REG = 1<<4,
    CAUSED_EXCEPTION = 1<<5,
    READS_CP0 = 1<<6,
    READS_DATA = 1<<7,
    IS_STORE = 1<<8,
    IS_LOAD = 1<<9,
    STORES_IN_RT = 1 << 10,
    STORES_IN_RD = 1 << 11,
    IS_IN_BRANCH_DELAY = 1<<12,
    CAUSES_BRANCH_DELAY = 1<<13,
    SIGNED_EXTENDED = 1 << 14,
    ACCESSES_BYTE = 1<<15,
    CAUSES_OVERFLOW_EXCEPTION = 1<<16,
    WRITES_HILO = 1<<17,
    STORES_IN_31 = 1<<18,
    IS_TRAP = 1<<19,
    ACCESSES_CP = 1 <<20
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
        OpTypes instruction_type = NOP_I;
    };
    
    struct Operation : OperationTemplate{
        Operation();
        uint8_t rs = 0;
        uint8_t rt = 0;
        uint8_t rd = 0;
        uint8_t sa = 0;
        uint8_t CPz = 0;
        uint16_t immediate = 0;
        uint32_t target = 0;
        uint32_t result_entryHI = 0;
        uint32_t result_entryLO0 = 0;
        uint32_t result_entryLO1 = 0;
        uint32_t result_pagemask = 0;
        uint64_t write_destination = 0;
        uint64_t rs_val = 0;
        uint64_t rt_val = 0;
        uint64_t cp_val = 0;
        uint64_t result = 0;
        uint64_t result_HI = 0;
        uint64_t result_LO = 0;
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

