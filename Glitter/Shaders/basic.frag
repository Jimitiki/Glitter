#version 330 core
in vec4 position;
in vec3 normal;
in vec2 uv;

uniform sampler2D objTexture;

out vec4 fColor;

void main()
{
    fColor = vec4((normal.x + 1) / 2, (normal.y + 1) / 2, (normal.z + 1) / 2, 1.0f);
}