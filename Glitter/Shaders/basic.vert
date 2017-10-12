#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;

uniform mat4 transform;
uniform mat4 perspective;
uniform mat4 rotate;

out vec4 position;
out vec3 rawPosition;
out vec3 normal;
out vec2 uv;

void main()
{
	uv = vUV;
	vec4 normalH = rotate * vec4(vNormal, 1.0f);
	normal = normalize(vec3(normalH.x, normalH.y, normalH.z));
	vec4 rawPositionH = transform * vec4(vPosition, 1.0f);
	position = perspective * rawPositionH;
	rawPosition = vec3(rawPositionH.x, rawPositionH.y, rawPositionH.z);
    gl_Position = position;
}