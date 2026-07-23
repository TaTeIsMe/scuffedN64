#pragma once
#include"glad/glad.h"
#include<GLFW/glfw3.h>
#include <Eigen/Dense>
#include "Vertex.h"
#include <vector>

struct Tile {
    uint8_t fmt;
    uint8_t siz;

    uint16_t line;
    uint16_t tmem;

    uint8_t palette;

    uint8_t cms;
    uint8_t masks;
    uint8_t shifts;

    uint8_t cmt;
    uint8_t maskt;
    uint8_t shiftt;

    uint16_t uls;
    uint16_t ult;
    uint16_t lrs;
    uint16_t lrt;

    GLint tex;
};

struct DrawCall {
    Vertex v0, v1, v2;
    GLuint texture;
};

class GFX
{
public:
    GFX();
    ~GFX();
    void render_cycle();
    GLuint create_new_texture();
    void drawTriangle(const DrawCall &dc);

    std::vector<Vertex> vertices;
    uint32_t VAO = 0;
    uint32_t VBO = 0;
    GLFWwindow* window;
    uint32_t shaderProgram = 0;
    uint32_t theOneTexture = 0;
    Tile tiles[8];
    std::vector<DrawCall> draw_calls;
    std::vector<GLuint> frameTextures;
    uint32_t current_tex = 0;

    uint8_t tmem[4096];
    uint16_t tlut_buffer[256]{};
    float s = 1.0f;
    float t = 1.0f;
    bool on = false;
    uint8_t active_tile = 0;

};