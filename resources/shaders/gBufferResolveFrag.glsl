#version 450
#define MAX_LIGHTS 10
layout (location = 0) out vec4 fragColor;

in vec2 vertUV;

uniform sampler2D albedoTexture;
uniform sampler2D normTexture;
uniform sampler2D posTexture;
uniform sampler2D depthTexture;
uniform usampler2D stencilTexture;

uniform vec3 camPos;
uniform unsigned int activeLights = 0;
uniform vec3 lightsPos[MAX_LIGHTS];
uniform vec3 lightsAmb[MAX_LIGHTS];
uniform vec3 lightsDiff[MAX_LIGHTS];
uniform vec3 lightsSpec[MAX_LIGHTS];
uniform float lightsIntens[MAX_LIGHTS];

void main()
{
	vec4 col = texture(albedoTexture, vertUV);
	vec3 normal = texture(normTexture, vertUV).xyz;
	uint sten = texture(stencilTexture, vertUV).r;
	vec3 pos = texture(posTexture, vertUV).xyz;
	
	//	LIGHTING			= 0b00000001,
	//	CREATES_SHADOWS		= 0b00000010,
	//	RECIEVES_SHADOWS	= 0b00000100,
	//	BLOOM				= 0b00001000,
	//	PHYSICS_MOVEMENT	= 0b00010000,
	//	TRANSPARENT			= 0b00100000,
	//	WATER				= 0b01000000 

	vec4 lightCol = vec4(0.0, 0.0, 0.0, 0.0);
	vec3 finalAmbient = vec3(0.0, 0.0, 0.0);
	vec3 finalDiffuse = vec3(0.0, 0.0, 0.0);
	vec3 finalSpecular = vec3(0.0, 0.0, 0.0);

	if(sten == 1)
	{
		for(int i = 0; i < activeLights; i++)
		{
			vec3 lightDir = normalize(lightsPos[i] - pos);
			float dist = length(lightsPos[i] - pos);

			float diff = max(dot(normal, lightDir), 0.0);

			vec3 amb = lightsAmb[i];
			vec3 diffuse = lightsDiff[i] * diff;

			vec3 specCol = lightsSpec[i];
			vec3 viewDir = normalize(camPos - pos);
			vec3 reflectDir = reflect(-lightDir, normal);
			float specVal = pow(max(dot(viewDir, reflectDir), 0.0), 32);
			vec3 specular = lightsIntens[i] * specVal * specCol;

			float attenuation = lightsIntens[i] * clamp(10.0 / dist, 0.0, 1.0);
			amb *= attenuation;
			diffuse *= attenuation;
			specular *= attenuation;

			finalAmbient += amb;
			finalDiffuse += diffuse;
			finalSpecular += specular;
		}
		finalAmbient *= vec3(col);
		finalDiffuse *= vec3(col);
		finalSpecular *= vec3(col);

		lightCol = vec4(finalAmbient + finalDiffuse + finalSpecular, 1.0);
	}
	else
	{
		lightCol = col;
	}

	fragColor = lightCol;
}