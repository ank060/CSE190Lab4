#version 410 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 vertNormal;
in vec3 vertPos;
in vec3 cameraPos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

uniform vec3 color;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct DirLight
{
	vec3 direction;
	vec3 color;
};

struct PointLight
{
	vec3 color;
	vec3 position;
	float linear;
};

const Material material = Material(
	vec3(0.2, 0.2, 0.2),
	vec3(0.4, 0.4, 0.4),
	vec3(0.1, 0.1, 0.1),
	0.6
);

const DirLight dirLight = DirLight(
	vec3(0.5, 0.5, 0.5),
	vec3(0.7, 0.7, 0)
);

const PointLight pointLight = PointLight(
	vec3(0.5, 0.5, 0.5),
	vec3(0, 0, 0),
	1.0
);

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// Combine
	vec3 diffuse = diff * material.diffuse * light.color;
	vec3 specular = spec * material.specular * light.color;
	return diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.linear * distance);
	// Combine
	vec3 diffuse = diff * material.diffuse * light.color;
	vec3 specular = spec * material.specular * light.color;
	diffuse *= attenuation;
	specular *= attenuation;
	return diffuse + specular;
}

void main()
{
	vec3 norm = normalize(vertNormal);
	vec3 viewDir = normalize(cameraPos - vertPos);

	vec3 result = color;
	// material.ambient;

	result += calcDirLight(dirLight, norm, viewDir);
	result += calcPointLight(pointLight, norm, vertPos, viewDir);

    fragColor = vec4(result, 1.0);
}
