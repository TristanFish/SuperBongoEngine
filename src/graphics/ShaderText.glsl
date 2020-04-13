#version 450
out vec4 FragColor;
out vec4 moreColors;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, TexCoord); 

	if(texColor.a < 0.7)
	    discard;
    FragColor = texColor;
	moreColors = texture(texture1, TexCoord);
} 


