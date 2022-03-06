#version 450
#define MAX_LIGHTS 20
layout (location = 0) out vec4 fragColor;

in vec2 vertUV;

uniform sampler2D albedoTexture;
uniform sampler2D normTexture;
uniform sampler2D posTexture;
uniform sampler2D depthTexture;
uniform usampler2D stencilTexture;

uniform vec3 camPos;
uniform unsigned int activeLights = 0;

uniform struct LightInfo
{
	//4th component of Amb holds constant for attenuation
	vec4 lightAmb;	
	//4th component of Diff holds linear for attenuation
	vec4 lightDiff;	
	//4th component of Spec holds quadratic for pointLight attenuation
	vec4 lightSpec;	
	//4th component of lightDir holds cutoff for Spotlight
	vec4 lightDir;
	vec3 lightPos;
	//lightIntens holds outerCutOff for SpotLights
	float lightIntens;
	int lightType;		//What type of light it is, 1 = Point, 2 = Spot, 3 = Directional
} 
lights[MAX_LIGHTS];

vec4 RenderSpotLight(LightInfo l, vec3 norm, vec3 pos)
{
	vec3 specCol = l.lightSpec.xyz;
	vec3 amb = l.lightAmb.xyz;
	vec3 viewDir = normalize(camPos - pos);
	vec3 specular = vec3(0.0, 0.0, 0.0);

	vec3 lightDir = normalize(l.lightPos - pos);
	float theta = dot(lightDir, normalize(-l.lightDir.xyz));
	float epsilon = l.lightDir.w - l.lightIntens;
	float intensity = clamp((theta - l.lightIntens) / epsilon, 0.0, 1.0);

	float diff = max(dot(norm, lightDir), 0.0);
	if(diff > 0.0)
	{
		vec3 reflectDir = reflect(-lightDir, norm);
		float specVal = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		specular = intensity * specVal * specCol;
	}

	vec3 diffuse = intensity * l.lightDiff.xyz * diff;

	return vec4(amb + diffuse + specular, 0.0);
}

vec4 RenderPointLight(LightInfo l, vec3 norm, vec3 pos)
{
	vec3 specCol = l.lightSpec.xyz;
	vec3 amb = l.lightAmb.xyz;

	vec3 viewDir = normalize(camPos - pos);
	vec3 specular = vec3(0.0, 0.0, 0.0);

	vec3 lightDir = normalize(l.lightPos - pos);
	float dist = length(l.lightPos - pos);

	float diff = max(dot(norm, lightDir), 0.0);
	if(diff > 0.0)
	{
		vec3 reflectDir = reflect(-lightDir, norm);
		float specVal = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		specular = l.lightIntens * specVal * specCol;
	}

	vec3 diffuse = l.lightDiff.xyz * diff;


	float attenuation = 1.0 / (l.lightAmb.w + l.lightDiff.w * dist + l.lightSpec.w * (dist * dist));
	attenuation *= l.lightIntens;
	amb *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return vec4(amb + diffuse + specular, 0.0);
}

vec4 RenderDirectionalLight(LightInfo l, vec3 norm, vec3 pos)
{
	vec3 specCol = l.lightSpec.xyz;
	vec3 viewDir = normalize(camPos - pos);
	vec3 lightDir = normalize(-l.lightDir.xyz);
	vec3 specular = vec3(0.0, 0.0, 0.0);

	float diff = max(dot(norm, lightDir), 0.0);

	vec3 amb = l.lightAmb.xyz;
	vec3 diffuse = l.lightDiff.xyz * diff;

	if(diff > 0.0)
	{
		vec3 reflectDir = reflect(-lightDir, norm);
		float specVal = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		specular = l.lightIntens * specVal * specCol;
	}


	return vec4(amb + diffuse + specular, 0.0);
}

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
	vec4 currentCol = vec4(0.0, 0.0, 0.0, 1.0);

	if(sten == 1)
	{
		for(int i = 0; i < activeLights; i++)
		{
			if(lights[i].lightType == 1)
			{
				currentCol += RenderPointLight(lights[i], normal, pos);
			}
			if(lights[i].lightType == 2)
			{
				currentCol += RenderSpotLight(lights[i], normal, pos);
			}
			if(lights[i].lightType == 3)
			{
				currentCol += RenderDirectionalLight(lights[i], normal, pos);
			}
		}
		currentCol *= col;

		lightCol = vec4(currentCol);
	}
	else
	{
		lightCol = col;
	}

	fragColor = lightCol;

}