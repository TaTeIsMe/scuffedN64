#pragma once

#include <cstdint>

#define CAUSE_EXCCODE_SHIFT 2
#define CAUSE_EXCCODE_MASK (0x1F<<CAUSE_EXCCODE_SHIFT)
#define CONTEXT_BADVPN2_SHIFT 4
#define CONTEXT_BADVPN2_MASK (0x7FFFF<<CAUSE_EXCCODE_SHIFT)
#define STATUS_EXL_SHIFT 1
#define STATUS_EXL_MASK (1<<STATUS_EXL_SHIFT)
#define ENTRYHI_VPN2_SHIFT 13
#define ENTRYHI_VPN2_MASK (0X7FFFF<<ENTRYHI_VPN2_SHIFT)
#define ENTRYHI_ASID_SHIFT 0
#define ENTRYHI_ASID_MASK (0xFF<<ENTRYHI_ASID_SHIFT)
class CP0
{
public:
    CP0();
    ~CP0();

    union {
        struct {
            uint32_t index;
            uint32_t random;
            uint32_t entryLo0;
            uint32_t entryLo1;
            uint32_t context;
            uint32_t pageMask;
            uint32_t wired;
            uint32_t r7;
            uint32_t badVAddr;
            uint32_t count;
            uint32_t entryHi;
            uint32_t compare;
            uint32_t status;
            uint32_t cause;
            uint32_t EPC;
            uint32_t PRId;
            uint32_t config;
            uint32_t LLAddr;
            uint32_t watchLo;
            uint32_t watchHi;
            uint32_t xcontext;
            uint32_t r21;
            uint32_t r22;
            uint32_t r23;
            uint32_t r24;
            uint32_t r25;
            uint32_t parityError;
            uint32_t cacheErr;
            uint32_t tagLo;
            uint32_t tagHi;
            uint32_t errorEPC;
            uint32_t r31;
        };
        uint32_t regs[32];
    };

    uint32_t TLB[4*32];

    struct TLB_Result{
        bool valid;
        bool miss;
        uint32_t asid;
        uint64_t p_addr;
    };

    TLB_Result tlb_translate(uint64_t v_addr);
    uint32_t set_bits(uint32_t reg, uint32_t mask, uint32_t value);
    uint32_t get_bits(uint32_t reg, uint32_t mask, int shift);

private:

};
