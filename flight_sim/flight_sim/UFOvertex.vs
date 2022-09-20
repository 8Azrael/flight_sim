#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

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

out vec2 TexCoords;
out vec3 FragPos;

// normal mapping 
out vec3 TangentPointLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec3 TangentDirLightDir;

// light
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform vec3 viewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));   
    TexCoords = aTexCoords;
    
    // calculate TBN vectors
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));

    TangentPointLightPos = TBN * pointLight.position;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;
    TangentDirLightDir = TBN * dirLight.direction;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}