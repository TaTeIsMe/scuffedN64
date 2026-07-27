#include "GFX.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::string load_shader_source(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<uint8_t> decode_tex(uint32_t offset, uint32_t line, uint8_t fmt, uint8_t siz, uint8_t pal, int w, int h, uint8_t* mem, uint16_t* tlut_buffer) {
    std::vector<uint8_t> rgba(w * h * 4, 0);

    auto get_tlut_color = [&](uint8_t index, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) {
        uint16_t raw = tlut_buffer[index];
        r = ((raw >> 11) & 0x1F) * 255 / 31;
        g = ((raw >> 6)  & 0x1F) * 255 / 31;
        b = ((raw >> 1)  & 0x1F) * 255 / 31;
        a = (raw & 1) ? 255 : 0;
    };

    // Stride per row in bytes
    uint32_t row_stride = line * 8;

    for (int y = 0; y < h; y++) {
        uint32_t row_addr = offset + (y * row_stride);

        for (int x = 0; x < w; x++) {
            int pixel_index = y * w + x;
            uint8_t r = 255, g = 255, b = 255, a = 255;

            switch (fmt) {
            case 0: // RGBA
                if (siz == 2) { // RGBA16
                    uint32_t addr = row_addr + (x * 2);
                    if (addr + 1 < 4096) {
                        uint16_t pixel = (mem[addr] << 8) | mem[addr + 1];
                        r = ((pixel >> 11) & 0x1F) * 255 / 31;
                        g = ((pixel >> 6)  & 0x1F) * 255 / 31;
                        b = ((pixel >> 1)  & 0x1F) * 255 / 31;
                        a = (pixel & 1) ? 255 : 0;
                    }
                } else if (siz == 3) { // RGBA32
                    uint32_t addr = row_addr + (x * 4);
                    if (addr + 3 < 4096) {
                        r = mem[addr + 0];
                        g = mem[addr + 1];
                        b = mem[addr + 2];
                        a = mem[addr + 3];
                    }
                }
                break;

            case 1: // YUV
                if (siz == 2) {
                    uint32_t pair_index = x / 2;
                    uint32_t addr = row_addr + (pair_index * 4);
                    if (addr + 3 < 4096) {
                        uint8_t y0 = mem[addr + 0];
                        uint8_t u  = mem[addr + 1];
                        uint8_t y1 = mem[addr + 2];
                        uint8_t v  = mem[addr + 3];

                        uint8_t luma = (x % 2 == 0) ? y0 : y1;
                        int u1 = (int)u - 128;
                        int v1 = (int)v - 128;

                        r = std::clamp((int)(luma + 1.402f * v1), 0, 255);
                        g = std::clamp((int)(luma - 0.344136f * u1 - 0.714136f * v1), 0, 255);
                        b = std::clamp((int)(luma + 1.772f * u1), 0, 255);
                        a = 255;
                    }
                }
                break;

            case 2: // CI (Color Indexed)
                if (siz == 0) { // CI4
                    uint32_t addr = row_addr + (x / 2);
                    if (addr < 4096) {
                        uint8_t byte = mem[addr];
                        uint8_t idx = (x % 2 == 0) ? (byte >> 4) : (byte & 0x0F);
                        uint8_t tlut_idx = (pal << 4) | idx;
                        get_tlut_color(tlut_idx, r, g, b, a);
                    }
                } else if (siz == 1) { // CI8
                    uint32_t addr = row_addr + x;
                    if (addr < 4096) {
                        uint8_t idx = mem[addr];
                        get_tlut_color(idx, r, g, b, a);
                    }
                }
                break;

            case 3: // IA (Intensity + Alpha)
                if (siz == 0) { // IA4
                    uint32_t addr = row_addr + (x / 2);
                    if (addr < 4096) {
                        uint8_t byte = mem[addr];
                        uint8_t nibble = (x % 2 == 0) ? (byte >> 4) : (byte & 0x0F);
                        r = g = b = ((nibble >> 1) & 0x07) * 255 / 7;
                        a = (nibble & 1) ? 255 : 0;
                    }
                } else if (siz == 1) { // IA8
                    uint32_t addr = row_addr + x;
                    if (addr < 4096) {
                        uint8_t byte = mem[addr];
                        r = g = b = ((byte >> 4) & 0x0F) * 255 / 15;
                        a = (byte & 0x0F) * 255 / 15;
                    }
                } else if (siz == 2) { // IA16
                    uint32_t addr = row_addr + (x * 2);
                    if (addr + 1 < 4096) {
                        r = g = b = mem[addr];
                        a = mem[addr + 1];
                    }
                }
                break;

            case 4: // I (Intensity)
                if (siz == 0) { // I4
                    uint32_t addr = row_addr + (x / 2);
                    if (addr < 4096) {
                        uint8_t byte = mem[addr];
                        uint8_t nibble = (x % 2 == 0) ? (byte >> 4) : (byte & 0x0F);
                        r = g = b = a = nibble * 255 / 15;
                    }
                } else if (siz == 1) { // I8
                    uint32_t addr = row_addr + x;
                    if (addr < 4096) {
                        r = g = b = a = mem[addr];
                    }
                }
                break;

            default:
                break;
            }

            rgba[pixel_index * 4 + 0] = r;
            rgba[pixel_index * 4 + 1] = g;
            rgba[pixel_index * 4 + 2] = b;
            rgba[pixel_index * 4 + 3] = a;
        }
    }

    return rgba;
}

GFX::GFX()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "ScuffedN64", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window,[](GLFWwindow* window, int width, int height) -> void{
        glViewport(0, 0, width, height);
    });

    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    std::string vertCode = load_shader_source("shaders/shader.vert");
    std::string fragCode = load_shader_source("shaders/shader.frag");

    const char* vertexShaderSource   = vertCode.c_str();
    const char* fragmentShaderSource = fragCode.c_str();

    uint32_t vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    uint32_t fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture_0"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture_1"), 1);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);  

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
    glEnableVertexAttribArray(2);

}

GFX::~GFX()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

void GFX::render_cycle()
{

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    

    glUseProgram(shaderProgram);

    for (auto& dc : draw_calls) {
        drawTriangle(dc);
    }

    glfwPollEvents();
    glfwSwapBuffers(window);

    vertices.clear();
    draw_calls.clear();
    current_tex = 0;
}

GLuint GFX::create_new_texture(uint8_t tile)
{
    if (current_tex == frameTextures.size())
    {
        GLuint tex;
        glGenTextures(1, &tex);
        frameTextures.push_back(tex);
    }

    GLuint tex = frameTextures[current_tex++];

    float s0 = tiles[tile].uls / 4.0f;
    float t0 = tiles[tile].ult / 4.0f;
    float s1 = tiles[tile].lrs / 4.0f;
    float t1 = tiles[tile].lrt / 4.0f;

    float width = (int)(s1 - s0 + 1);
    float height = (int)(t1 - t0 + 1);

    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::vector<uint8_t> tex_data = decode_tex(tiles[tile].tmem * 8,
        tiles[tile].line,
        tiles[tile].fmt,
        tiles[tile].siz,
        tiles[tile].palette,
        width,
        height,
        tmem,
        tlut_buffer);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data.data());

    return tex;

}

void GFX::drawTriangle(const DrawCall& dc)
{
    Vertex verts[] = {
        dc.v0,
        dc.v1,
        dc.v2
    };

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, dc.texture0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, dc.texture1);

    glUniform1i(glGetUniformLocation(shaderProgram, "uIs2Cycle"), dc.is2Cycle);

    if(dc.cull_back && dc.cull_front){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK | GL_FRONT);
    }
    else if(dc.cull_front){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }else if(dc.cull_back){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }else{
        glDisable(GL_CULL_FACE);
    }

    if (dc.zmode == 3){
        glDepthMask(GL_FALSE);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(-2.f, -2.f);
    }else{
        glDepthMask(GL_TRUE);
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    if(dc.blender.use_cvg_as_a && ! dc.blender.cvg_x_a){
        glDisable(GL_BLEND);
    }else {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    int framebuffer_width, framebuffer_height;
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);

    float n64_vp_width  = 2.0f * (dc.view_port_state.scale[0]);
    float n64_vp_height = 2.0f * (dc.view_port_state.scale[1]);

    float n64_vp_x = (dc.view_port_state.trans[0]) - (n64_vp_width / 2.0f);
    float n64_vp_y = (dc.view_port_state.trans[1]) - (n64_vp_height / 2.0f);

    constexpr float N64_BASE_WIDTH  = 320.0f;
    constexpr float N64_BASE_HEIGHT = 240.0f;

    float scale_x = static_cast<float>(framebuffer_width)  / N64_BASE_WIDTH;
    float scale_y = static_cast<float>(framebuffer_height) / N64_BASE_HEIGHT;

    float scaled_width  = n64_vp_width  * scale_x;
    float scaled_height = n64_vp_height * scale_y;

    float scaled_x = n64_vp_x * scale_x;

    float scaled_y = static_cast<float>(framebuffer_height) - ((n64_vp_y + n64_vp_height) * scale_y);

    glViewport(static_cast<GLint>(scaled_x), 
            static_cast<GLint>(scaled_y), 
            static_cast<GLsizei>(scaled_width), 
            static_cast<GLsizei>(scaled_height));

    glUniform4f(glGetUniformLocation(shaderProgram, "uPrimColor"),
        dc.combiner.prim_r, dc.combiner.prim_g, dc.combiner.prim_b, dc.combiner.prim_a);
    glUniform4f(glGetUniformLocation(shaderProgram, "uEnvColor"), 
        dc.combiner.env_r, dc.combiner.env_g, dc.combiner.env_b, dc.combiner.env_a);
    glUniform4f(glGetUniformLocation(shaderProgram, "uBlendColor"), 
        dc.blender.blend_r, dc.blender.blend_g, dc.blender.blend_b, dc.blender.blend_a);

    // Texture 0
    glUniform1i(glGetUniformLocation(shaderProgram, "uShiftS0"), dc.tile_state0.shifts);
    glUniform1i(glGetUniformLocation(shaderProgram, "uShiftT0"), dc.tile_state0.shiftt);

    glUniform1i(glGetUniformLocation(shaderProgram, "uMaskS0"), dc.tile_state0.masks);
    glUniform1i(glGetUniformLocation(shaderProgram, "uMaskT0"), dc.tile_state0.maskt);

    glUniform1i(glGetUniformLocation(shaderProgram, "uMirrorS0"), dc.tile_state0.mirrors);
    glUniform1i(glGetUniformLocation(shaderProgram, "uMirrorT0"), dc.tile_state0.mirrort);

    glUniform1i(glGetUniformLocation(shaderProgram, "uClampS0"), dc.tile_state0.clamps);
    glUniform1i(glGetUniformLocation(shaderProgram, "uClampT0"), dc.tile_state0.clampt);

    glUniform1i(glGetUniformLocation(shaderProgram, "uTileWidth0"), static_cast<GLint>(dc.tile_state0.width));
    glUniform1i(glGetUniformLocation(shaderProgram, "uTileHeight0"), static_cast<GLint>(dc.tile_state0.height));

    glUniform1f(glGetUniformLocation(shaderProgram, "uTexWidth0"), static_cast<GLfloat>(dc.tile_state0.width));
    glUniform1f(glGetUniformLocation(shaderProgram, "uTexHeight0"), static_cast<GLfloat>(dc.tile_state0.height));


    // Texture 1
    glUniform1i(glGetUniformLocation(shaderProgram, "uShiftS1"), dc.tile_state1.shifts);
    glUniform1i(glGetUniformLocation(shaderProgram, "uShiftT1"), dc.tile_state1.shiftt);

    glUniform1i(glGetUniformLocation(shaderProgram, "uMaskS1"), dc.tile_state1.masks);
    glUniform1i(glGetUniformLocation(shaderProgram, "uMaskT1"), dc.tile_state1.maskt);

    glUniform1i(glGetUniformLocation(shaderProgram, "uMirrorS1"), dc.tile_state1.mirrors);
    glUniform1i(glGetUniformLocation(shaderProgram, "uMirrorT1"), dc.tile_state1.mirrort);

    glUniform1i(glGetUniformLocation(shaderProgram, "uClampS1"), dc.tile_state1.clamps);
    glUniform1i(glGetUniformLocation(shaderProgram, "uClampT1"), dc.tile_state1.clampt);

    glUniform1i(glGetUniformLocation(shaderProgram, "uTileWidth1"), static_cast<GLint>(dc.tile_state1.width));
    glUniform1i(glGetUniformLocation(shaderProgram, "uTileHeight1"), static_cast<GLint>(dc.tile_state1.height));

    glUniform1f(glGetUniformLocation(shaderProgram, "uTexWidth1"), static_cast<GLfloat>(dc.tile_state1.width));
    glUniform1f(glGetUniformLocation(shaderProgram, "uTexHeight1"), static_cast<GLfloat>(dc.tile_state1.height));
    
    glUniform1i(glGetUniformLocation(shaderProgram, "uAlphaCompare"), dc.blender.alpha_compare);

    glUniform1i(glGetUniformLocation(shaderProgram, "uCC0_A"), dc.combiner.cc0_a);
    glUniform1i(glGetUniformLocation(shaderProgram, "uCC0_B"), dc.combiner.cc0_b);
    glUniform1i(glGetUniformLocation(shaderProgram, "uCC0_C"), dc.combiner.cc0_c);
    glUniform1i(glGetUniformLocation(shaderProgram, "uCC0_D"), dc.combiner.cc0_d);

    glUniform1i(glGetUniformLocation(shaderProgram, "uAC0_A"), dc.combiner.ac0_a);
    glUniform1i(glGetUniformLocation(shaderProgram, "uAC0_B"), dc.combiner.ac0_b);
    glUniform1i(glGetUniformLocation(shaderProgram, "uAC0_C"), dc.combiner.ac0_c);
    glUniform1i(glGetUniformLocation(shaderProgram, "uAC0_D"), dc.combiner.ac0_d);

    glUniform1i(glGetUniformLocation(shaderProgram, "uCC1_A"), dc.combiner.cc1_a);
    glUniform1i(glGetUniformLocation(shaderProgram, "uCC1_B"), dc.combiner.cc1_b);
    glUniform1i(glGetUniformLocation(shaderProgram, "uCC1_C"), dc.combiner.cc1_c);
    glUniform1i(glGetUniformLocation(shaderProgram, "uCC1_D"), dc.combiner.cc1_d);

    glUniform1i(glGetUniformLocation(shaderProgram, "uAC1_A"), dc.combiner.ac1_a);
    glUniform1i(glGetUniformLocation(shaderProgram, "uAC1_B"), dc.combiner.ac1_b);
    glUniform1i(glGetUniformLocation(shaderProgram, "uAC1_C"), dc.combiner.ac1_c);
    glUniform1i(glGetUniformLocation(shaderProgram, "uAC1_D"), dc.combiner.ac1_d);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(verts),
        verts,
        GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

DrawCall::DrawCall(Vertex v0, 
    Vertex v1, 
    Vertex v2, 
    GLuint texture0, 
    GLuint texture1, 
    CombinerState combiner, 
    uint64_t othermode, 
    Eigen::Vector4f blend_colr,
    Tile tile0,
    Tile tile1,
    ViewPort view_port,
    uint32_t geometry_mode
): v0(v0), v1(v1), v2(v2), texture0(texture0), texture1(texture1), combiner(combiner)
{
    is2Cycle = ((othermode >> 52) & 0x3) == 1;
    zmode = ((othermode >> 10) & 0x3);
    cull_back = ((geometry_mode >> 10) & 0x1);
    cull_front = ((geometry_mode >> 9) & 0x1);

    blender.p0 = (othermode >> 30) & 0x3;
    blender.a0 = (othermode >> 26) & 0x3;
    blender.m0 = (othermode >> 22) & 0x3;
    blender.b0 = (othermode >> 18) & 0x3;

    blender.p1 = (othermode >> 28) & 0x3;
    blender.a1 = (othermode >> 24) & 0x3;
    blender.m1 = (othermode >> 20) & 0x3;
    blender.b1 = (othermode >> 16) & 0x3;
    blender.cvg_x_a = (othermode >> 12) & 1;
    blender.use_cvg_as_a = (othermode >> 13) & 1;
    blender.force_blend = (othermode >> 14) & 1;
    blender.alpha_compare = othermode & 0x3;
    blender.blend_r = blend_colr(0);
    blender.blend_g = blend_colr(1);
    blender.blend_b = blend_colr(2);
    blender.blend_a = blend_colr(3);

    tile_state0.mirrors = tile0.cms & 1;
    tile_state0.mirrort = tile0.cmt & 1;
    tile_state0.clamps =  tile0.cms & 2;
    tile_state0.clampt =  tile0.cmt & 2;
    tile_state0.height =  tile0.height;
    tile_state0.width =   tile0.width;
    tile_state0.masks =   tile0.masks;
    tile_state0.maskt =   tile0.maskt;
    tile_state0.shifts =  tile0.shifts;
    tile_state0.shiftt =  tile0.shiftt;


    tile_state1.mirrors = tile1.cms & 1;
    tile_state1.mirrort = tile1.cmt & 1;
    tile_state1.clamps =  tile1.cms & 2;
    tile_state1.clampt =  tile1.cmt & 2;
    tile_state1.height =  tile1.height;
    tile_state1.width =   tile1.width;
    tile_state1.masks =   tile1.masks;
    tile_state1.maskt =   tile1.maskt;
    tile_state1.shifts =  tile1.shifts;
    tile_state1.shiftt =  tile1.shiftt;

    view_port_state.scale[0] = view_port.scale[0];
    view_port_state.scale[1] = view_port.scale[1];
    view_port_state.scale[2] = view_port.scale[2];
    view_port_state.scale[3] = view_port.scale[3];
    view_port_state.trans[0] = view_port.trans[0];
    view_port_state.trans[1] = view_port.trans[1];
    view_port_state.trans[2] = view_port.trans[2];
    view_port_state.trans[3] = view_port.trans[3];
}