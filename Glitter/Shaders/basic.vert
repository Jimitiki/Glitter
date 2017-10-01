#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;

uniform mat4 transform;

out vec4 position;
out vec4 color;

void main()
{
	color = vec4(vColor, 1.0f);
	position = vec4(vPosition, 1.0f);
    gl_Position = transform * position;
}