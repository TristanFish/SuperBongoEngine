#version 450
#define MAX_LIGHTS 10
layout (location = 0) out vec4 fragColor;



in vec2 vertUV;

uniform sampler2D albedoTexture;
uniform sampler2D normTexture;
uniform sampler2D posTexture;
uniform sampler2D depthTexture;
uniform usampler2D stencilTexture;

uniform unsigned int activeLights = 1;
uniform vec3 lightsPos[MAX_LIGHTS];
uniform vec3 lightsAmb[MAX_LIGHTS];
uniform vec3 lightsDiff[MAX_LIGHTS];
uniform vec3 lightsSpec[MAX_LIGHTS];
uniform float lightsIntens[MAX_LIGHTS];


void main()
{
	vec4 col = texture(albedoTexture, vertUV);
	vec4 normal = texture(normTexture, vertUV);
	uint sten = texture(stencilTexture, vertUV).r;
	vec4 pos = texture(posTexture, vertUV);
	
	//	LIGHTING			= 0b00000001,
	//	CREATES_SHADOWS		= 0b00000010,
	//	RECIEVES_SHADOWS	= 0b00000100,
	//	BLOOM				= 0b00001000,
	//	PHYSICS_MOVEMENT	= 0b00010000,
	//	TRANSPARENT			= 0b00100000,
	//	WATER				= 0b01000000 

	vec4 lightCol = vec4(0.0, 0.0, 0.0, 0.0);

	for(int i = 0; i < activeLights; i++)
	{

		if(sten != 1)
		{
			continue;
		}
		
		vec3 lightDir = normalize(lightsPos[i] - pos.xyz);

		float diff = max(dot(normal.xyz, lightDir), 0.0);
		
		float dist = length(lightsPos[i] - pos.xyz);

		vec3 amb = lightsAmb[i] * col.xyz;
		vec3 diffuse = lightsDiff[i] * diff * col.xyz;

		float attenuation = clamp(10.0 / dist, 0.0, 1.0);
		//vec3 spec = lightsSpec[i] * col.xyz;


		lightCol += vec4(attenuation * (amb + diffuse), 1.0);
	}

	fragColor = lightCol + col;
}