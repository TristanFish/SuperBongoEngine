#version 450
#define MAX_LIGHTS 1

out vec4 fragColor;


in VS_OUT
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

} fragmentInput;

uniform vec4 meshColor = vec4(0.0, 0.0, 0.0, 0.0);
uniform sampler2D diffuseTex1;

void main()
{
	for(int i = 0; i < fragmentInput.activeLights; i++)
	{
		float tempDiff = max(dot(fragmentInput.vertNormal, normalize(fragmentInput.lPos)), 0.0);
		/// Reflection is based incedent which means a vector from the light source
		/// not the direction to the light source
		vec3 reflection = normalize(reflect(-fragmentInput.lPos, fragmentInput.vertNormal));
		float tempSpec = max(dot(fragmentInput.eyeDir, reflection), 0.0);
	
		if(tempDiff > -0.01)
		{
			tempSpec = pow(tempSpec,14.0);
		}

		//fragColor += vec4((tempDiff * fragmentInput.lights[i].diffuse) + (tempSpec * fragmentInput.lights[i].specular), 0.0);	

	}
	fragColor +=  vec4(fragmentInput.lPos, 0.0);
}