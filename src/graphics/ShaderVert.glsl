#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;


out vec3 ourColor;
out vec2 TexCoord;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    ourColor = aColor;
    TexCoord = aTexCoord;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
}
