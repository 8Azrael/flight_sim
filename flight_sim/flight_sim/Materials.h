#ifndef MATERIALS_H
#define MATERIALS_H

#include <glm/glm.hpp>
#include "shader.h"

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

void setupUFOMaterial(Material& material);
void setupHouseMaterial(Material& material);
void setMaterialUniforms(Shader shader, Material material);

#endif