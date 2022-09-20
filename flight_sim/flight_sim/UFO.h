#ifndef UFO_H
#define UFO_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <iostream>

#include "Entity.h"

const float ACCELERATION_RATE = 4.0f;
const float DECELERATION_RATE = 10.0f;
const float MAX_VELOCITY = 20.0f;

const float TURN_SPEED_ACCELERATION = 45.0f;
const float TURN_SPEED_DECELERATION = 67.5f;
const float MAX_TURN_SPEED = 45.0f;

class UFO : public Entity {
public:
    // moving
    float yVelocity;
    float zVelocity;
    float yAcceleration;
    float zAcceleration;
    // turning
    float TurnSpeed;
    float TurnAcceleration;
    float Yaw = 90.0f;
    float Pitch = 0.0f;
    //is user influencing movement in a direction
    bool yInput;
    bool zInput;
    bool tInput; 

    // constructor
    UFO(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Entity(position, front, up, right, scale)
    {        
        yVelocity = 0.0f;
        zVelocity = 0.0f;
        yAcceleration = 0.0f;
        zAcceleration = 0.0f;
        TurnSpeed = 0.0f;
        TurnAcceleration = 0.0f;
        yInput = false;
        zInput = false;
        updateVectors();
    }
   
    // constructor with a pointer to the model
    UFO(Model* model, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Entity(model, position, front, up, right, scale)
    {
        yVelocity = 0.0f;
        zVelocity = 0.0f;
        yAcceleration = 0.0f;
        zAcceleration = 0.0f;
        TurnSpeed = 0.0f;
        TurnAcceleration = 0.0f;
        yInput = false;
        zInput = false;
        updateVectors();
    }

    // actions that happen once per key press
    void KeyboardCallback(GLFWwindow* window, int key, int action)
    {
        // key release actions
        if (key == GLFW_KEY_W && action == GLFW_RELEASE) determineReleaseAcceleration(zInput, zVelocity, zAcceleration);
        if (key == GLFW_KEY_S && action == GLFW_RELEASE) determineReleaseAcceleration(zInput, zVelocity, zAcceleration);
        if (key == GLFW_KEY_A && action == GLFW_RELEASE) determineTurnRelease(tInput, TurnSpeed, TurnAcceleration);
        if (key == GLFW_KEY_D && action == GLFW_RELEASE) determineTurnRelease(tInput, TurnSpeed, TurnAcceleration);
        if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) determineReleaseAcceleration(yInput, yVelocity, yAcceleration);
        if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) determineReleaseAcceleration(yInput, yVelocity, yAcceleration);
        if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) determineReleaseAcceleration(yInput, yVelocity, yAcceleration);
        // go down on arrow down
        if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
            yInput = true;
            yAcceleration = -ACCELERATION_RATE;
        }
        
        // key press actions
        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            zInput = true;
            zAcceleration = ACCELERATION_RATE;
        }
        if (key == GLFW_KEY_S && action == GLFW_PRESS) {
            zInput = true;
            zAcceleration = -ACCELERATION_RATE;
        }
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            yInput = true;
            yAcceleration = ACCELERATION_RATE;
        }
        if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
            yInput = true;
            yAcceleration = -ACCELERATION_RATE;
        }
        if (key == GLFW_KEY_A && action == GLFW_PRESS) tInput = true;
        if (key == GLFW_KEY_D && action == GLFW_PRESS) tInput = true;

    }

    // actions that happen as long as the key is pressed
    void ProcessKeyboard(int key) {
        if (key == GLFW_KEY_A) {
            if (zVelocity >= 0 && zAcceleration >= 0) TurnAcceleration = -TURN_SPEED_ACCELERATION;
            else if (zVelocity >= 0 && zAcceleration < 0) TurnAcceleration = TURN_SPEED_ACCELERATION;
            else if (zVelocity < 0 && zAcceleration < 0) TurnAcceleration = TURN_SPEED_ACCELERATION;
            else if (zVelocity < 0 && zAcceleration >= 0) TurnAcceleration = -TURN_SPEED_ACCELERATION;
        }
        if (key == GLFW_KEY_D) {
            if (zVelocity >= 0 && zAcceleration >= 0) TurnAcceleration = TURN_SPEED_ACCELERATION;
            else if (zVelocity >= 0 && zAcceleration < 0) TurnAcceleration = -TURN_SPEED_ACCELERATION;
            else if (zVelocity < 0 && zAcceleration < 0) TurnAcceleration = -TURN_SPEED_ACCELERATION;
            else if (zVelocity < 0 && zAcceleration >= 0) TurnAcceleration = TURN_SPEED_ACCELERATION;
        }
    }
    

	
	// action that blocks the user from moving in a direction
	void BlockMovement() {
		if (zVelocity > 0) {
			zVelocity = 0.0f;
			zAcceleration = 0.0f;
			zInput = false;
		}
		
	}

	//block movement in the y direction
	void BlockYMovement() {
		if (yVelocity < 0) {
			yVelocity = 0.0f;
			yAcceleration = 0.0f;
			yInput = false;
		}
	}

    void Move(float deltaTime, Entity entity[], Entity grass) {
		
		//print ufo posision
		std::cout << "UFO Position: " << Position.x << " " << Position.y << " " << Position.z << std::endl;
        for(int i = 0; i < 5; i++) {
            if(Position.x + 3.0f > entity[i].Position.x - 2.0f * entity[i].Scale.x
                && Position.x - 3.0f < entity[i].Position.x + 2.0f * entity[i].Scale.x
                && Position.y + 3.0f > entity[i].Position.y - 3.5f * entity[i].Scale.y
                && Position.y - 3.0f < entity[i].Position.y + 3.5f * entity[i].Scale.y
                && Position.z + 3.0f > entity[i].Position.z - 2.25f * entity[i].Scale.z
                && Position.z - 3.0f < entity[i].Position.z + 2.25f * entity[i].Scale.z)
            {
                //stop moving forward
                BlockMovement();
            }
		
        }
	
        

        // If ufo intersects grass block y movement down

        if(Position.x + 3.0f > grass.Position.x * grass.Scale.x
            && Position.x - 3.0f < grass.Position.x * grass.Scale.x
            && Position.y + 3.0f > grass.Position.y * grass.Scale.y
            && Position.y - 3.0f < grass.Position.y * grass.Scale.y
            && Position.z + 3.0f > grass.Position.z  * grass.Scale.z
            && Position.z - 3.0f < grass.Position.z * grass.Scale.z)
        {
            //stop moving forward
            BlockYMovement();
        }

        //calculate velocities
        zVelocity = calculateVelocity(zVelocity, zAcceleration, deltaTime, zInput);
        yVelocity = calculateVelocity(yVelocity, yAcceleration, deltaTime, yInput);
        TurnSpeed = calculateTurnSpeed(TurnSpeed, TurnAcceleration, deltaTime, tInput);

        // calculate rotation
        Yaw += calculateDistance(TurnSpeed, TurnAcceleration, deltaTime);
        updateVectors();
        // calculate movement
        Position += Front * calculateDistance(zVelocity, zAcceleration, deltaTime);
        Position += glm::vec3(0.0f, 1.0f, 0.0f) * calculateDistance(yVelocity, yAcceleration, deltaTime);
    }

private:

    float calculateVelocity(float velocity, float& acceleration, float deltaTime, bool input) {
        float new_velocity = velocity + acceleration * deltaTime;
        // accelerate up to max velocity
        if (input == true) {
            if (new_velocity > MAX_VELOCITY) return MAX_VELOCITY;
            else if (new_velocity < -MAX_VELOCITY) return -MAX_VELOCITY;
            return new_velocity;
        }
        // slow down to 0 if the user is not accelerating in this direction
        else {
            if (velocity > 0) {
                if (new_velocity < 0) {
                    acceleration = 0.0f;
                    return 0.0f;
                }
                else return new_velocity;
            }
            else if (velocity < 0) {
                if (new_velocity > 0) {
                    acceleration = 0.0f;
                    return 0.0f;
                }
                else return new_velocity;
            }
            else {
                acceleration = 0.0f;
                return 0.0f;
            }

        }

    }

    float calculateDistance(float velocity, float acceleration, float deltaTime) {
        float dist = velocity * deltaTime + acceleration * deltaTime * deltaTime * 0.5;
        return dist;
    }

    void determineReleaseAcceleration(bool& input, float& velocity, float& acceleration) {
        input = false;
        if (velocity > 0) acceleration = -DECELERATION_RATE;
        else if (velocity < 0) acceleration = DECELERATION_RATE;
        else acceleration = 0.0f;
    }
    

    // same as calculateVelocity but for turning
    float calculateTurnSpeed(float velocity, float& acceleration, float deltaTime, bool input) {
        float new_velocity = velocity + acceleration * deltaTime;
        if (input == true) {
            if (new_velocity > MAX_TURN_SPEED) return MAX_TURN_SPEED;
            else if (new_velocity < -MAX_TURN_SPEED) return -MAX_TURN_SPEED;
            return new_velocity;
        }
        else {
            if (velocity > 0) {
                if (new_velocity < 0) {
                    acceleration = 0.0f;
                    return 0.0f;
                }
                else return new_velocity;
            }
            else if (velocity < 0) {
                if (new_velocity > 0) {
                    acceleration = 0.0f;
                    return 0.0f;
                }
                else return new_velocity;
            }
            else {
                acceleration = 0.0f;
                return 0.0f;
            }

        }

    }
    // same as determineReleaseAcceleration but for turning
    void determineTurnRelease(bool& input, float& velocity, float& acceleration) {
        input = false;
        if (velocity > 0)acceleration = -TURN_SPEED_DECELERATION;
        else if (velocity < 0)acceleration = TURN_SPEED_DECELERATION;
        else acceleration = 0.0f;
    }

    void updateVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));
        Up = glm::normalize(glm::cross(Right, Front));
    }


};

#endif UFO_H