#version 460 core
out vec4 FragColor;

in vec3 Color;

uniform bool isBackgroundColor;
uniform vec3 backgroundColor;

void main()
{
	if (isBackgroundColor)
		FragColor = vec4(backgroundColor, 1.0);
	else
		FragColor = vec4(Color, 1.0);
}