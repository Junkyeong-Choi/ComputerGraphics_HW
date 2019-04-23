#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 Color;

uniform vec3 aColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	Color = aColor;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}