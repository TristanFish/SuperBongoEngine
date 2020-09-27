#version 450
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;


out VS_OUT
{
	vec3 eyeDir;
	vec3 vertPos;
	vec3 vertNormal;
	vec2 vertUV;
	int activeLights;

	vec3 lPos;
	vec3 lAmb;
	vec3 lDiff;
	vec3 lSpec;
	float lIntens;

} vertexOutput;



uniform vec3 lightPos[2];
uniform vec3 lightAmb[2];
uniform vec3 lightDiff[2];
uniform vec3 lightSpec[2];
uniform float lightIntens[2];
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform int activeLights;

void main()
{
	vertexOutput.eyeDir = normalize(vec3(viewMatrix * vec4(inPosition, 1.0)));
	vertexOutput.vertPos = inPosition;
	vertexOutput.vertNormal = normalMatrix * inNormal;
	vertexOutput.vertUV = inUV;
	vertexOutput.activeLights = activeLights;
	for(int i = 0; i < activeLights; i++)
	{
		vertexOutput.lPos  = lightPos[i];
		vertexOutput.lAmb = lightAmb[i];
		vertexOutput.lDiff = lightDiff[i];
		vertexOutput.lSpec  = lightSpec[i];
		vertexOutput.lIntens  = lightIntens[i];
	}

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}