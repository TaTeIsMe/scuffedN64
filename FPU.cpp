#include "FPU.h"
#include <cfenv>
#include <cmath>
#pragma STDC FENV_ACCESS ON

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

void FPU::set_cause(bool inexact, bool underflow, bool overflow, bool zerodiv, bool invalid, bool unimplemented)
{
    FCR31 |= (inexact << CAUSE_INEXACT_SHIFT);
    FCR31 |= (overflow << CAUSE_OVERFLOW_SHIFT);
    FCR31 |= (underflow << CAUSE_UNDERFLOW_SHIFT);
    FCR31 |= (invalid << CAUSE_INVALID_SHIFT);
    FCR31 |= (unimplemented << CAUSE_UNIMPLEMENTED_SHIFT);
}

void FPU::clear_cause(){
    FCR31 &= ~(0x3F << 12);
}

void FPU::set_control(uint8_t fcr, uint32_t val){
    if(fcr == 31){
        uint8_t prev_rm = FCR31 & 3;
        FCR31 = val & 0x183ffff;
        uint8_t new_rm = FCR31 & 3;
        if(prev_rm != new_rm) switch (new_rm){
            case RN: std::fesetround(FE_TONEAREST); break;
            case RZ: std::fesetround(FE_TOWARDZERO); break;
            case RP: std::fesetround(FE_UPWARD); break;
            case RM: std::fesetround(FE_DOWNWARD); break;
            default:break;
            }
    }
}

float FPU::flush_float(float value){
    uint8_t rm = FCR31 & 3;
    if(!std::signbit(value)){
        if(rm == RP) 
            return std::numeric_limits<float>::min();
        else return +0.f;
    }else{
        if(rm == RM) 
            return -std::numeric_limits<float>::min();
        else return -0.f;
    }
    return 0;
}

double FPU::flush_double(double value)
{
    uint8_t rm = FCR31 & 3;
    if(!std::signbit(value)){
        if(rm == RP) 
            return std::numeric_limits<double>::min();
        else return +0.;
    }else{
        if(rm == RM) 
            return -std::numeric_limits<double>::min();
        else return -0.;
    }
    return 0;
}

uint32_t FPU::rounding_mode()
{
    return FCR31 & 3;
}

bool FPU::inexact_enabled()
{
    if((FCR31 >> 7) & 1)return true;
    else return false;
}
bool FPU::underflow_enabled()
{
    if((FCR31 >> 8) & 1)return true;
    else return false;
    return false;
}
bool FPU::overflow_enabled()
{
    if((FCR31 >> 9) & 1)return true;
    else return false;
    return false;
}
bool FPU::zerodiv_enabled()
{
    if((FCR31 >> 10) & 1)return true;
    else return false;
    return false;
}
bool FPU::invalid_enabled()
{
    if((FCR31 >> 11) & 1)return true;
    else return false;
    return false;
}

