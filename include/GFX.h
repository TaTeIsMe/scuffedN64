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

    uint32_t height;
    uint32_t width;

    GLint tex;
};

struct CombinerState {
    float prim_r = 1.0f, prim_g = 1.0f, prim_b = 1.0f, prim_a = 1.0f;
    float env_r  = 0.0f, env_g  = 0.0f, env_b  = 0.0f, env_a  = 1.0f;
    
    int cc0_a = 1, cc0_b = 15, cc0_c = 4, cc0_d = 15;
    int ac0_a = 1, ac0_b = 7,  ac0_c = 4, ac0_d = 7;
    int cc1_a = 1, cc1_b = 15, cc1_c = 4, cc1_d = 15;
    int ac1_a = 1, ac1_b = 7,  ac1_c = 4, ac1_d = 7;
};

struct BlenderState {
    float blend_r = 1.0f, blend_g = 1.0f, blend_b = 1.0f, blend_a = 1.0f;
    uint8_t p0, a0, m0, b0;
    uint8_t p1, a1, m1, b1;
    bool cvg_x_a;
    bool use_cvg_as_a;
    bool force_blend;
    uint8_t alpha_compare;
};

struct TileState{

    uint8_t mirrors;
    uint8_t clamps;
    uint8_t masks;
    uint8_t shifts;

    uint8_t mirrort;
    uint8_t clampt;
    uint8_t maskt;
    uint8_t shiftt;

    uint32_t width;
    uint32_t height;

};

struct ViewPort{
    uint16_t scale[4];
    uint16_t trans[4];
};

struct DrawCall {
    DrawCall(Vertex v0, Vertex v1, Vertex v2, GLuint texture0, GLuint texture1, CombinerState combiner, uint64_t othermode, Eigen::Vector4f blend_colr, Tile tile0, Tile tile1, ViewPort view_port);
    Vertex v0, v1, v2;
    GLuint texture0;
    GLuint texture1;
    CombinerState combiner;
    BlenderState blender;
    TileState tile_state0;
    TileState tile_state1;
    ViewPort view_port_state;
    bool is2Cycle;
};

class GFX
{
public:
    GFX();
    ~GFX();
    void render_cycle();
    GLuint create_new_texture(uint8_t tile);
    void drawTriangle(const DrawCall &dc);

    std::vector<Vertex> vertices;
    uint32_t VAO = 0;
    uint32_t VBO = 0;
    GLFWwindow* window;
    uint32_t shaderProgram = 0;
    Tile tiles[8];
    std::vector<DrawCall> draw_calls;
    std::vector<GLuint> frameTextures;
    uint32_t current_tex = 0;
    uint64_t othermode = 0;
    Eigen::Vector4f blend_colr;
    ViewPort view_port;

    uint8_t tmem[4096];
    uint16_t tlut_buffer[256]{};
    float s = 1.0f;
    float t = 1.0f;
    bool on = false;
    uint8_t active_tile = 0;

    CombinerState current_combiner;

};