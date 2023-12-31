#version 460 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

const float discardAlpha = 0.1;

struct Material {
	sampler2D textureDiffuse;
	sampler2D textureSpecular;

	float shininess;
};

struct DirectionLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;

	float cutOff;
	float outterCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct PointLight {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform PointLight pointLight;
uniform DirectionLight dirLight;
uniform SpotLight spotLight;
uniform vec3 viewPos;

uniform int damaged;


out vec4 FragColor;

vec3 calcDirectionLighting(DirectionLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	vec3 texDiffuse = vec3(texture(material.textureDiffuse, texCoords));
	vec3 ambient = light.ambient * texDiffuse;

	// 퐁모델의 디퓨즈 항
	vec3 vNorm = normalize(normal);
	vec3 lightDirection = normalize(-light.direction);

	float diffuseN = max(dot(vNorm, lightDirection), 0.0f);
	vec3 diffuse = light.diffuse * (diffuseN * texDiffuse);

	vec3 reflectDirection = reflect(-lightDirection, vNorm);
	float spec = pow(max(dot(viewDir, reflectDirection), 0.0f), material.shininess);
	vec3 specular = spec * (light.specular * vec3(texture(material.textureSpecular, texCoords)));

	return (ambient + diffuse);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{	
	vec3 ambient = light.ambient * vec3(texture(material.textureDiffuse, texCoords));

	// 퐁모델의 디퓨즈 항
	vec3 vNorm = normalize(normal);
	vec3 lightDirection = normalize(light.position - fragPos);

	float diffuseN = max(dot(vNorm, lightDirection), 0.0f);
	vec3 diffuse = light.diffuse * (diffuseN * vec3(texture(material.textureDiffuse, texCoords)));

	// 퐁모델의 스페큘러 항
	vec3 viewDirection = normalize(viewPos - fragPos);
	vec3 reflectDirection = reflect(-lightDirection, vNorm);
	float spec = pow(max(dot(viewDir, reflectDirection), 0.0f), material.shininess);
	vec3 specular = spec * (light.specular * vec3(texture(material.textureSpecular, texCoords)));

	float dist = length(light.position - fragPos);
	float attenuationUnder = light.constant + light.linear * dist + light.quadratic * dist * dist;
	float attenuation = 1.0f / attenuationUnder;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir,  vec3 fragPos)
{
	vec3 lightDirection = normalize(light.position - fragPos);
	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon = light.cutOff - light.outterCutOff;
	float intensity = clamp((theta - light.outterCutOff) / epsilon, 0.0f, 1.0f);

	vec3 resultColor = vec3(0.0f);
	vec3 ambient = light.ambient * vec3(texture(material.textureDiffuse, texCoords));

	// 퐁모델의 디퓨즈 항
	vec3 vNorm = normalize(normal);

	float diffuseN = max(dot(vNorm, lightDirection), 0.0f);
	vec3 diffuse = light.diffuse * (diffuseN * vec3(texture(material.textureDiffuse, texCoords)));

	// 퐁모델의 스페큘러 항
	vec3 reflectDirection = reflect(-lightDirection, vNorm);
	float spec =  pow(max(dot(viewDir, reflectDirection), 0.0f), material.shininess);
	vec3 specular =  light.specular * spec * vec3(texture(material.textureSpecular, texCoords));

	float dist = length(light.position - fragPos);
	float attenuationUnder = light.constant + light.linear * dist + light.quadratic * (dist * dist);
	float attenuation = 1.0f / attenuationUnder;

	// ambient *= attenuation; // remove ambient * attenuation
	diffuse *= intensity;
	specular *= intensity; 

	return ambient + diffuse;
}

void main()
{
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 resultColor = calcDirectionLighting(dirLight, normal, viewDir, fragPos);
	resultColor += calcPointLight(pointLight, normal, viewDir, fragPos);
	resultColor += calcSpotLight(spotLight, normal, viewDir, fragPos);

	float texAlpha = texture(material.textureDiffuse, texCoords).a;

	if(damaged == 1) {
		resultColor.r = clamp(resultColor.r + 0.3,0.0,1.0);
	}

	FragColor = vec4(resultColor, 1.f);
}