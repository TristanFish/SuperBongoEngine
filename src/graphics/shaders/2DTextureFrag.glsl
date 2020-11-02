#version 450

layout (location = 0) out vec4 gAlbedo;

in vec2 vertUV;
uniform sampler2D tex;

void main()
{
	vec4 col = texture(tex, vertUV);
	gAlbedo = col;
}