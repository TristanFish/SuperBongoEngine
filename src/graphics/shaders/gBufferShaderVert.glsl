#version 450
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

out struct lightInfo
{
	float lIntens;
	vec3 lPos;
	vec3 lAmb;
	vec3 lDiff;
	vec3 lSpec;
}lights[5];

out vec3 eyeDir;
out vec3 vertPos;
out vec3 vertNormal;
out vec2 vertUV;


uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform lightInfo uLights[5];


void main()
{
	eyeDir = normalize(vec3(viewMatrix * vec4(inPosition, 1.0)));
	vertNormal = normalMatrix * inNormal;
	vertUV = inUV;
	vertPos = vec3(modelMatrix * vec4(inPosition, 1.0));

	lights[0].lIntens = uLights[0].lIntens;
	lights[0].lPos = uLights[0].lPos;
	lights[0].lAmb = uLights[0].lAmb;
	lights[0].lDiff = uLights[0].lDiff;
	lights[0].lSpec = uLights[0].lSpec;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}