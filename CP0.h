#pragma once

#include <cstdint>

#define RESET_EXCEPTION_VEC_64 0xFFFFFFFFBFC00000
#define EXCEPTION_VEC_64 0xFFFFFFFF80000000
#define BOOTSTRAP_EXCEPTION_VEC_64 0xFFFFFFFFBFC00200
#define CAUSE_EXCCODE_SHIFT 2
#define CAUSE_EXCCODE_MASK (0x1F<<CAUSE_EXCCODE_SHIFT)
#define CAUSE_BD_SHIFT 31
#define CAUSE_BD_MASK (1<<CAUSE_BD_SHIFT)
#define CONTEXT_BADVPN2_SHIFT 4
#define CONTEXT_BADVPN2_MASK (0x7FFFF<<CONTEXT_BADVPN2_SHIFT)
#define XCONTEXT_BADVPN2_SHIFT 4
#define XCONTEXT_BADVPN2_MASK (0x7FFFFFF<<XCONTEXT_BADVPN2_SHIFT)
#define STATUS_EXL_SHIFT 1
#define STATUS_EXL_MASK (1<<STATUS_EXL_SHIFT)
#define STATUS_BEV_SHIFT 22
#define STATUS_BEV_MASK (1<<STATUS_BEV_SHIFT)
#define STATUS_KSU_SHIFT 3
#define STATUS_KSU_MASK (0x3<<STATUS_KSU_SHIFT)
#define STATUS_ERL_SHIFT 2
#define STATUS_ERL_MASK (1<<STATUS_ERL_SHIFT)
#define STATUS_UX_SHIFT 5
#define STATUS_UX_MASK (1<<STATUS_UX_SHIFT)
#define STATUS_SX_SHIFT 6
#define STATUS_SX_MASK (1<<STATUS_SX_SHIFT)
#define STATUS_KX_SHIFT 7
#define STATUS_KX_MASK (1<<STATUS_KX_SHIFT)
#define STATUS_FR_SHIFT 26
#define STATUS_FR_MASK (1 << STATUS_FR_SHIFT)
#define ENTRYHI_VPN2_SHIFT 13
#define ENTRYHI_VPN2_MASK (0X7FFFF<<ENTRYHI_VPN2_SHIFT)
#define ENTRYHI_ASID_SHIFT 0
#define ENTRYHI_ASID_MASK (0xFF<<ENTRYHI_ASID_SHIFT)
#define WATCHLO_W_SHIFT 0
#define WATCHLO_W_MASK (1<<WATCHLO_W_SHIFT)
#define WATCHLO_R_SHIFT 1
#define WATCHLO_R_MASK (1<<WATCHLO_R_SHIFT)
#define TAGLO_PTAGLO_SHIFT 8
#define TAGLO_PTAGLO_MASK (0xFFFFF << TAGLO_PTAGLO_SHIFT)
#define TAGLO_PSTATE_SHIFT 6
#define TAGLO_PSTATE_MASK (0x3 << TAGLO_PSTATE_SHIFT)

enum ExceptionCode {
    Int = 0,
    Mod = 1,
    TLBL = 2,
    TLBS = 3,
    AdEL = 4,
    AdES = 5,
    IBE = 6,
    DBE = 7,
    Sys = 8,
    Bp = 9,
    RI = 10,
    CpU = 11,
    Ov = 12,
    Tr = 13,
    FPE = 15,
    WATCH = 23
};

enum SegmentID{
    err = 0,
    ckseg3 = 1,
    cksseg = 2,
    ckseg1 = 3,
    ckseg0 = 4,
    xkseg = 5,
    xkphys0 = 6,
    xkphys1 = 7, 
    xkphys2 = 8, 
    xkphys3 = 9, 
    xkphys4 = 10, 
    xkphys5 = 11, 
    xkphys6 = 12,
    xkphys7 = 13, 
    xksseg = 14,
    xkuseg = 15
};

class CP0
{
public:
    CP0();
    ~CP0();

    union {
        struct {
            uint64_t index;
            uint64_t random;
            uint64_t entryLo0;
            uint64_t entryLo1;
            uint64_t context;
            uint64_t pageMask;
            uint64_t wired;
            uint64_t r7;
            uint64_t badVAddr;
            uint64_t count;
            uint64_t entryHi;
            uint64_t compare;
            uint64_t status;
            uint64_t cause;
            uint64_t EPC;
            uint64_t PRId;
            uint64_t config;
            uint64_t LLAddr;
            uint64_t watchLo;
            uint64_t watchHi;
            uint64_t xcontext;
            uint64_t r21;
            uint64_t r22;
            uint64_t r23;
            uint64_t r24;
            uint64_t r25;
            uint64_t parityError;
            uint64_t cacheErr;
            uint64_t tagLo;
            uint64_t tagHi;
            uint64_t errorEPC;
            uint64_t r31;
        };
        uint64_t regs[32];
    };

    struct Segment{
        bool kernel_accesible;
        bool supervisor_accesible;
        bool user_accesible;
        bool tlb_mapped;
        bool cacheable;
        uint64_t translation_offset;
    };

    Segment segment_lut[16]={
        {0,0,0,0,0,0}, //err
        {1,0,0,1,1,0}, //ckseg3
        {1,1,0,1,1,0}, //cksseg
        {1,0,0,0,0,0xFFFFFFFFA0000000}, //ckseg1
        {1,0,0,0,1,0xFFFFFFFF80000000}, //ckseg0
        {1,0,0,1,1,0}, //xkseg
        {1,0,0,0,1,0xB800000000000000}, //xkphys0
        {1,0,0,0,1,0xB000000000000000}, //xkphys1
        {1,0,0,0,1,0xA800000000000000}, //xkphys2
        {1,0,0,0,1,0xA000000000000000}, //xkphys3
        {1,0,0,0,1,0x9800000000000000}, //xkphys4
        {1,0,0,0,0,0x9000000000000000}, //xkphys5
        {1,0,0,0,1,0x8800000000000000}, //xkphys6
        {1,0,0,0,1,0x8000000000000000}, //xkphys7
        {1,1,0,1,1,0}, //xksseg
        {1,1,1,1,1,0} //xkuseg
    };

    Segment get_segment(uint64_t v_addr);
    
    uint64_t TLB[32][4];

    struct TLB_Result{
        bool miss;
        bool dirty;
        bool valid;
        bool global;
        uint8_t cache;
        uint8_t asid;
        uint32_t p_addr;
    };

    bool in_user_mode();
    bool in_supervisor_mode();
    bool in_kernel_mode();
    bool is_xmode();
    TLB_Result tlb_translate(uint64_t v_addr);
    uint32_t set_bits(uint32_t reg, uint32_t mask, uint32_t value);
    uint32_t get_bits(uint32_t reg, uint32_t mask, int shift);

private:

};
