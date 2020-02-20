#version 450
out vec4 FragColor;
out vec4 moreColors;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord);
	moreColors = texture(texture1, TexCoord);
} 


