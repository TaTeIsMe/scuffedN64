#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 Color;

uniform sampler2D theOneTexture;
void main()
{
   FragColor = texture(theOneTexture, TexCoord) * Color;
}