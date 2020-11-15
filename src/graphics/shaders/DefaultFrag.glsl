#version 450
layout (location = 0) out vec4 gAlbedo;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gPosition;
layout (location = 3) out vec4 gDepth;

in vec3 vertNormal;
in vec2 vertUV;

uniform vec4 meshColor;
uniform sampler2D diffuseTex1;

void main()
{
	gAlbedo = texture(diffuseTex1, vertUV) + meshColor;
	gNormal = vertNormal;
	gDepth = vec4(vec3(gl_FragCoord.z), 1.0);
}