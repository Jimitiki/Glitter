#version 330 core
in vec4 position;
in vec4 color;

uniform mat4 colorMask;

out vec4 fColor;

void main()
{
    fColor = colorMask * color;
}