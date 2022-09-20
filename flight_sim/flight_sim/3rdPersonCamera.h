#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "UFO.h"

#define PI 3.14159265358979323846

const float MOUSE_SENSITIVITY = 0.1f;

class Camera {
	public:		
		UFO* Ufo; // pointer to UFO object that camera should follow
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		float Distance = 10; // distance between camera and UFO
		float Pitch;
		float Yaw;

		float MouseSensitivity = MOUSE_SENSITIVITY;

		// constructor expecting pointer to UFO object
		Camera(UFO* ufo) {
			Ufo = ufo;
			Position = ufo->Position;
			Pitch = ufo->Pitch - 20;
			Yaw = ufo->Yaw;
			updateVectors();
		}

		void calculatePosition() {			
			Yaw = Ufo->Yaw;
			Position = Ufo->Position;
			updateVectors();
			Position -= Front * Distance;						
		}

		glm::mat4 GetViewMatrix()
		{	
			return glm::lookAt(Position, Ufo->Position, Up);
		}

		void calculateDistance(float yoffset)
		{
			Distance -= (float)yoffset;
			if (Distance < 5.0f)
				Distance = 5.0f;
			if (Distance > 50.0f)
				Distance = 50.0f;
		}

		void calculatePitch(float yoffset, GLboolean constrainPitch = true)
		{
			Pitch += yoffset * MouseSensitivity;

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch)
			{
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}
		}


	private:
		
		void updateVectors()
		{
			// calculate the new Front vector
			glm::vec3 front;
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(front);
			// also re-calculate the Right and Up vector
			Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = glm::normalize(glm::cross(Right, Front));
		}


};	

#endif