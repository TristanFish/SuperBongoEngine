#version 450
out vec4 fragColor;

in vec2 vertUV;

uniform sampler2D posTexture;
uniform sampler2D normTexture;
uniform sampler2D albedoTexture;
uniform sampler2D depthTexture;
uniform isampler2D stencilTexture;

void main()
{
	vec4 col = texture(albedoTexture, vertUV);
	fragColor = col;
}