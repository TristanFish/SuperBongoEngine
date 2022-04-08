#version 450
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;


out vec3 eyeDir;
out vec3 vertPos;
out vec3 vertNormal;
out vec2 vertUV;
out unsigned int uuid;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform uint uniqueID = 0;

void main()
{
	eyeDir = normalize(vec3(viewMatrix * vec4(inPosition, 1.0)));
	vertNormal = normalMatrix * inNormal;
	vertUV = inUV;
	vertPos = vec3(modelMatrix * vec4(inPosition, 1.0));
	uuid = uniqueID; 
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}