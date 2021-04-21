#version 450
out vec4 fragColor;

in vec3 vertNormal;
in vec2 vertUV;

uniform vec4 meshColor;
uniform sampler2D diffuseTex1;


void main()
{
	fragColor =   meshColor;
}