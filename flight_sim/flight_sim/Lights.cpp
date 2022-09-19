#include "Lights.h"

void setupDirLight(DirLight& light){
	light.direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	light.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	light.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

void setDirLightUniforms(Shader shader, DirLight light) {
	shader.setVec3("dirLight.ambient", light.ambient);
	shader.setVec3("dirLight.diffuse", light.diffuse);
	shader.setVec3("dirLight.specular", light.specular);
	shader.setVec3("dirLight.direction", light.direction);
}

void setupPointLight(PointLight& light) {
	light.position = glm::vec3(0.0f, 50.0f, 0.0f);
	light.ambient = glm::vec3(0.05f, 0.0f, 0.0f);
	light.diffuse = glm::vec3(0.8f, 0.0f, 0.0f);
	light.specular = glm::vec3(1.0f, 0.0f, 0.0f);
	light.constant = 1.0f;
	light.linear = 0.014f;
	light.quadratic = 0.0007f;
}

void setPointLightUniforms(Shader shader, PointLight light) {
	shader.setVec3("pointLight.position", light.position);
	shader.setVec3("pointLight.ambient", light.ambient);
	shader.setVec3("pointLight.diffuse", light.diffuse);
	shader.setVec3("pointLight.specular", light.specular);
	shader.setFloat("pointLight.constant", light.constant);
	shader.setFloat("pointLight.linear", light.linear);
	shader.setFloat("pointLight.quadratic", light.quadratic);

}