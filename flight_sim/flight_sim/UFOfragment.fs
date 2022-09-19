#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_glow1;
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

in vec2 TexCoords;
in vec3 FragPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec3 TangentPointLightPos;
in vec3 TangentDirLightDir;

// uniforms
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform Material material;

// functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){
    vec3 norm = texture(material.texture_normal1, TexCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);

    vec3 result = vec3(0.0f);

    result += CalcDirLight(dirLight, norm, viewDir);
    result += CalcPointLight(pointLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-TangentDirLightDir);

    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));

    // diffuse   
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse;
    // check glow
    vec3 glowColor = texture(material.texture_glow1, TexCoords).rgb;
    if(glowColor.g > 0.4f) {
       diffuse = glowColor;
       ambient = vec3(0.0f);
    }
    else diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
 
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(TangentPointLightPos - TangentFragPos);
    
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords)); 
    
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // apply attenuation    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}