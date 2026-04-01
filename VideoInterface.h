#ifndef VIDEOINTERFACE_H
#define VIDEOINTERFACE_H

#pragma once
#include"MemoryArea.h"
class VideoInterface : public MemoryArea
{
public:

    union{
        struct{
            uint32_t VI_CTRL;
            uint32_t VI_ORIGIN;
            uint32_t VI_WIDTH;
            uint32_t VI_V_INTR;
            uint32_t VI_V_CURRENT;
            uint32_t VI_BURST;
            uint32_t VI_V_TOTAL;
            uint32_t VI_H_TOTAL;
            uint32_t VI_H_TOTAL_LEAP;
            uint32_t VI_H_VIDEO;
            uint32_t VI_V_VIDEO;
            uint32_t VI_V_BURST;
            uint32_t VI_X_SCALE;
            uint32_t VI_Y_SCALE;
            uint32_t VI_TEST_ADDR;
            uint32_t VI_STAGED_DATA;
        };
        uint32_t regs[16];
    };

    void write_size(uint32_t address, uint64_t value, uint8_t size) override;
    uint64_t read_size(uint32_t address, uint8_t size) override;
private:

};

#endif