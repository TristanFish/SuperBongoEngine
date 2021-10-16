#version 450
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

out vec3 vertNormal;
out vec2 vertUV;
out vec3 Normal;
out vec3 inPos;

out vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

const float tiling = 5.0;
void main()
{
	Normal = mat3(transpose(inverse(modelMatrix))) * inNormal;
	inPos = vec3(modelMatrix * vec4(inPosition, 1.0));
	textureCoords = vec2(inPosition.x/2.0 + 0.5, inPosition.y/2.0 + 0.5) * tiling;
	gl_Position = projectionMatrix * viewMatrix * vec4(inPos, 1.0);
}