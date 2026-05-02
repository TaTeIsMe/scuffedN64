#pragma once

#include<cstdint>
#include"CP0.h"

#define CAUSE_INEXACT_SHIFT 12
#define CAUSE_UNDERFLOW_SHIFT 13
#define CAUSE_OVERFLOW_SHIFT 14
#define CAUSE_ZERODIV_SHIFT 15
#define CAUSE_INVALID_SHIFT 16
#define CAUSE_UNIMPLEMENTED_SHIFT 17

class FPU
{
public:
    FPU(CP0& cp0);
    ~FPU();
    
    CP0 &cp0;

    uint32_t FCR0 = 0xa00;
    uint32_t FCR31;

    union {
        struct {
            uint64_t FGR0;
            uint64_t FGR1;
            uint64_t FGR2;
            uint64_t FGR3;
            uint64_t FGR4;
            uint64_t FGR5;
            uint64_t FGR6;
            uint64_t FGR7;
            uint64_t FGR8;
            uint64_t FGR9;
            uint64_t FGR10;
            uint64_t FGR11;
            uint64_t FGR12;
            uint64_t FGR13;
            uint64_t FGR14;
            uint64_t FGR15;
            uint64_t FGR16;
            uint64_t FGR17;
            uint64_t FGR18;
            uint64_t FGR19;
            uint64_t FGR20;
            uint64_t FGR21;
            uint64_t FGR22;
            uint64_t FGR23;
            uint64_t FGR24;
            uint64_t FGR25;
            uint64_t FGR26;
            uint64_t FGR27;
            uint64_t FGR28;
            uint64_t FGR29;
            uint64_t FGR30;
            uint64_t FGR31;
        };
        uint64_t regs[32];
    };

    uint64_t get_fpr(uint8_t fpr, uint8_t write_size);
    void write_fpr(uint8_t fpr, uint64_t val, uint8_t write_size);

private:

};
