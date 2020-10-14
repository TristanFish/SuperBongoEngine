#version 450

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gDepth;
layout (location = 4) out int gStencil;


in vec3 eyeDir;
in vec3 vertPos;
in vec3 vertNormal;
in vec2 vertUV;

uniform vec4 meshColor = vec4(0.0, 0.0, 0.0, 0.0);
uniform sampler2D diffuseTex1;

//float near = 0.1;
//float far = 150.0;

void main()
{
	gPosition = vertPos;
	gNormal = normalize(vertNormal);
	//float z = gl_FragCoord.z * 2.0 - 1.0;
	//float d = (2.0 * near * far) / (far + near - z * (far - near))/ far;
	gDepth = vec4(vec3(gl_FragCoord.z), 1.0);
	gStencil = 254;


	gAlbedo = texture(diffuseTex1, vertUV) + meshColor;
}