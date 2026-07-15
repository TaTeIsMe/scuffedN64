#pragma once

#include<cstdint>
#include"CP0.h"

#define CAUSE_INEXACT_SHIFT 12
#define CAUSE_UNDERFLOW_SHIFT 13
#define CAUSE_OVERFLOW_SHIFT 14
#define CAUSE_ZERODIV_SHIFT 15
#define CAUSE_INVALID_SHIFT 16
#define CAUSE_UNIMPLEMENTED_SHIFT 17
#define CONTROL_FS_SHIFT 24
#define CONTROL_FS_MASK 1

enum RoundingMode:uint8_t{
    RN = 0,
    RZ = 1,
    RP = 2,
    RM = 3
};

class FPU
{
public:
    FPU(CP0& cp0);
    ~FPU();
    
    CP0 &cp0;

    bool COC = false;

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
        uint64_t regs[32]{};
    };

    union {
        struct {
            uint64_t FCR0;
            uint64_t empty;
            uint64_t empty1;
            uint64_t empty2;
            uint64_t empty3;
            uint64_t empty4;
            uint64_t empty5;
            uint64_t empty6;
            uint64_t empty7;
            uint64_t empty8;
            uint64_t empty9;
            uint64_t empty10;
            uint64_t empty11;
            uint64_t empty12;
            uint64_t empty13;
            uint64_t empty14;
            uint64_t empty15;
            uint64_t empty16;
            uint64_t empty17;
            uint64_t empty18;
            uint64_t empty19;
            uint64_t empty20;
            uint64_t empty21;
            uint64_t empty22;
            uint64_t empty23;
            uint64_t empty24;
            uint64_t empty25;
            uint64_t empty26;
            uint64_t empty27;
            uint64_t empty28;
            uint64_t empty29;
            uint64_t FCR31;
        };
        uint64_t control_regs[32]{0xa00,0};
    };

    uint64_t get_fpr(uint8_t fpr, uint8_t size);
    void set_fpr(uint64_t val, uint8_t fpr, uint8_t size);
    void set_cause(bool inexact, bool underflow, bool overflow, bool zerodiv, bool invalid, bool unimplemented);
    void clear_cause();
    void set_control(uint32_t val, uint8_t fcr);
    uint64_t get_control(uint8_t fcr);

    float flush_float(float value);
    double flush_double(double value);
    uint32_t rounding_mode();
    bool inexact_enabled();
    bool underflow_enabled();
    bool overflow_enabled();
    bool zerodiv_enabled();
    bool invalid_enabled();

private:

};
