#ifndef ENTITY_H
#define ENTITY_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model.h"

class Entity {
	public:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 Scale;
		Model* EntityModel;

		Entity(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) {
			EntityModel = nullptr;
			Position = position;
			Front = front;
			Up = up;
			Right = right;
			Scale = scale;
		};

		// constructor with model
		Entity(Model* model, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) {
			EntityModel = model;
			Position = position;
			Front = front;
			Up = up;
			Right = right;
			Scale = scale;
		};

		void Draw(Shader& shader) {
			EntityModel->Draw(shader);
		}
		
};

#endif