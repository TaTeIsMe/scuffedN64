#pragma once
#include"glad/glad.h"
#include<GLFW/glfw3.h>
#include <Eigen/Dense>
#include "Vertex.h"
#include <vector>

// Key to uniquely identify a texture in memory
struct TextureKey {
    uint32_t address;
    uint16_t width;
    uint16_t height;

    bool operator==(const TextureKey& other) const {
        return address == other.address && width == other.width && height == other.height;
    }
};

// Hash function for TextureKey
struct TextureKeyHash {
    std::size_t operator()(const TextureKey& k) const {
        return std::hash<uint32_t>()(k.address) ^ 
              (std::hash<uint32_t>()((uint32_t)k.width << 16 | k.height));
    }
};

class GFX
{
public:
    GFX();
    ~GFX();
    void render_cycle();

    std::vector<Vertex> vertices;
    uint32_t VAO = 0;
    uint32_t VBO = 0;
    GLFWwindow* window;
    uint32_t shaderProgram = 0;
    uint32_t theOneTexture = 0;

};