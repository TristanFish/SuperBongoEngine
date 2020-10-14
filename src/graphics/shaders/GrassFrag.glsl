#version 450
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gDepth;

in vec3 vertPos;
in vec3 vertNormal;
in vec2 vertUV;
in vec3 eyeDir;
uniform vec4 meshColor;
uniform sampler2D diffuseTex1;



vec4 toonShade()
{
	const int levels = 2;
	float intensity;
	vec3 _color;
	const float scalefactor = 1.0 / levels;
	const vec3 ks = vec3(0.0, 0.8, 0.4);
	const vec3 kd = vec3(0.0, 1.0, 0.3); /// const means it cannot be changed just like C++
	const vec3 ka = 0.15 * kd;

	vec3 n = normalize(vertNormal) ;
	vec3 s = normalize(vec3(1.0,1.0,1.0) - eyeDir);
	vec3 ambient = vec3(1.0,1.0,1.0) * ka;
	intensity = dot(vec3(1.0,1.0,1.0),n);
	float sDotN = max(dot(s ,n ), 0.0);
	vec3 Diffuse = kd * ceil(sDotN * levels) * scalefactor;

	 if (intensity > 0.98)
            _color = vec3(0.2,0.6,0.0);
    else if (intensity > 0.5)
            _color = vec3(0.1,0.3,0.1);        
    else if (intensity > 0.35)
            _color = vec3(0.0,0.3,0.0);
    else 
            _color = vec3(1.0,1.0,1.0); 

    vec3 product = (ambient + ks * Diffuse) * _color;
	return vec4(product,1.0);
}


vec4 Phong()
{
	const vec4 ks = vec4(0.0, 0.4, 0.0, 1.0);
    const vec4 kd = vec4(0.0, 0.4, 0.0, 1.0); /// const means it cannot be changed just like C++
	const vec4 ka = vec4(0.0, 0.4, 0.0, 1.0);
	
	float diff = max(dot(vertNormal, vec3(1.0,1.0,1.0)), 0.0);
	/// Reflection is based incedent which means a vector from the light source
	/// not the direction to the light source

	vec3 reflection = normalize(reflect(-vec3(1.0,1.0,1.0), vertNormal));


	float spec = max(dot(eyeDir, reflection), 0.0);

	if(diff > 0.0){
		spec = pow(spec,14.0);
	}
	
	return  ka + (diff * kd) + (spec * ks);	
}

void main()
{
    vec3 product = vec3(mix(toonShade() * Phong() , meshColor,0.7));

	gPosition = vertPos;
	gNormal = vertNormal;
	gDepth = vec4(vec3(gl_FragCoord.z), 1.0);
	gAlbedo = vec4(product,1.0);
}