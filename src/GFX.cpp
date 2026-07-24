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

std::vector<uint8_t> decode_tex(uint32_t offset, uint8_t fmt, uint8_t siz, uint8_t pal, int w, int h ,uint8_t* mem, uint16_t* tlut_buffer) {
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
                    uint32_t addr = offset + (i * 2);
                        uint16_t pixel = (mem[addr] << 8) | mem[addr + 1];
                        r = ((pixel >> 11) & 0x1F) * 255 / 31;
                        g = ((pixel >> 6)  & 0x1F) * 255 / 31;
                        b = ((pixel >> 1)  & 0x1F) * 255 / 31;
                        a = (pixel & 1) ? 255 : 0;
                } else if (siz == 3) { // RGBA32 (8-8-8-8)
                    uint32_t addr = offset + (i * 4);
                        r = mem[addr + 0];
                        g = mem[addr + 1];
                        b = mem[addr + 2];
                        a = mem[addr + 3];
                }
                break;

            case 1: // YUV (YUV16 4:2:2) - Full Range BT.601 Conversion
                if (siz == 2) {
                    uint32_t pair_index = i / 2;
                    uint32_t addr = offset + (pair_index * 4);
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
                    uint32_t addr = offset + (i / 2);
                        uint8_t byte = mem[addr];
                        uint8_t idx = (i % 2 == 0) ? (byte >> 4) : (byte & 0x0F);
                        uint8_t tlut_idx = (pal << 4) | idx;
                        get_tlut_color(tlut_idx, r, g, b, a);
                } else if (siz == 1) { // CI8
                    uint32_t addr = offset + i;
                        uint8_t idx = mem[addr];
                        get_tlut_color(idx, r, g, b, a);
                }
                break;

            case 3: // IA (Intensity + Alpha)
                if (siz == 0) { // IA4 (3 bits I, 1 bit A)
                    uint32_t addr = offset + (i / 2);
                        uint8_t byte = mem[addr];
                        uint8_t nibble = (i % 2 == 0) ? (byte >> 4) : (byte & 0x0F);
                        uint8_t intensity = ((nibble >> 1) & 0x07) * 255 / 7;
                        r = g = b = intensity;
                        a = (nibble & 1) ? 255 : 0;
                } else if (siz == 1) { // IA8 (4 bits I, 4 bits A)
                    uint32_t addr = offset + i;
                        uint8_t byte = mem[addr];
                        r = g = b = ((byte >> 4) & 0x0F) * 255 / 15;
                        a = (byte & 0x0F) * 255 / 15;
                } else if (siz == 2) { // IA16 (8 bits I, 8 bits A)
                    uint32_t addr = offset + (i * 2);
                        r = g = b = mem[addr];
                        a = mem[addr + 1];
                }
                break;

            case 4: // I (Intensity)
                if (siz == 0) { // I4
                    uint32_t addr = offset + (i / 2);
                        uint8_t byte = mem[addr];
                        uint8_t nibble = (i % 2 == 0) ? (byte >> 4) : (byte & 0x0F);
                        r = g = b = a = nibble * 255 / 15;
                } else if (siz == 1) { // I8
                    uint32_t addr = offset + i;
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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
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


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    auto get_gl_wrap_mode = [](uint8_t cm) -> GLenum {
        if (cm & 0x2)
            return GL_CLAMP_TO_EDGE;   // G_TX_CLAMP
        if (cm & 0x1)
            return GL_MIRRORED_REPEAT; // G_TX_MIRROR
        return GL_REPEAT;                        // G_TX_WRAP
    };

    // In texture creation / binding:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, get_gl_wrap_mode(tiles[tile].cms));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, get_gl_wrap_mode(tiles[tile].cmt));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::vector<uint8_t> tex_data = decode_tex(tiles[tile].tmem * 8,
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

    // Pass the state uniforms per draw call
    glUniform4f(glGetUniformLocation(shaderProgram, "uPrimColor"),
        dc.combiner.prim_r, dc.combiner.prim_g, dc.combiner.prim_b, dc.combiner.prim_a);
    glUniform4f(glGetUniformLocation(shaderProgram, "uEnvColor"), 
        dc.combiner.env_r, dc.combiner.env_g, dc.combiner.env_b, dc.combiner.env_a);

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