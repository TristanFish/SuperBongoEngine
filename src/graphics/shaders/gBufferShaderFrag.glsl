#version 450

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

in struct lightInfo
{
	float lIntens;
	vec3 lPos;
	vec3 lAmb;
	vec3 lDiff;
	vec3 lSpec;
}lights[5];

in vec3 eyeDir;
in vec3 vertPos;
in vec3 vertNormal;
in vec2 vertUV;

uniform vec4 meshColor = vec4(0.0, 0.0, 0.0, 0.0);
uniform sampler2D diffuseTex1;

void main()
{
	gPosition = vertPos;
	gNormal = vertNormal;

	float tempDiff = max(dot(vertNormal, normalize(lights[0].lPos - vertPos)), 0.0);

	vec3 reflection = normalize(reflect(-normalize(lights[0].lPos - vertPos), vertNormal));
	float tempSpec = max(dot(eyeDir, reflection), 0.0);
	
	if(tempDiff > -0.01)
	{
		tempSpec = pow(tempSpec,14.0);
	}

	gAlbedo += (tempDiff * vec4(lights[0].lDiff, 1.0)) + (tempSpec * vec4(lights[0].lSpec, 1.0));	

	gAlbedo += texture(diffuseTex1, vertUV) + meshColor + vec4(lights[0].lAmb, 0.0);
}