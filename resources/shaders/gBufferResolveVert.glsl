#version 450
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;

out vec2 vertUV;

void main()
{
	vertUV = inUV;
	gl_Position = vec4(inPosition, 1.0);
}