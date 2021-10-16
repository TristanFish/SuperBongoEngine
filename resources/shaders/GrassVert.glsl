#version 450
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in mat4 instanceMatrix;


out vec3 vertNormal;
out vec3 vertPos;
out vec2 vertUV;
out vec3 eyeDir;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

void main()
{
    vertPos = vec3(instanceMatrix * vec4(inPosition, 1.0));
	vec3 vertDir = normalize(vec3((viewMatrix * vec4(vertPos, 1.0))));
	eyeDir = -vertDir;
	vertNormal = normalMatrix * inNormal;
	vertUV = inUV;

	gl_Position = projectionMatrix * viewMatrix * instanceMatrix * vec4(inPosition, 1.0);
}