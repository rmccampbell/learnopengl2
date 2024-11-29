#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Texture {
	sampler2D texture;
	bool bound;
};

vec4 GetTexture(Texture tex, vec2 texCoords) {
	return tex.bound ? texture(tex.texture, texCoords) : vec4(1.0);
}

struct Material {
    Texture diffuse_texture;
    Texture specular_texture;
    Texture emissive_texture;
    Texture ao_texture;
	vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    vec3 emissive_color;
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

struct SpotLight {
    vec3 position;
	vec3 direction;
	float inner_cuttoff;
	float outer_cuttoff;

	float constant;
	float linear;
	float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_LIGHTS 10

uniform vec3 viewPos;
uniform Material material;

uniform int numDirLights, numPointLights, numSpotLights;
uniform DirLight dirLights[MAX_LIGHTS];
uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS];

vec4 diffTex = GetTexture(material.diffuse_texture, TexCoords);
vec4 specTex = GetTexture(material.specular_texture, TexCoords);
vec4 emissTex = GetTexture(material.emissive_texture, TexCoords);
vec4 aoTex = GetTexture(material.ao_texture, TexCoords);

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = material.shininess > 0.0 ?
		pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) : 0.0;
    // combine results
    vec3 ambient  = light.ambient  * material.ambient_color  * diffTex.rgb;
    vec3 diffuse  = light.diffuse  * material.diffuse_color  * diffTex.rgb * diff;
    vec3 specular = light.specular * material.specular_color * specTex.rgb * spec;
    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDisp = light.position - fragPos;
	float lightDist = length(lightDisp);
	vec3 lightDir = lightDisp / lightDist;
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = material.shininess > 0.0 ?
		pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) : 0.0;
	// attenuation
	float attenuation = 1 / (light.constant + light.linear*lightDist + light.quadratic*lightDist*lightDist);
    // combine results
    vec3 ambient  = light.ambient  * material.ambient_color  * diffTex.rgb;
    vec3 diffuse  = light.diffuse  * material.diffuse_color  * diffTex.rgb * diff;
    vec3 specular = light.specular * material.specular_color * specTex.rgb * spec;
    return ambient + (diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDisp = light.position - fragPos;
	float lightDist = length(lightDisp);
	vec3 lightDir = lightDisp / lightDist;
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = material.shininess > 0.0 ?
		pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) : 0.0;
	// attenuation
	float attenuation = 1 / (light.constant + light.linear*lightDist + light.quadratic*lightDist*lightDist);
	// spotlight intensity
	float spotCos = dot(lightDir, normalize(-light.direction));
	float spotIntensity = smoothstep(light.outer_cuttoff, light.inner_cuttoff, spotCos);
    // combine results
    vec3 ambient  = light.ambient  * material.ambient_color  * diffTex.rgb;
    vec3 diffuse  = light.diffuse  * material.diffuse_color  * diffTex.rgb * diff;
    vec3 specular = light.specular * material.specular_color * specTex.rgb * spec;
    return ambient + (diffuse + specular) * attenuation * spotIntensity;
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 color = vec3(0);

	for (int i = 0; i < numDirLights; i++)
		color += CalcDirLight(dirLights[i], norm, viewDir);
	for (int i = 0; i < numPointLights; i++)
		color += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	for (int i = 0; i < numSpotLights; i++)
		color += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);

	color *= aoTex.rgb;
	color += material.emissive_color * emissTex.rgb;

    FragColor = vec4(color, 1.0);
}
