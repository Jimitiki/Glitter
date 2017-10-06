#version 330 core
in vec4 position;
in vec2 uv;

uniform sampler2D objTexture;

out vec4 fColor;

void main()
{
    fColor = texture(objTexture, uv);
}