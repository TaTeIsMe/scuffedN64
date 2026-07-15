#pragma once
#include <cstdint>
#include "CP0.h"
#include "FPU.h"
#include<iostream>
#include "VR4300.h"
#include<optional>

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


enum class RegFile{
    CPU_GPR,
    CP0_REGS,
    FPU_REGS,
    FPU_CONTROL_REGS
};

class VR4300Interpreter : public VR4300
{
public:
    VR4300Interpreter();
    
    FPU fpu;
    CP0 cp0;
    class RCP* rcp;
    uint64_t PC = 0;
    uint64_t next_PC = 0;
    bool is_bd = false;
    bool increment = false;

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
        uint64_t GPR[34]{};
    };

    void handle_tlb_miss_exception(uint64_t op_PC, uint64_t addr, ExceptionCode cause);

    void handle_general_exception(uint64_t op_PC, ExceptionCode cause);

    bool check_fpu_exception(uint64_t op_PC);

    void update_hardware_interrupt(uint8_t enable, uint8_t value) override;

    void dec_random();

    void inc_count();

    void set_tlb_context(uint64_t addr);

    void do_instruction();

    void do_bd();

    void on_pclock() override;

    void print_instruction(uint32_t op_code);

    bool validate_PC_align(uint64_t PC);

    bool cp_usable(uint8_t cp);

    std::optional<uint32_t> translate_address(uint64_t v_addr);

};

