#version 450
out vec4 fragColor;

in vec2 vertUV;
uniform sampler2D tex;

void main()
{
	vec4 col = texture(tex, vertUV);

	fragColor = col;
}