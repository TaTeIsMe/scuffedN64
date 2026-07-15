#pragma once
#include"glad/glad.h"
#include<GLFW/glfw3.h>
#include <Eigen/Dense>

#include <vector>
class GFX
{
public:
    GFX();
    ~GFX();
    void render_cycle();
    std::vector<Eigen::Vector4f> vertices;
    uint32_t VAO = 0;
    uint32_t VBO = 0;
    GLFWwindow* window;
    uint32_t shaderProgram = 0;
};