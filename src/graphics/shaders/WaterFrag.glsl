#version 450
out vec4 fragColor;

in vec3 vertNormal;
in vec2 vertUV;
in vec3 inPos;
in vec3 Normal;
in vec2 textureCoords;


uniform vec3 cameraPos;
uniform vec4 meshColor;
uniform samplerCube cube;


void main()
{

//Get red and green values and convert texture to -1,1 coordinate system
	//vec2 distortion = (texture(dudv, vec2(textureCoords.x, textureCoords.y)).rg * 2.0 -1.0) * waveStrength; 
	vec3 I = normalize(inPos - cameraPos);
	vec3 Reflect = reflect(I, normalize(Normal));
	vec4 reflectColor = vec4(texture(cube, Reflect).rgb, 1.0);

	float refractionRatio = 1.00 / 1.55;
	vec3 Refract = refract(I, normalize(Normal), refractionRatio );
	vec4 refractColor = vec4(texture(cube, Refract).rgb, 1.0);

	gl_FragColor = mix(reflectColor,refractColor, 0.2);
	
}