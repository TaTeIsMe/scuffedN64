#include "CP0.h"
#include <cmath>

CP0::CP0()
{

}

CP0::~CP0()
{

}


const CP0::Segment& CP0::get_segment(uint64_t v_addr)
{

    //if(!xmode){
    //    uint64_t sign_extension = (v_addr >> 31); //includes the sign bit
    //
    //    if (sign_extension != 0 && sign_extension != 0x1FFFFFFFFULL)
    //        return segment_lut[err];
    //}else{
    //    uint32_t sign_extension = (v_addr >> 39) & 0x7FFFFF; //includes the sign bit
    //
    //    if (sign_extension != 0 && sign_extension != 0x7FFFFF)
    //        return segment_lut[err];
    //}


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

void CP0::stash_status()
{
    IM = get_bits(status,STATUS_IM_MASK,STATUS_IM_SHIFT);
    IE = get_bits(status,STATUS_IE_MASK,STATUS_IE_SHIFT);
    EXL = get_bits(status,STATUS_EXL_MASK, STATUS_EXL_SHIFT);
    ERL = get_bits(status,STATUS_ERL_MASK, STATUS_ERL_SHIFT);
    if(in_kernel_mode())mode = Mode::KERNEL;
    else if(in_user_mode())mode=Mode::USER;
    else if(in_supervisor_mode())mode=Mode::SUPERVISOR;
    xmode = is_xmode();
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
    return (KSU == 0b01 && EXL == 0 && ERL == 0);
}

bool CP0::in_kernel_mode()
{
    uint8_t KSU = get_bits(status,STATUS_KSU_MASK, STATUS_KSU_SHIFT);
    uint8_t EXL = get_bits(status,STATUS_EXL_MASK, STATUS_EXL_SHIFT);
    uint8_t ERL = get_bits(status,STATUS_ERL_MASK, STATUS_ERL_SHIFT);
    return (KSU == 0b00 || EXL == 1 || ERL == 1);
}

bool CP0::is_xmode(){
    uint8_t UX = get_bits(status,STATUS_UX_MASK, STATUS_UX_SHIFT);
    uint8_t SX = get_bits(status,STATUS_SX_MASK, STATUS_SX_SHIFT);
    uint8_t KX = get_bits(status,STATUS_KX_MASK, STATUS_KX_SHIFT);
    return (mode == Mode::USER && UX)|| (mode == Mode::SUPERVISOR  && SX) || (mode == Mode::KERNEL && KX);
}

void CP0::set_reg(uint64_t val, uint8_t dest_reg)
{
    regs[dest_reg] = (regs[dest_reg] & ~write_masks[dest_reg]) | (val & write_masks[dest_reg]);
    if(dest_reg == 11)
        cause = set_bits(cause, CAUSE_IP_TIMER_MASK, 0 << CAUSE_IP_TIMER_SHIFT);
    if(dest_reg == 6)
        random = (wired > 31)?63:31;
    if(dest_reg == 12)
        stash_status();
}

uint64_t CP0::get_reg(uint8_t reg) const
{
    return regs[reg];
}

// figure out the c (cache) bit in entrylo register
CP0::TLB_Result CP0::tlb_translate(uint64_t v_addr)
{
    for (int i = 0; i < 32; i++)
    {
        const uint64_t* tlb_entry = TLB[i];
        uint64_t page_mask = tlb_entry[0];
        uint64_t VPN2 = tlb_entry[1] & 0xFFFFFFFE000;
        bool global = tlb_entry[1] & 1<<12;
        uint8_t asid = tlb_entry[1] & 0xFF;
        uint8_t current_asid = entryHi & 0xFF;
        if(!global && asid != current_asid)continue;
        if((~page_mask & VPN2) != (~page_mask & v_addr & 0xFFFFFFFE000))continue;
        else{

            uint32_t offset = v_addr & ((page_mask >> 1) | 0xFFF);

            uint64_t evenOddBit = (page_mask + 0x2000) >> 1;
            bool odd = v_addr & evenOddBit;
            uint8_t PFN_line = odd ? 3 : 2;
            uint32_t PFN =((tlb_entry[PFN_line] & 0x03FFFFC0) << 6) & ~(page_mask >> 1);
            uint32_t p_addr = PFN | offset;

            uint8_t cache = (tlb_entry[PFN_line] >> 3) & 0x7;
            bool dirty = (tlb_entry[PFN_line] >> 2) & 1;
            bool valid = (tlb_entry[PFN_line] >> 1) & 1;

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