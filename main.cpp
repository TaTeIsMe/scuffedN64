#include <iostream>
#include "VR4300.h"
#include "Bus.h"

//finish cpu
//make scuffed periferals

int main(){
    std::cout << "hello world!\n";
    uint8_t memory[1024] = {
        0b00100000, 0b00100010, 0b00000000, 0b00001000,
        0b00100000, 0b00100010, 0b00000000, 0b00001000,
        0b00100000, 0b00100010, 0b00000000, 0b00001000,
        0b00100000, 0b00100010, 0b00000000, 0b00001000,
        0b00100000, 0b00100010, 0b00000000, 0b00001000,
        0b00100000, 0b00100010, 0b00000000, 0b00001000,
        0b00100000, 0b00100010, 0b00000000, 0b00001000,
        0b00100000, 0b00100010, 0b00000000, 0b00001000,
    };
    Bus bus = Bus(memory);
    VR4300 vr4300 = VR4300(bus);

    vr4300.on_clock();
    vr4300.on_clock();
    vr4300.on_clock();
    vr4300.on_clock();
    vr4300.on_clock();
    vr4300.on_clock();
    vr4300.on_clock();
    vr4300.on_clock();
    vr4300.on_clock();
    vr4300.on_clock();
    vr4300.on_clock();

    std::cout << vr4300.r2 << "\n";

}