#version 460 core
out vec4 FragColor;

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;
in mat3 TBN;

uniform bool useNormalMap;
uniform bool useTexture;
uniform vec3 aColor;
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm;

	if (useNormalMap) {
		norm = texture(material.texture_normal1, TexCoords).rgb;
		norm = normalize(norm * 2.0f - 1.0f);
		norm = normalize(TBN * norm);
	} else {
		norm = normalize(Normal);
	}

    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    result += CalcPointLight(pointLight, norm, FragPos, viewDir);    

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * (useTexture ? vec3(texture(material.texture_diffuse1, TexCoords)) : aColor);
	vec3 diffuse = light.diffuse * diff * (useTexture ? vec3(texture(material.texture_diffuse1, TexCoords)) : aColor);
	vec3 specular = light.specular * spec * (useTexture ? vec3(texture(material.texture_specular1, TexCoords)) : aColor);

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec3 ambient = light.ambient * (useTexture ? vec3(texture(material.texture_diffuse1, TexCoords)) : aColor);
	vec3 diffuse = light.diffuse * diff * (useTexture ? vec3(texture(material.texture_diffuse1, TexCoords)) : aColor);
	vec3 specular = light.specular * spec * (useTexture ? vec3(texture(material.texture_specular1, TexCoords)) : aColor);

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}