#include "CP0.h"
#include <cmath>

CP0::CP0()
{

}

CP0::~CP0()
{

}

CP0::Segment CP0::get_segment(uint64_t v_addr)
{
    uint32_t sign_extension = (v_addr >> 39) & 0x7FFFFF; //includes the sign bit

    if (sign_extension != 0 && sign_extension != 0x7FFFFF)
        return segment_lut[err];

    uint8_t region = (v_addr >> 62) & 0x3;

    //wtf even is this...
    switch (region) {
        case 0b00: return segment_lut[xkuseg];
        case 0b01: return segment_lut[xksseg];
        case 0b10: {
            uint32_t error_bits = (v_addr >> 32) & 0xFFFFFF;
            if(error_bits) return segment_lut[err];
            uint8_t space_id = (v_addr >> 59) & 0x7;
            switch (space_id){
                case 0: return segment_lut[xkphys0];
                case 1: return segment_lut[xkphys1];
                case 2: return segment_lut[xkphys2];
                case 3: return segment_lut[xkphys3];
                case 4: return segment_lut[xkphys4];
                case 5: return segment_lut[xkphys5];
                case 6: return segment_lut[xkphys6];
                case 7: return segment_lut[xkphys7];
            }

            return segment_lut[err];
        }
        case 0b11: {
            uint8_t space_id = (v_addr >> 29) & 0x7;;
            switch (space_id) {
                case 0b100: return segment_lut[ckseg0];
                case 0b101: return segment_lut[ckseg1];
                case 0b110: return segment_lut[cksseg];
                case 0b111: return segment_lut[ckseg3];
                default:    return segment_lut[xkseg];
            }
        }
    }

    return segment_lut[err];
}

bool CP0::in_user_mode()
{
    uint8_t KSU = get_bits(status,STATUS_KSU_MASK, STATUS_KSU_SHIFT);
    uint8_t EXL = get_bits(status,STATUS_EXL_MASK, STATUS_EXL_SHIFT);
    uint8_t ERL = get_bits(status,STATUS_ERL_MASK, STATUS_ERL_SHIFT);
    return (KSU == 0b10 && EXL == 0 && ERL == 0);
}

bool CP0::in_supervisor_mode()
{
    uint8_t KSU = get_bits(status,STATUS_KSU_MASK, STATUS_KSU_SHIFT);
    uint8_t EXL = get_bits(status,STATUS_EXL_MASK, STATUS_EXL_SHIFT);
    uint8_t ERL = get_bits(status,STATUS_ERL_MASK, STATUS_ERL_SHIFT);
    return (KSU == 0b10 && EXL == 0 && ERL == 0);
}

bool CP0::in_kernel_mode()
{
    uint8_t KSU = get_bits(status,STATUS_KSU_MASK, STATUS_KSU_SHIFT);
    uint8_t EXL = get_bits(status,STATUS_EXL_MASK, STATUS_EXL_SHIFT);
    uint8_t ERL = get_bits(status,STATUS_ERL_MASK, STATUS_ERL_SHIFT);
    return (KSU == 0b00 || EXL == 1 || ERL == 1);
}

// figure out the c (cache) bit in entrylo register
CP0::TLB_Result CP0::tlb_translate(uint64_t v_addr)
{
    for (int i = 0; i < 32; i++)
    {
        const uint64_t* tlb_entry = TLB[i];
        uint64_t mask = tlb_entry[0];
        uint64_t line2 = tlb_entry[1];
        if(~mask & line2 != ~mask & v_addr)continue;
        else{
            uint8_t asid = line2 & 0xFF;
            uint32_t offset = v_addr & (mask + 0xFFF);
            bool global = line2 & 1<<12;
            uint8_t PFN_line = 2+(v_addr >> 12) & 1;
            uint32_t PFN = tlb_entry[PFN_line] << 6;
            uint8_t cache = (tlb_entry[PFN_line] >> 3) & 0x7;
            bool dirty = (tlb_entry[PFN_line] >> 2) & 1;
            bool valid = (tlb_entry[PFN_line] >> 1) & 1;
            uint32_t p_addr = PFN + offset;
            return {0, dirty, valid, global, cache, asid, p_addr};
        }
    }
    
    return TLB_Result{1};
}

uint32_t CP0::set_bits(uint32_t reg, uint32_t mask, uint32_t value)
{
    return ((reg & ~mask) | (value & mask));
}

uint32_t CP0::get_bits(uint32_t reg, uint32_t mask, int shift)
{
    return (reg & mask) >> shift;
}