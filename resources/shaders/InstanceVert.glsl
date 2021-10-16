#version 450
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in mat4 instanceMatrix;
out vec3 vertNormal;
out vec2 vertUV;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
void main()
{

	vertNormal = inNormal * 1.1;
	vertUV = inUV;

	gl_Position = projectionMatrix * viewMatrix * instanceMatrix * vec4(inPosition, 1.0);
}