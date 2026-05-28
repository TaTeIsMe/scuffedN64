#include "Cartridge.h"

#include <iconv.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>

std::string eucjp_to_utf8(const std::vector<uint8_t>& buffer, size_t len)
{
    if (len == 0) return {};

    // iconv needs a mutable input buffer
    std::vector<char> input(buffer.begin(), buffer.begin() + len);
    char* inbuf = input.data();
    size_t inbytesleft = len;

    // Worst-case expansion: 1 EUC-JP byte → up to 3 UTF-8 bytes
    std::string output(len * 3, '\0');
    char* outbuf = output.data();
    size_t outbytesleft = output.size();

    iconv_t cd = iconv_open("UTF-8", "EUC-JP");
    if (cd == (iconv_t)-1) {
        // Try alternative names (rarely needed)
        cd = iconv_open("UTF-8", "EUCJP");
        if (cd == (iconv_t)-1)
            cd = iconv_open("UTF-8", "EUC-JP-MS"); // some systems
    }
    if (cd == (iconv_t)-1) {
        std::cerr << "[eucjp_to_utf8] iconv_open failed\n";
        return {};
    }

    size_t ret = iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
    iconv_close(cd);

    if (ret == (size_t)-1) {
        // Partial conversion, still usable
    }

    output.resize(output.size() - outbytesleft);
    return output;
}

Cartridge::Cartridge(std::vector<uint8_t> rom):mem(rom),is_viewer_buffer(0xFFFF, 0){}

uint64_t Cartridge::read_is_viewer(uint32_t address, uint8_t size){
    uint64_t result = 0;
    uint32_t offset = address - 0x03FF0000;
    for (int i = 0; i < size; i++) {
        result = (result << 8) | is_viewer_buffer[offset + i];
    }
    return result;
}

void Cartridge::write_is_viewer(uint32_t address, uint32_t value, uint8_t size){

    if (address == 0x3FF0014)
    {
        size_t len = value & 0xFFFFFFFF;

        if (len > is_viewer_buffer.size())
            len = is_viewer_buffer.size();

        std::string utf8_out =
            eucjp_to_utf8(std::vector<uint8_t>(is_viewer_buffer.begin() + 32, is_viewer_buffer.end()), len);

        std::cout.write(utf8_out.data(), utf8_out.size());
        std::cout.flush();

        std::fill(is_viewer_buffer.begin() + 32, is_viewer_buffer.end(), 0);
    }else if(address >= 0x03FF0000 && address < 0x03FFFFFF)
    {
        uint32_t offset = address - 0x03FF0000;

        for(uint8_t i = 0; i < size; i++)
        {
            is_viewer_buffer[offset + i] =
                (value >> ((size - 1 - i) * 8)) & 0xFF;
        }
    }
}

void Cartridge::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    if(address >= 0x03FF0000 && address < 0x03FFFFFF)
        write_is_viewer(address,value,size);
    
    if((address + size - 1) >= mem.size())return;
    
    //for (uint8_t i = 0; i < size; i++)
    //{
    //    mem[address + i] = (value >> ((size - 1 - i) * 8)) & 0xFF;
    //}
}

uint64_t Cartridge::read_size(uint32_t address, uint8_t size)
{
    if(address >= 0x03FF0000 && address < 0x03FFFFFF) 
        return read_is_viewer(address,size);

    if((address + size - 1) >= mem.size()) return 0;
    uint64_t result = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        result |= (uint64_t)mem[(address + i)] << ((size - 1 - i) * 8);
    }
    return result;
}
