#ifndef UFO_H
#define UFO_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <iostream>

#include "Entity.h"

const float ACCELERATION_RATE = 1.0f;
const float DECELERATION_RATE = 2.0f;
const float MAX_VELOCITY = 4.0f;
const float TURN_SPEED = 160.0f;

class UFO : public Entity {
public:
    float xVelocity;
    float yVelocity;
    float zVelocity;
    float xAcceleration;
    float yAcceleration;
    float zAcceleration;

    float currentTurnSpeed;

    //is user influencing movement in a direction
    bool xInput; 
    bool yInput;
    bool zInput;

    UFO(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Entity(position, front, up, right, scale)
    {
        xVelocity = 0.0f;
        yVelocity = 0.0f;
        zVelocity = 0.0f;
        xAcceleration = 0.0f;
        yAcceleration = 0.0f;
        zAcceleration = 0.0f;
        currentTurnSpeed = 0.0f;
        xInput = false;
        yInput = false;
        zInput = false;
    }

    UFO(Model* model, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : Entity(model, position, front, up, right, scale)
    {
        xVelocity = 0.0f;
        yVelocity = 0.0f;
        zVelocity = 0.0f;
        xAcceleration = 0.0f;
        yAcceleration = 0.0f;
        zAcceleration = 0.0f;
        currentTurnSpeed = 0.0f;
        xInput = false;
        yInput = false;
        zInput = false;
    }

    void ProcessKeyboardRelease(GLFWwindow* window, int key, int action)
    {
        if (key == GLFW_KEY_W && action == GLFW_RELEASE) determineReleaseAcceleration(zInput, zVelocity, zAcceleration);
        if (key == GLFW_KEY_S && action == GLFW_RELEASE) determineReleaseAcceleration(zInput, zVelocity, zAcceleration);
        if (key == GLFW_KEY_A && action == GLFW_RELEASE) determineReleaseAcceleration(xInput, xVelocity, xAcceleration);
        if (key == GLFW_KEY_D && action == GLFW_RELEASE) determineReleaseAcceleration(xInput, xVelocity, xAcceleration);
        if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) determineReleaseAcceleration(yInput, yVelocity, yAcceleration);
        if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) determineReleaseAcceleration(yInput, yVelocity, yAcceleration);

        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            zInput = true;
            zAcceleration = ACCELERATION_RATE;
        }
        if (key == GLFW_KEY_S && action == GLFW_PRESS) {
            zInput = true;
            zAcceleration = -ACCELERATION_RATE;
        }
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            xInput = true;
            xAcceleration = -ACCELERATION_RATE;
        }
        if (key == GLFW_KEY_D && action == GLFW_PRESS) {
            xInput = true;
            xAcceleration = ACCELERATION_RATE;
        }
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            yInput = true;
            yAcceleration = ACCELERATION_RATE;
        }
        if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
            yInput = true;
            yAcceleration = -ACCELERATION_RATE;
        }

    }

    //void ProcessKeyboardPress(GLFWwindow* window) {

    //    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    //        zInput = true;
    //        zAcceleration = ACCELERATION_RATE;
    //    }
    //    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    //        zInput = true;
    //        zAcceleration = -ACCELERATION_RATE;
    //    }
    //    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    //        xInput = true;
    //        xAcceleration = -ACCELERATION_RATE;
    //    }
    //    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    //        xInput = true;
    //        xAcceleration = ACCELERATION_RATE;
    //    }
    //    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    //        yInput = true;
    //        yAcceleration = ACCELERATION_RATE;
    //    }
    //    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    //        yInput = true;
    //        yAcceleration = -ACCELERATION_RATE;
    //    }
    //}

    void Move(float deltaTime) {
        //calculate velocities
        zVelocity = calculateVelocity(zVelocity, zAcceleration, deltaTime, zInput);
        yVelocity = calculateVelocity(yVelocity, yAcceleration, deltaTime, yInput);
        xVelocity = calculateVelocity(xVelocity, xAcceleration, deltaTime, xInput);

        //apply distance
        Position += glm::vec3(0.0f, 0.0f, -1.0f) * calculateDistance(zVelocity, zAcceleration, deltaTime);
        Position += glm::vec3(1.0f, 0.0f, 0.0f) * calculateDistance(xVelocity, xAcceleration, deltaTime);
        Position += glm::vec3(0.0f, 1.0f, 0.0f) * calculateDistance(yVelocity, yAcceleration, deltaTime);
    }

private:

    float calculateVelocity(float velocity, float& acceleration, float deltaTime, bool input) {
        float new_velocity = velocity + acceleration * deltaTime;
        if (input == true) {
            if (new_velocity > MAX_VELOCITY) return MAX_VELOCITY;
            else if (new_velocity < -MAX_VELOCITY) return -MAX_VELOCITY;
            return new_velocity;
        }
        // slow down to 0
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
        if (velocity > 0)acceleration = -DECELERATION_RATE;
        else if (velocity < 0)acceleration = DECELERATION_RATE;
        else acceleration = 0.0f;
    }

};

#endif UFO_H