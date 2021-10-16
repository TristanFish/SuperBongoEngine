#version 450
out vec4 fragColor;

in vec3 vertNormal;
in vec2 vertUV;
in vec3 inPos;
in vec3 Normal;

uniform vec3 cameraPos;
uniform vec4 meshColor;
uniform samplerCube cube;

void main()
{
float refractionRatio = 1.00 / 2.42;
	vec3 I = normalize(inPos - cameraPos);
	vec3 R = refract(I, normalize(Normal), refractionRatio );
	fragColor = vec4(texture(cube, R).rgb, 1.0);
}