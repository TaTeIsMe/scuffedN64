#include "FPU.h"

FPU::FPU(CP0& cp0):cp0{cp0}
{
}


FPU::~FPU()
{

}

uint64_t FPU::get_fpr(uint8_t fpr, uint8_t size)
{
    uint8_t fr_bit = cp0.get_bits(cp0.status,STATUS_FR_MASK, STATUS_FR_SHIFT);
    uint8_t reg_id = fpr &~!fr_bit;
    if(size == 4){
        return (regs[reg_id] >> (32 * (fpr & 1) * !fr_bit)) & 0xFFFFFFFF;
    }else if(size == 8)
        return regs[reg_id];
    return 0;
}

void FPU::write_fpr(uint8_t fpr, uint64_t val, uint8_t size)
{
    uint8_t fr_bit = cp0.get_bits(cp0.status,STATUS_FR_MASK, STATUS_FR_SHIFT);
    uint8_t reg_id = fpr &~!fr_bit;
    if(size == 4){
        uint8_t shift = (32 * (fpr & 1) * !fr_bit);
        uint64_t mask = 0xFFFFFFFF;
        regs[reg_id] = (regs[reg_id] & ~(mask << shift)) | ((val & (mask)) << shift);
    }else if(size == 8)
        regs[reg_id] = val;
}
