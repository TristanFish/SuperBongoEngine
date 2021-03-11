#version 450
out vec4 fragColor;

in vec2 vertUV;

uniform sampler2D albedoTexture;
uniform sampler2D normTexture;
uniform sampler2D posTexture;
uniform sampler2D depthTexture;
uniform usampler2D stencilTexture;

void main()
{

	vec4 col = texture(albedoTexture, vertUV);
	uvec4 sten = texture(stencilTexture, vertUV);
	vec4 stencilModifier = vec4(1.0, 1.0, 1.0, 1.0);
	if (sten.r == 1)
	{
		stencilModifier = vec4(1.0, 0.0, 0.0, 1.0);
	}
	fragColor = col * stencilModifier;
}