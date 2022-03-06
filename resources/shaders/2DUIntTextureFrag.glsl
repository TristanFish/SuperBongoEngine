#version 450
#define MAX_LIGHTS 10
layout (location = 0) out vec4 fragColor;

in vec2 vertUV;

uniform usampler2D stencilTexture;

void main()
{
	uint sten = texture(stencilTexture, vertUV).r;

	fragColor = vec4(sten, 0.0, 0.0, 1.0);
}