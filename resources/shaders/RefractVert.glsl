#version 450
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

out vec3 vertNormal;
out vec2 vertUV;
out vec3 Normal;
out vec3 inPos;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{

	Normal = mat3(transpose(inverse(modelMatrix))) * inNormal;
	inPos = vec3(modelMatrix * vec4(inPosition, 1.0));
	gl_Position = projectionMatrix * viewMatrix * vec4(inPos, 1.0);
}