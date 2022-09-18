#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_glow1;
uniform vec3 viewPos;

void main()
{       
    // light properties
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightPos = vec3(0.0f, 100.0f, 0.0f);
    float ambientStrength = 0.1f;
    float diffuseStrength = 0.5f;
    float specularStrength = 1.0f;
    float glowStrength = 0.0f;
    float shininess = 1.0f;

    // pixel color from texture
    vec3 objectColor = texture(texture_diffuse1, TexCoords).rgb;    
    vec3 glowColor = texture(texture_glow1, TexCoords).rgb;

    if(any(greaterThan(glowColor, vec3(0.4f, 0.4f, 0.4f)))) {
        ambientStrength = 0.0f;
        diffuseStrength = 0.0f;
        specularStrength = 0.7f;
        glowStrength = 1.0f;
    }

    // ambient    
    vec3 ambient = ambientStrength * objectColor;
  	
    // diffuse     
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * objectColor * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * texture(texture_specular1, TexCoords).rgb;  
    
    //glow
    vec3 glow = glowStrength * glowColor;

    vec3 result = ambient + diffuse + specular + glow;
    FragColor = vec4(result, 1.0);
}