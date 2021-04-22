#version 450
layout (location = 0) in vec3 inPosition;

out vec3 texCoords;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;


void main()
{

	texCoords = inPosition;
	vec4 pos = projectionMatrix * viewMatrix * vec4(inPosition, 1.0);
	gl_Position = pos.xyww;
}