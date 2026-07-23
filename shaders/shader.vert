#version 460 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;

out vec2 TexCoord;
out vec4 Color;

void main()
{
   gl_Position = aPos;
   TexCoord = aTexCoord;
   Color = aColor;
}