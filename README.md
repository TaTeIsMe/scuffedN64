# Scuffed64
A Nintendo 64 emulator written in C++.

## VR4300 interpreter
This project contains two cores: a regular interpreter modelling most of MIPS behaviours correctly and a pipeline core. 

## HLE RSP
Scuffed can currently render The Legend of Zelda: Ocarina of Time, since **F3DZEX2** is the microcode it interprets.
Eigen is used to store and multiply pushed matrices to create the modelview projection matrix.
Vertices are then all multiplied by that matrix, and as such the triangles the GPU receives are already in clip space.
I used eigen to see if it's faster than my own matrix implementation from the software renderer I made. It isn't.

![Ocarina of Time Title Screen](/images/title_screen.png)

## OpenGL powered graphics
All the draw calls made by the RSP are gathered and rendered every frame using OpenGL shaders.
They process each fragment according to color combiner settings set by the RSP.


![In-Game Gameplay](/images/game_running.png)

## Building
The project only builds on Linux, and expects a .z64 ROM.

### Prerequisites
* C++20
* Make
* Eigen3
* OpenGL
* GLFW 3

### Commands

```bash
# Debug Build
make

# Release Build (Optimized)
make RELEASE=1

# Run ROM
./build/main <path_to_rom.z64>