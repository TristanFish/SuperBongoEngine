#version 450
out vec4 fragColor;

in vec2 vertUV;

uniform sampler2D albedoTexture;
uniform sampler2D normTexture;
uniform sampler2D posTexture;
uniform sampler2D depthTexture;
//uniform usampler2D stencilTexture;

uniform int activeLights = 1;
uniform vec3 lightsPos[2];
uniform vec3 lightsAmb[2];
uniform vec3 lightsDiff[2];
uniform vec3 lightsSpec[2];
uniform float lightsIntens[2];

void main()
{
	vec4 col = texture(albedoTexture, vertUV);
	vec4 normal = texture(normTexture, vertUV);
	//uvec4 sten = texture(stencilTexture, vertUV);
	vec4 pos = texture(posTexture, vertUV);
	
	//	LIGHTING			= 0b00000001,
	//	CREATES_SHADOWS		= 0b00000010,
	//	RECIEVES_SHADOWS	= 0b00000100,
	//	BLOOM				= 0b00001000,
	//	PHYSICS_MOVEMENT	= 0b00010000,
	//	TRANSPARENT			= 0b00100000,
	//	WATER				= 0b01000000 

	vec4 lightCol = vec4(0.0, 0.0, 0.0, 0.0);

//	for(int i = 0; i < activeLights; i++)
//	{
//		vec3 lightDir = normalize(lightsPos[i] - pos.xyz);
//
//		float diff = max(dot(normal.xyz, lightDir), 0.0);
//		
//		float dist = length(lightsPos[i] - pos.xyz);
//		float attenuation = lightsIntens[i] / (dist * dist);
//
//		vec3 amb = lightsAmb[i] * col.xyz;
//		vec3 diffuse = lightsDiff[i] * diff * col.xyz;
//		vec3 spec = lightsSpec[i] * col.xyz;
//
//		amb *= attenuation;
//		diffuse *= attenuation;
//		spec *= attenuation;
//
//		lightCol += amb + diffuse;
//	}

//	vec4 stenModifier = vec4(1.0, 1.0, 1.0, 1.0);
//
//	if((sten.r & 1) != 0)
//	{
//		stenModifier = vec4(1.0, 0.0, 0.0, 1.0);
//	}

	fragColor = col + lightCol;
}