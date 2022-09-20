#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm/glm.hpp>
#include "shader.h"

struct DirLight {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLight {
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

void setupDirLight(DirLight& light);
void setupPointLight(PointLight& light);
void setDirLightUniforms(Shader shader, DirLight light);
void setPointLightUniforms(Shader shader, PointLight light);
#endif