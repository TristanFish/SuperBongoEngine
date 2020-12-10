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
	vec3 I = normalize(inPos - cameraPos);
	vec3 R = reflect(I, normalize(Normal));
	fragColor = vec4(texture(cube, R).rgb, 1.0);
}