#version 330 core
in vec4 position;
in vec2 uv;

uniform sampler2D objTexture;

out vec4 fColor;

void main()
{
    fColor = /*vec4(1.0f, 1.0f, 1.0f, 1.0f);/*/texture(objTexture, uv);
}