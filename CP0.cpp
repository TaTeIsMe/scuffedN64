#include "CP0.h"

CP0::CP0()
{

}

CP0::~CP0()
{

}

CP0::TLB_Result CP0::tlb_translate(uint64_t v_addr)
{
    return TLB_Result{1,0,0,v_addr};
}

uint32_t CP0::set_bits(uint32_t reg, uint32_t mask, uint32_t value)
{
    return ((reg & ~mask) | (value & mask));
}

uint32_t CP0::get_bits(uint32_t reg, uint32_t mask, int shift)
{
    return (reg & mask) >> shift;
}