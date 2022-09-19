#include "Materials.h"

void setupUFOMaterial(Material& material) {
	material.shininess = 0.4f * 32;
}

void setupHouseMaterial(Material& material) {
	material.specular = glm::vec3(0.296648f, 0.296648f, 0.296648f);
	material.shininess = 0.088 * 32;
}

void setMaterialUniforms(Shader shader, Material material) {
	shader.setVec3("material.ambient", material.ambient);
	shader.setVec3("material.diffuse", material.diffuse);
	shader.setVec3("material.specular", material.specular);
	shader.setFloat("material.shininess", material.shininess);
}