#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec2 TexCoord;
out vec3 ourColor;

uniform mat4 model;
uniform mat4 trans;

void main()
{
	gl_Position = trans * model * vec4(aPos, 1.0f);
	ourColor = aColor;
}