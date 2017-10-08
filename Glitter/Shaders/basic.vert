#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;

uniform mat4 transform;

out vec4 position;
out vec3 normal;
out vec2 uv;

void main()
{
	uv = vUV;
	normal = vNormal;
	position = transform * vec4(vPosition, 1.0f);
    gl_Position = position;
}