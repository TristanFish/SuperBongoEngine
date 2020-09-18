#version 450
out vec4 fragColor;

in vec3 vertNormal;
in vec2 vertUV;

uniform sampler2D texture_diffuse1;

void main()
{
	fragColor = texture(texture_diffuse1, vertUV);
}