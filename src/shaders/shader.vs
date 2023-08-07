#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec2 TexCoord;
out vec3 ourColor;

uniform mat4 model;
uniform mat4 trans;
uniform mat4 projection;

void main()
{
	gl_Position = projection * trans * model * vec4(aPos, 1.0f);
	ourColor = vec3(gl_Position.x, gl_Position.y, 1.0f);
}