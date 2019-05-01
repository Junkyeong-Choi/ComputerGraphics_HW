#version 460 core
in vec4 aIllumination;
in vec2 TexCoords;

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};

out vec4 FragColor;

uniform vec3 aColor;
uniform bool useTexture;
uniform Material material;

void main()
{
	FragColor = aIllumination * (useTexture ? texture(material.texture_diffuse1, TexCoords) : vec4(aColor, 1.0f));
}