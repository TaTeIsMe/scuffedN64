#include "RSP.h"
#include "RCP.h"
#include <stack>
#include <Eigen/Dense>
#include "GFX.h"
#include "Vertex.h"
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdint>
#include <iostream>

// Helper to compile shaders for the full-screen quad
static GLuint createFullScreenShaderProgram() {
    const char* vertexShaderSource = R"(
        #version 330 core
        out vec2 TexCoord;
        void main() {
            float x = -1.0 + float((gl_VertexID & 1) << 2);
            float y = -1.0 + float((gl_VertexID & 2) << 1);
            TexCoord = vec2((x + 1.0) * 0.5, (y + 1.0) * 0.5);
            gl_Position = vec4(x, y, 0.0, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        uniform sampler2D screenTexture;
        void main() {
            FragColor = texture(screenTexture, TexCoord);
        }
    )";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, NULL);
    glCompileShader(fs);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

/**
 * Creates a single persistent GLFW window and updates its texture content per call.
 */
void displayTextureInWindow(const std::vector< uint8_t>& data, int width, int height, int channels) {
    if (!data.size() || width <= 0 || height <= 0) return;

    // Persistent state across function calls
    static GLFWwindow* window = nullptr;
    static GLuint shaderProgram = 0;
    static GLuint dummyVAO = 0;
    static GLuint textureID = 0;
    static int currentWidth = 0;
    static int currentHeight = 0;

    // 1. One-time initialization for GLFW and Window Creation
    if (!window) {
        // Assume main game already initialized GLFW; do not call glfwTerminate() on failure here!
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width * 10, height * 10, "Texture Viewer", NULL, NULL);
        if (!window) {
            std::cerr << "Failed to create debug GLFW window" << std::endl;
            return;
        }

        // Temporarily set context to load glad and gen resources
        GLFWwindow* prev = glfwGetCurrentContext();
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD in debug window" << std::endl;
            glfwDestroyWindow(window);
            window = nullptr;
            glfwMakeContextCurrent(prev);
            return;
        }

        glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
            glViewport(0, 0, w, h);
        });

        shaderProgram = createFullScreenShaderProgram();
        glGenVertexArrays(1, &dummyVAO);
        glGenTextures(1, &textureID);

        currentWidth = width * 10;
        currentHeight = height * 10;
        glViewport(0, 0, width * 10, height * 10);

        // Restore context back to game
        glfwMakeContextCurrent(prev);
    }

    // Save the original game context
    GLFWwindow* previousContext = glfwGetCurrentContext();

    // Make texture window active
    glfwMakeContextCurrent(window);

    // Process OS events
    glfwPollEvents();
    if (glfwWindowShouldClose(window)) {
        // ALWAYS restore game context before returning!
        glfwMakeContextCurrent(previousContext);
        return;
    }

    // Resize window dynamically if texture dimensions changed
    if (width != currentWidth || height != currentHeight) {
        glfwSetWindowSize(window, width * 10, height * 10);
        glViewport(0, 0, width * 10, height * 10);
        currentWidth = width * 10;
        currentHeight = height * 10;
    }

    // 2. Determine texture pixel format
    GLenum format = GL_RGB;
    GLenum internalFormat = GL_RGB8;
    
    if (channels == 1) {
        format = GL_RED;
        internalFormat = GL_R8;
    } else if (channels == 3) {
        format = GL_RGB;
        internalFormat = GL_RGB8;
    } else if (channels == 4) {
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
    }

    // 3. Update Texture Data
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // 4. Render Frame
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(dummyVAO);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);

    // ALWAYS restore main game context before exiting function
    glfwMakeContextCurrent(previousContext);
}

std::vector<uint8_t> decode_tex_temp(uint32_t phys_addr, uint8_t fmt, uint8_t siz, uint8_t pal, int w, int h ,uint8_t* mem, uint16_t* tlut_buffer) {
        std::vector<uint8_t> rgba(w * h * 4, 0);

        auto get_tlut_color = [&](uint8_t index, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) {
            uint16_t raw = tlut_buffer[index];
            r = ((raw >> 11) & 0x1F) * 255 / 31;
            g = ((raw >> 6)  & 0x1F) * 255 / 31;
            b = ((raw >> 1)  & 0x1F) * 255 / 31;
            a = (raw & 1) ? 255 : 0;
        };

        int total_pixels = w * h;

        for (int i = 0; i < total_pixels; i++) {
            uint8_t r = 255, g = 255, b = 255, a = 255;

            switch (fmt) {
            case 0: // RGBA
                if (siz == 2) { // RGBA16 (5-5-5-1)
                    uint32_t addr = phys_addr + (i * 2);
                        uint16_t pixel = (mem[addr] << 8) | mem[addr + 1];
                        r = ((pixel >> 11) & 0x1F) * 255 / 31;
                        g = ((pixel >> 6)  & 0x1F) * 255 / 31;
                        b = ((pixel >> 1)  & 0x1F) * 255 / 31;
                        a = (pixel & 1) ? 255 : 0;
                } else if (siz == 3) { // RGBA32 (8-8-8-8)
                    uint32_t addr = phys_addr + (i * 4);
                        r = mem[addr + 0];
                        g = mem[addr + 1];
                        b = mem[addr + 2];
                        a = mem[addr + 3];
                }
                break;

            case 1: // YUV (YUV16 4:2:2) - Full Range BT.601 Conversion
                if (siz == 2) {
                    uint32_t pair_index = i / 2;
                    uint32_t addr = phys_addr + (pair_index * 4);
                        uint8_t y0 = mem[addr + 0];
                        uint8_t u  = mem[addr + 1];
                        uint8_t y1 = mem[addr + 2];
                        uint8_t v  = mem[addr + 3];

                        uint8_t y = (i % 2 == 0) ? y0 : y1;
                        int u1 = (int)u - 128;
                        int v1 = (int)v - 128;

                        int rf = (int)(y + 1.402f * v1);
                        int gf = (int)(y - 0.344136f * u1 - 0.714136f * v1);
                        int bf = (int)(y + 1.772f * u1);

                        r = std::clamp(rf, 0, 255);
                        g = std::clamp(gf, 0, 255);
                        b = std::clamp(bf, 0, 255);
                        a = 255;
                }
                break;

            case 2: // CI (Color Indexed)
                if (siz == 0) { // CI4 (4-bit sub-palette)
                    uint32_t addr = phys_addr + (i / 2);
                        uint8_t byte = mem[addr];
                        uint8_t idx = (i % 2 == 0) ? (byte >> 4) : (byte & 0x0F);
                        uint8_t tlut_idx = (pal << 4) | idx;
                        get_tlut_color(tlut_idx, r, g, b, a);
                } else if (siz == 1) { // CI8
                    uint32_t addr = phys_addr + i;
                        uint8_t idx = mem[addr];
                        get_tlut_color(idx, r, g, b, a);
                }
                break;

            case 3: // IA (Intensity + Alpha)
                if (siz == 0) { // IA4 (3 bits I, 1 bit A)
                    uint32_t addr = phys_addr + (i / 2);
                        uint8_t byte = mem[addr];
                        uint8_t nibble = (i % 2 == 0) ? (byte >> 4) : (byte & 0x0F);
                        uint8_t intensity = ((nibble >> 1) & 0x07) * 255 / 7;
                        r = g = b = intensity;
                        a = (nibble & 1) ? 255 : 0;
                } else if (siz == 1) { // IA8 (4 bits I, 4 bits A)
                    uint32_t addr = phys_addr + i;
                        uint8_t byte = mem[addr];
                        r = g = b = ((byte >> 4) & 0x0F) * 255 / 15;
                        a = (byte & 0x0F) * 255 / 15;
                } else if (siz == 2) { // IA16 (8 bits I, 8 bits A)
                    uint32_t addr = phys_addr + (i * 2);
                        r = g = b = mem[addr];
                        a = mem[addr + 1];
                }
                break;

            case 4: // I (Intensity)
                if (siz == 0) { // I4
                    uint32_t addr = phys_addr + (i / 2);
                        uint8_t byte = mem[addr];
                        uint8_t nibble = (i % 2 == 0) ? (byte >> 4) : (byte & 0x0F);
                        r = g = b = a = nibble * 255 / 15;
                } else if (siz == 1) { // I8
                    uint32_t addr = phys_addr + i;
                        r = g = b = a = mem[addr];
                }
                break;

            default:
                break;
            }

            rgba[i * 4 + 0] = r;
            rgba[i * 4 + 1] = g;
            rgba[i * 4 + 2] = b;
            rgba[i * 4 + 3] = a;
        }

        return rgba;
}

RSP::RSPRegs::RSPRegs(RSP &rsp):rsp(rsp){}

void inline set_clear_reg_bit(uint8_t set, uint8_t clear,uint32_t& reg, uint32_t bit){
    if(set && clear)return;
    if(set) reg |= 1 << bit;
    if(clear) reg &= ~(1 << bit);
};

void RSP::RSPRegs::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    address &= 0x1F; // address mirroring

    uint32_t reg_id = address >> 2;

    switch (address)
    {
    case 0:
        rsp.pending_dma[reg_id] = value & 0x1FF8;
        break;
    case 0x4:
        rsp.pending_dma[reg_id] = value & 0xFFFFF8;
        break;
    case 0x8:
    case 0xC:
        if(!SP_DMA_FULL){
            rsp.pending_dma[2] = value & ~0x00700007;
            rsp.pending_dma[3] = value & ~0x00700007;
            rsp.pending_dma_direction = (reg_id == 3);
            if(SP_DMA_BUSY){
                SP_DMA_FULL = 1; 
                SP_STATUS |= (1 << 3); 
            }
        }
        if(!SP_DMA_BUSY){
            regs[0] = rsp.pending_dma[0];
            regs[1] = rsp.pending_dma[1];
            regs[2] = rsp.pending_dma[2];
            regs[3] = rsp.pending_dma[3];
            rsp.dma_direction = rsp.pending_dma_direction;
            SP_DMA_BUSY = 1;
            SP_STATUS |= (1 << 2);
            rsp.start_dma();
        }
        break;
    case 0x10:{
        uint8_t SET_SIG7 = (value >> 24) & 1;
        uint8_t SET_SIG6 = (value >> 22) & 1;
        uint8_t SET_SIG5 = (value >> 20) & 1;
        uint8_t SET_SIG4 = (value >> 18) & 1;
        uint8_t SET_SIG3 = (value >> 16) & 1;
        uint8_t SET_SIG2 = (value >> 14) & 1;
        uint8_t SET_SIG1 = (value >> 12) & 1;
        uint8_t SET_SIG0 = (value >> 10) & 1;
        uint8_t CLR_SIG7 = (value >> 23) & 1;
        uint8_t CLR_SIG6 = (value >> 21) & 1;
        uint8_t CLR_SIG5 = (value >> 19) & 1;
        uint8_t CLR_SIG4 = (value >> 17) & 1;
        uint8_t CLR_SIG3 = (value >> 15) & 1;
        uint8_t CLR_SIG2 = (value >> 13) & 1;
        uint8_t CLR_SIG1 = (value >> 11) & 1;
        uint8_t CLR_SIG0 = (value >> 9) & 1;
        uint8_t SET_INTBREAK = (value >> 8) & 1;
        uint8_t CLR_INTBREAK = (value >> 7) & 1;
        uint8_t SET_SSTEP = (value >> 6) & 1;
        uint8_t CLR_SSTEP = (value >> 5) & 1;
        uint8_t SET_INTR = (value >> 4) & 1;
        uint8_t CLR_INTR = (value >> 3) & 1;
        uint8_t CLR_BROKE = (value >> 2) & 1;
        uint8_t SET_HALT = (value >> 1) & 1;
        uint8_t CLR_HALT = (value >> 0) & 1;

        if(CLR_HALT)rsp.start_task();

        set_clear_reg_bit(SET_SIG7, CLR_SIG7, SP_STATUS, 14);
        set_clear_reg_bit(SET_SIG6, CLR_SIG6, SP_STATUS, 13);
        set_clear_reg_bit(SET_SIG5, CLR_SIG5, SP_STATUS, 12);
        set_clear_reg_bit(SET_SIG4, CLR_SIG4, SP_STATUS, 11);
        set_clear_reg_bit(SET_SIG3, CLR_SIG3, SP_STATUS, 10);
        set_clear_reg_bit(SET_SIG2, CLR_SIG2, SP_STATUS, 9);
        set_clear_reg_bit(SET_SIG1, CLR_SIG1, SP_STATUS, 8);
        set_clear_reg_bit(SET_SIG0, CLR_SIG0, SP_STATUS, 7);
        set_clear_reg_bit(SET_INTBREAK, CLR_INTBREAK, SP_STATUS, 6);
        set_clear_reg_bit(SET_SSTEP, CLR_SSTEP, SP_STATUS, 5);
        if(SET_INTR)rsp.rcp.mi.route_interrupt(InterruptSource::SP);
        if(CLR_INTR)rsp.rcp.mi.clear_interrupt(InterruptSource::SP);
        set_clear_reg_bit(0, CLR_BROKE, SP_STATUS, 1);
        set_clear_reg_bit(SET_HALT, CLR_HALT, SP_STATUS, 0);
        break;
    }
    case 0x14:
    case 0x18:
        break;
    case 0x1C:
        SP_SEMAPHORE = value & 1;
        break;
    default:
        break;
    }

}

uint64_t RSP::RSPRegs::read_size(uint32_t address, uint8_t size)
{
    address = address & 0x1F; //mirroring
    uint32_t reg_id = address >> 2;
    return regs[reg_id];
}

void RSP::Imem::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    if((address + size - 1) >= mem.size())return;
    
    for (uint8_t i = 0; i < size; i++)
    {
        mem[address + i] = (value >> ((size - 1 - i) * 8)) & 0xFF;
    }
}

uint64_t RSP::Imem::read_size(uint32_t address, uint8_t size)
{
    if((address + size - 1) >= mem.size()) return 0;
    uint64_t result = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        result |= (uint64_t)mem[(address + i)] << ((size - 1 - i) * 8);
    }
    return result;
}

RSP::Dmem::Dmem():mem(0x1000){}
RSP::Imem::Imem():mem(0x1000){}

void RSP::Dmem::write_size(uint32_t address, uint64_t value, uint8_t size)
{
    if((address + size - 1) >= mem.size())return;
    
    for (uint8_t i = 0; i < size; i++)
    {
        mem[address + i] = (value >> ((size - 1 - i) * 8)) & 0xFF;
    }
}

uint64_t RSP::Dmem::read_size(uint32_t address, uint8_t size)
{
    if((address + size - 1) >= mem.size()) return 0;
    uint64_t result = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        result |= (uint64_t)mem[(address + i)] << ((size - 1 - i) * 8);
    }
    return result;
}

RSP::RSP(RCP& rcp, GFX& gfx, Rdram& rdram): rcp(rcp),gfx(gfx), regs(*this), rdram(rdram){}

void RSP::start_dma()
{
    uint32_t ram_start = regs.SP_DMA_RAMADDR & 0xFFFFF8;
    uint16_t mem_start = regs.SP_DMA_SPADDR & 0xFF8;
    len = (regs.SP_DMA_RDLEN & 0xFFF) | 7;
    start_len = len;
    count = ((regs.SP_DMA_RDLEN >> 12) & 0xFF) + 1;
    skip = (regs.SP_DMA_RDLEN >> 20) & 0xFF8;
    current_ram_addr = ram_start;
    current_mem_addr = mem_start;
    //timer = len * count / 8 * 3;
    rcp.eventq.enqueue(rcp.cycles + len * count / 8 * 3,EventType::SP_DMA_DONE);
}

void RSP::continue_dma()
{
   //timer-=16;
   //if(timer < 0)
   //finish_dma();
}

void RSP::finish_dma()
{

    bool mem_bank = regs.SP_DMA_SPADDR & 0x1000;
    //this might end up copying too much memory
    while(true){
        if(count > 0){
            if(len >= 0){
                len -= 8;
                if(dma_direction){
                    mem_bank?
                    rdram.write_size(current_ram_addr,imem.read_size(current_mem_addr,8),8):
                    rdram.write_size(current_ram_addr,dmem.read_size(current_mem_addr,8),8);
                }else{
                    mem_bank?
                    imem.write_size(current_mem_addr,rdram.read_size(current_ram_addr,8),8):
                    dmem.write_size(current_mem_addr,rdram.read_size(current_ram_addr,8),8);
                }
                current_ram_addr += 8;
                current_mem_addr = (current_mem_addr + 8) & 0xFFF;
                regs.SP_DMA_RDLEN = (skip << 20) | (count << 12) | (len & 0xFF8);
                regs.SP_DMA_WRLEN = (skip << 20) | (count << 12) | (len & 0xFF8);
                continue;
            }
            count--;
            len = start_len;
            current_ram_addr += skip;
            continue;
        }
        break;
    }

    if(regs.SP_DMA_FULL){
        dma_direction = pending_dma_direction;
        regs.regs[0] = pending_dma[0];
        regs.regs[1] = pending_dma[1];
        regs.regs[2] = pending_dma[2];
        regs.regs[3] = pending_dma[3];
        regs.SP_DMA_BUSY = 1;
        regs.SP_STATUS |= (1 << 2);
        start_dma();
        regs.SP_DMA_FULL = 0;
        regs.SP_STATUS &= ~(1 << 3);
    }else{
        regs.SP_DMA_BUSY = 0;
        regs.SP_STATUS &= ~(1 << 2);
    }
}

void RSP::start_task()
{
    task_in_progress = true;
    current_task_type = (RSPTaskType)dmem.mem[0xFC3];
    task_timer = 0; //this should break it
    rcp.eventq.enqueue(rcp.cycles + 2000, EventType::SP_TASK_DONE);
}

void RSP::continue_task()
{
    //task_timer+=16;
    //if(task_timer > 160000)
    //    finish_task();
}

inline Eigen::Matrix4f parse_mtx_from_mem(const std::vector<uint8_t>& mem, uint32_t address)
{
    Eigen::Matrix4f mtx;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {

            int16_t intpart = (int16_t)
                ((mem[address + ((i * 4 + j) * 2)] << 8) |
                mem[address + ((i * 4 + j) * 2)+ 1]);

            uint16_t fracpart = (uint16_t)
                ((mem[address + 32 + ((i * 4 + j) * 2)] << 8) | 
                mem[address + 32 + ((i * 4 + j) * 2)+ 1]);
            
            uint32_t raw_combined = ((uint32_t)intpart << 16) | fracpart;
            int32_t combined = (int32_t)raw_combined;
            mtx(j,i) = (float)combined / 65536.0f;
        }
    }
    return mtx;
}


void RSP::process_gfx_task(OSTask task){

    uint32_t segments[16]{};
    std::stack<uint32_t> address_stack;
    std::stack<Eigen::Matrix4f> modelview_mtx_stack;
    modelview_mtx_stack.push(Eigen::Matrix4f::Identity());
    Eigen::Matrix4f projection_mtx(Eigen::Matrix4f::Identity());
    Eigen::Matrix4f modelview_projection_mtx;
    Vertex vertex_buffer[32];

    uint32_t tex_load_ptr = 0;
    uint8_t tex_load_fmt = 0;
    uint8_t tex_load_siz = 0;
    uint16_t tex_load_width = 0;

    uint32_t geometry_mode = 0;
    
    auto translate_address = [&](uint32_t address){
        if(address >> 24 == 0x80)return address & 0xFFFFFF;
        uint32_t seg_id = (address >> 24);
        uint32_t offset = address & 0xFFFFFF;
        uint32_t segment = segments[seg_id];
        return (segment + offset) & 0xFFFFFF;
    };
    
    uint32_t instr_ptr = translate_address(task.data_ptr);
    
    while(true){
        uint64_t instr = rcp.rdram.read_size(instr_ptr, 8);

        switch ((instr >> 56) & 0xFF)
        {
        case G_NOOP: // G_NOOP
            break;
        case G_VTX: // G_VTX
        {
            uint8_t numv = (instr >> 44) & 0xFF;
            uint8_t buf_id = (((instr >> 32) & 0xFF) >> 1) - numv;
            uint32_t vaddr = translate_address(instr & 0xFFFFFFFF);
            for (uint16_t i = 0; i < numv; i++)
            {
                int16_t x = rcp.rdram.read_size(vaddr,2);
                int16_t y = rcp.rdram.read_size(vaddr + 2,2);
                int16_t z = rcp.rdram.read_size(vaddr + 4,2);
                int16_t u = rcp.rdram.read_size(vaddr + 8,2);
                int16_t v = rcp.rdram.read_size(vaddr + 10,2);

                float s0 = gfx.tiles[gfx.active_tile].uls / 4.0f;
                float t0 = gfx.tiles[gfx.active_tile].ult / 4.0f;
                float s1 = gfx.tiles[gfx.active_tile].lrs / 4.0f;
                float t1 = gfx.tiles[gfx.active_tile].lrt / 4.0f;

                float width = (int)(s1 - s0 + 1);
                float height = (int)(t1 - t0 + 1);

                vertex_buffer[buf_id + i].vector = modelview_projection_mtx * Eigen::Vector4f(x,y,z,1) ;
                vertex_buffer[buf_id + i].u = (float)u * gfx.s / 32. / width;
                vertex_buffer[buf_id + i].v = (float)v * gfx.t / 32. / height;

                if(((geometry_mode >> 17) & 1)){

                    // Read signed 8-bit normal values from the vertex data scuffed lighting
                    int8_t nx = (int8_t)rcp.rdram.read_size(vaddr + 12, 1);
                    int8_t ny = (int8_t)rcp.rdram.read_size(vaddr + 13, 1);
                    int8_t nz = (int8_t)rcp.rdram.read_size(vaddr + 14, 1);
                    
                    // Convert normals from raw bytes to normalized floats (-1.0 to 1.0)
                    Eigen::Vector3f normal(nx / 127.0f, ny / 127.0f, nz / 127.0f);
                    
                    // Transform the normal vector using your modelview matrix
                    Eigen::Matrix3f normalMatrix =
                        modelview_mtx_stack.top().block<3,3>(0,0)
                            .inverse()
                            .transpose();
                    Eigen::Vector3f transformedNormal = (normalMatrix * normal).normalized();
                    
                    // Simple directional light math: Ambient + Max(0, Normal dot LightDir) * LightColor
                    Eigen::Vector3f lightDir(0.5f, 1.0f, 0.3f);
                    lightDir.normalize();
                    
                    float diff = std::max(transformedNormal.dot(lightDir), 0.0f);
                    
                    Eigen::Vector3f ambient(0.2f, 0.2f, 0.2f);
                    Eigen::Vector3f diffuseColor(1.0f, 1.0f, 1.0f);
                    Eigen::Vector3f finalColor = ambient + (diff * diffuseColor);
                    
                    // Store this inside your vertex buffer to send to OpenGL
                    vertex_buffer[buf_id + i].r = finalColor.x();
                    vertex_buffer[buf_id + i].g = finalColor.y();
                    vertex_buffer[buf_id + i].b = finalColor.z();
                    vertex_buffer[buf_id + i].a = 1.0f; // Alpha
                }else{
                    vertex_buffer[buf_id + i].r  = rcp.rdram.read_size(vaddr + 12,1) / 255.f;
                    vertex_buffer[buf_id + i].g  = rcp.rdram.read_size(vaddr + 13,1) / 255.f;
                    vertex_buffer[buf_id + i].b  = rcp.rdram.read_size(vaddr + 14,1) / 255.f;
                    vertex_buffer[buf_id + i].a  = rcp.rdram.read_size(vaddr + 15,1) / 255.f;
                }
                    
                vaddr += 16;
            }
            
            break;
        }
        case 0x02: // G_MODIFYVTX
            break;
        case 0x03: // G_CULLDL
            break;
        case 0x04: // G_BRANCH_Z
            break;
        case G_TRI1: // G_TRI1
        {
            uint8_t v0 = ((instr >> 48) & 0xFF) / 2;
            uint8_t v1 = ((instr >> 40) & 0xFF) / 2;
            uint8_t v2 = ((instr >> 32) & 0xFF) / 2;
            //gfx.vertices.insert(gfx.vertices.end(),{vertex_buffer[v0], vertex_buffer[v1], vertex_buffer[v2]});

            float s0 = gfx.tiles[gfx.active_tile].uls / 4.0f;
            float t0 = gfx.tiles[gfx.active_tile].ult / 4.0f;
            float s1 = gfx.tiles[gfx.active_tile].lrs / 4.0f;
            float t1 = gfx.tiles[gfx.active_tile].lrt / 4.0f;

            float width = (int)(s1 - s0 + 1);
            float height = (int)(t1 - t0 + 1);

            //displayTextureInWindow(
            //    decode_tex_temp(gfx.tiles[gfx.active_tile].tmem,
            //        gfx.tiles[gfx.active_tile].fmt,
            //        gfx.tiles[gfx.active_tile].siz,
            //        gfx.tiles[gfx.active_tile].palette,
            //        width,
            //        height,
            //        gfx.tmem,
            //        gfx.tlut_buffer),
            //    width,
            //    height,
            //    4
            //);
            GLuint tex = gfx.create_new_texture();

            gfx.draw_calls.push_back(DrawCall(vertex_buffer[v0], vertex_buffer[v1], vertex_buffer[v2], tex));
            
            break;
        }
        case 0x06: // G_TRI2
        {
            uint8_t v00 = ((instr >> 48) & 0xFF) / 2;
            uint8_t v01 = ((instr >> 40) & 0xFF) / 2;
            uint8_t v02 = ((instr >> 32) & 0xFF) / 2;
            //gfx.vertices.insert(gfx.vertices.end(),{vertex_buffer[v00], vertex_buffer[v01], vertex_buffer[v02]});
            GLuint tex = gfx.create_new_texture();
            gfx.draw_calls.push_back(DrawCall(vertex_buffer[v00], vertex_buffer[v01], vertex_buffer[v02], tex));

            uint8_t v10 = ((instr >> 16) & 0xFF) / 2;
            uint8_t v11 = ((instr >> 8) & 0xFF) / 2;
            uint8_t v12 = ((instr) & 0xFF) / 2;
            //gfx.vertices.insert(gfx.vertices.end(),{vertex_buffer[v10], vertex_buffer[v11], vertex_buffer[v12]});
            gfx.draw_calls.push_back(DrawCall(vertex_buffer[v10], vertex_buffer[v11], vertex_buffer[v12], tex));
            break;
        }
        case 0x07: // G_QUAD
            break;
        case 0xD3: // G_SPECIAL_3
            break;
        case 0xD4: // G_SPECIAL_2
            break;
        case 0xD5: // G_SPECIAL_1
            break;
        case 0xD6: // G_DMA_IO
            break;
        case G_TEXTURE: // G_TEXTURE
        {
            uint16_t raw_s = (instr >> 16) & 0xFFFF;
            uint16_t raw_t = instr & 0xFFFF;
            gfx.s = (float)raw_s / 65536.0f;
            gfx.t = (float)raw_t / 65536.0f;
            uint8_t tile = (instr >> 40) & 0x7;
            gfx.active_tile = tile;
            break;
        }
        case G_POPMTX: // G_POPMTX
        {
            uint16_t num = (instr & 0xFFFFFFFF) >> 6;
            for (int i = 0; i < num; i++)
                if(!modelview_mtx_stack.empty())
                    modelview_mtx_stack.pop();
            
            break;
        }
        case G_GEOMETRYMODE: // G_GEOMETRYMODE
        {
            uint32_t clear_bits = (instr >> 32) & 0x00FFFFFF;
            uint32_t set_bits = instr & 0xFFFFFFFF;
            geometry_mode = (geometry_mode & ~clear_bits) | set_bits;
            break;
        }
        case G_MTX: // G_MTX
        {
            uint32_t mtxaddr = translate_address(instr & 0xFFFFFFFF);
            Eigen::Matrix4f mtx = parse_mtx_from_mem(rdram.mem, mtxaddr);
            bool G_MTX_PUSH = !((instr >> 32) & 0x1);
            bool G_MTX_MUL = !((instr >> 32) & 0x2);
            bool G_MTX_PROJECTION = ((instr >> 32) & 0x4);

            Eigen::Matrix4f topmtx;

            if(G_MTX_PROJECTION)
                topmtx = projection_mtx;
            else if(!modelview_mtx_stack.empty())
                topmtx = modelview_mtx_stack.top();
            else
                topmtx = Eigen::Matrix4f::Identity();

            if(G_MTX_MUL)
                mtx = topmtx * mtx;
            if(!G_MTX_PROJECTION){
                if(G_MTX_PUSH)
                    modelview_mtx_stack.push(mtx);
                else if(!G_MTX_PUSH){
                    if(!modelview_mtx_stack.empty())
                        modelview_mtx_stack.top() = mtx;
                    else
                        modelview_mtx_stack.push(mtx);
                }
            }
            else if(G_MTX_PROJECTION){
                projection_mtx = mtx;
            }
            if(!modelview_mtx_stack.empty())
                modelview_projection_mtx = projection_mtx * modelview_mtx_stack.top();
            
            break;
        }
        case G_MOVEWORD: // G_MOVEWORD
        {
            uint8_t index = ((instr >> 48) & 0xFF);
            switch (index)
            {
            case G_MW_SEGMENT:
            {
                uint8_t segment_id = ((instr >> 32) & 0xFFFF) >> 2;
                segments[segment_id] = instr & 0xFFFFFFFF;
                break;
            }
            default:
                break;
            }
            break;
        }
        case 0xDC: // G_MOVEMEM
            break;
        case 0xDD: // G_LOAD_UCODE
            break;
        case G_DL: // G_DL
            if(((instr >> 48) & 0xFF) == 0) address_stack.push(instr_ptr);
            instr_ptr = translate_address(instr);
            continue;
            break;
        case G_ENDDL: // G_ENDDL
            if(address_stack.empty())
                return;

            instr_ptr = address_stack.top();
            address_stack.pop();
            break;
        case 0xE0: // G_SPNOP
            break;
        case 0xE1: // G_RDPHALF_1
            break;
        case 0xE2: // G_SETOTHERMODE_L
            break;
        case 0xE3: // G_SETOTHERMODE_H
            break;
        case 0xE4: // G_TEXRECT
            break;
        case 0xE5: // G_TEXRECTFLIP
            break;
        case 0xE6: // G_RDPLOADSYNC
            break;
        case 0xE7: // G_RDPPIPESYNC
            break;
        case 0xE8: // G_RDPTILESYNC
            break;
        case 0xE9: // G_RDPFULLSYNC
            break;
        case 0xEA: // G_SETKEYGB
            break;
        case 0xEB: // G_SETKEYR
            break;
        case 0xEC: // G_SETCONVERT
            break;
        case 0xED: // G_SETSCISSOR
            break;
        case 0xEE: // G_SETPRIMDEPTH
            break;
        case 0xEF: // G_RDPSETOTHERMODE
            break;
        case G_LOADTLUT: // G_LOADTLUT
        {
            uint32_t count = ((instr >> 12) & 0xFFF) + 1;
            uint32_t tlut_phys = translate_address(tex_load_ptr);
            for (uint32_t i = 0; i < 256 && i < count; i++) {
                uint32_t addr = tlut_phys + (i * 2);
                if (addr + 1 < rcp.rdram.mem.size()) {
                    gfx.tlut_buffer[i] = (rcp.rdram.mem[addr] << 8) | rcp.rdram.mem[addr + 1];
                }
            }
            break;
        }
        case 0xF1: // G_RDPHALF_2
            break;
        case G_SETTILESIZE: // G_SETTILESIZE
        {
            uint8_t tile = (instr >> 24) & 0xF;
            gfx.tiles[tile].uls = (instr >> 44) & 0xFFF;
            gfx.tiles[tile].ult = (instr >> 32) & 0xFFF;
            gfx.tiles[tile].lrs = (instr >> 12) & 0xFFF;
            gfx.tiles[tile].lrt =  instr        & 0xFFF;
        }
            break;
        case G_LOADBLOCK:
        {
            uint8_t tile = (instr >> 24) & 0x7;

            uint32_t texels = ((instr >> 12) & 0xFFF) + 1;

            Tile& t = gfx.tiles[tile];

            uint32_t src = translate_address(tex_load_ptr);

            uint32_t bytesPerTexel;
            switch (tex_load_siz)
            {
            case 0: bytesPerTexel = 0; break; // handled separately (4bpp)
            case 1: bytesPerTexel = 1; break;
            case 2: bytesPerTexel = 2; break;
            case 3: bytesPerTexel = 4; break;
            }

            uint32_t bytes =
                (tex_load_siz == 0)
                ? ((texels + 1) / 2)
                : texels * bytesPerTexel;

            memcpy(
                &gfx.tmem[t.tmem * 8],
                &rcp.rdram.mem[src],
                bytes);

            break;
        }
        case G_LOADTILE:
        {
            uint8_t tile = (instr >> 24) & 0x7;

            Tile& t = gfx.tiles[tile];

            uint32_t src = translate_address(tex_load_ptr);

            uint32_t width =
                (((instr >> 12) & 0xFFF) - ((instr >> 44) & 0xFFF)) / 4 + 1;

            uint32_t height =
                ((instr & 0xFFF) - ((instr >> 32) & 0xFFF)) / 4 + 1;

            uint32_t bytesPerPixel;

            switch (tex_load_siz)
            {
            case 0: bytesPerPixel = 0; break;
            case 1: bytesPerPixel = 1; break;
            case 2: bytesPerPixel = 2; break;
            case 3: bytesPerPixel = 4; break;
            }

            uint32_t dst = t.tmem * 8;

            if (tex_load_siz == 0)
            {
                uint32_t rowBytes = (width + 1) / 2;

                for (uint32_t y = 0; y < height; y++)
                {
                    memcpy(
                        &gfx.tmem[dst + y * t.line * 8],
                        &rcp.rdram.mem[src + y * rowBytes],
                        rowBytes);
                }
            }
            else
            {
                uint32_t rowBytes = width * bytesPerPixel;

                for (uint32_t y = 0; y < height; y++)
                {
                    memcpy(
                        &gfx.tmem[dst + y * t.line * 8],
                        &rcp.rdram.mem[src + y * rowBytes],
                        rowBytes);
                }
            }

            break;
        }
        case G_SETTILE: // G_SETTILE
        {
            uint8_t tile = (instr >> 24) & 0x7;
            gfx.tiles[tile].fmt = (instr >> 53) & 0x7;
            gfx.tiles[tile].siz = (instr >> 51) & 0x3;
            gfx.tiles[tile].line = (instr >> 41) & 0x1FF;
            gfx.tiles[tile].tmem = (instr >> 32) & 0x1FF;
            gfx.tiles[tile].palette = (instr >> 20) & 0x0F;
            gfx.tiles[tile].cmt = (instr >> 18) & 0x3;
            gfx.tiles[tile].maskt = (instr >> 14) & 0xF;
            gfx.tiles[tile].shiftt = (instr >> 10) & 0xF;
            gfx.tiles[tile].cms = (instr >> 8) & 0x3;
            gfx.tiles[tile].masks = (instr >> 4) & 0xF;
            gfx.tiles[tile].shifts = (instr >> 0) & 0xF;
            break;
        }
        case 0xF6: // G_FILLRECT
            break;
        case 0xF7: // G_SETFILLCOLOR
            break;
        case 0xF8: // G_SETFOGCOLOR
            break;
        case 0xF9: // G_SETBLENDCOLOR
            break;
        case 0xFA: // G_SETPRIMCOLOR
            break;
        case 0xFB: // G_SETENVCOLOR
            break;
        case 0xFC: // G_SETCOMBINE
            break;
        case G_SETTIMG: // G_SETTIMG
        {
            tex_load_ptr = instr & 0xFFFFFFFF;
            tex_load_fmt = (instr >> 53) & 0x7;
            tex_load_siz = (instr >> 51) & 0x3;
            tex_load_width = (instr >> 32) & 0xFFF;
            break;
        }
        case 0xFE: // G_SETZIMG
            break;
        case 0xFF: // G_SETCIMG
            break;
        default:
            break;
        }
    instr_ptr += 8;
    }
};

void RSP::finish_task()
{
    //also set PC in the future
    task_in_progress = false;
    regs.SP_STATUS |= 0x0203;

    OSTask new_task = OSTask::parse_from_mem(dmem.mem,0xFC0);

    if(regs.SP_STATUS & 0x40)
        rcp.mi.route_interrupt(InterruptSource::SP);
    if(current_task_type == RSPTaskType::GFXTASK){
        process_gfx_task(new_task);
        rcp.rsp.gfx.render_cycle();
        rcp.mi.route_interrupt(InterruptSource::DP);
    }
}

OSTask OSTask::parse_from_mem(std::vector<uint8_t> mem, uint32_t addr)
{
    OSTask new_task;
    for(size_t i = 0; i < sizeof(OSTask); i++)
    {
        reinterpret_cast<uint8_t*>(&new_task)[i] =
            mem[(addr + i) ^ 3];
    }

    return new_task;
}

