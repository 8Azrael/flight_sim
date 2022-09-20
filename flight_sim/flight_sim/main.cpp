#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "3rdPersonCamera.h"
#include "model.h"
#include "UFO.h"
#include "surface.h"
#include "Materials.h"
#include "Lights.h"

#include <iostream>

#define PI 3.14159265358979323846

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// UFO
UFO UFO1(glm::vec3(0.0f, 0.0f, 0.0f));


// camera
Camera camera(&UFO1);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // initialize and configure glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // enable mouse capturing
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    Shader UFOShader("UFOvertex.vs", "UFOfragment.fs");
    Shader GrassShader("Grass.vs", "Grass.fs");
    Shader HouseShader("House.vs", "House.fs");
    Shader SphereShader("Sphere.vs", "Sphere.fs");

    // setup materials
    Material UFOMaterial;
    setupUFOMaterial(UFOMaterial);
    Material HouseMaterial;
    setupHouseMaterial(HouseMaterial);
    // setup lights
    DirLight Sun;
    setupDirLight(Sun);
    PointLight Sphere;
    setupPointLight(Sphere);

    // Sphere
    Entity SphereEntity(Sphere.position);
    // Surface
    Surface Grass;

    // load models
    Model UFOModel("resources/objects/UFO/UFO.obj");
    Model House("resources/objects/House2/house2.obj");
    Model SphereModel("resources/objects/Sphere/sphere.obj");

    // bind objects with models
    UFO1.EntityModel = &UFOModel;
    SphereEntity.EntityModel = &SphereModel;


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // clear window
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        

        // view/projection transformations
        camera.calculatePosition();
        glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();          

        // enable shader before setting uniforms
        UFOShader.use();
        UFOShader.setMat4("projection", projection);
        UFOShader.setMat4("view", view);

        //Entity House1(glm::vec3(-31.5f, 5.5f, 15.0f));
        

        Entity House1(glm::vec3(-9.0f, 1.2f, 14.5f),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        Entity House2(glm::vec3(20.0f, 4.0f, 15.0f),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));
        Entity House3(glm::vec3(-30.5f, 5.5f, 14.5f), glm::vec3(0.0f, 0.0f, -1.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(3.1f, 3.1f, 3.1f));
        Entity House4(glm::vec3(9.0f, 1.2f, -14.5f),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        Entity House5(glm::vec3(-20.0f, 4.0f, -15.0f),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));

        

        Entity Houses[5] = {
            House1,
            House2,
            House3,
            House4,
            House5,
            };
		
		

        // render the loaded model
		UFO1.Move(deltaTime, Houses);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, UFO1.Position);
        model = glm::rotate(model, -glm::radians(UFO1.Yaw), UFO1.Up);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        
        UFOShader.use();
        UFOShader.setMat4("projection", projection);
        UFOShader.setMat4("view", view);
        UFOShader.setMat4("model", model);
        UFOShader.setVec3("viewPos", camera.Position);
        setMaterialUniforms(UFOShader, UFOMaterial);
        setPointLightUniforms(UFOShader, Sphere);
        setDirLightUniforms(UFOShader, Sun);        
        
        UFO1.Draw(UFOShader);

        // render surface
        glm::mat4 grassModel = glm::mat4(1.0f);
        
        GrassShader.use();
        GrassShader.setMat4("model", grassModel);
        GrassShader.setMat4("projection", projection);
        GrassShader.setMat4("view", view);
        GrassShader.setVec3("viewPos", camera.Position);
        setPointLightUniforms(GrassShader, Sphere);
        setDirLightUniforms(GrassShader, Sun);
        
        Grass.Draw(GrassShader);

        // render sphere
        glm::mat4 sphereModel = glm::mat4(1.0f);
        sphereModel = glm::translate(sphereModel, SphereEntity.Position);
        sphereModel = glm::scale(sphereModel, glm::vec3(1.0f, 1.0f, 1.0f));

        SphereShader.use();
        SphereShader.setMat4("model", sphereModel);
        SphereShader.setMat4("projection", projection);
        SphereShader.setMat4("view", view);
        SphereShader.setVec3("aColor", Sphere.diffuse);

        SphereEntity.Draw(SphereShader);

        SphereShader.use();
        SphereShader.setMat4("model", sphereModel);
        SphereShader.setMat4("projection", projection);
        SphereShader.setMat4("view", view);
        SphereShader.setVec3("aColor", Sphere.diffuse);

        SphereEntity.Draw(SphereShader);

        HouseShader.use();
        // render houses
        for (int i = 1; i <= 3; i++)
        {
            glm::mat4 model4 = glm::mat4(1.0f);
            model4 = glm::translate(model4, glm::vec3(1.0f * (i % 2 ? -i : i) * 10, 1.5f + i, 15.0f));
            model4 = glm::scale(model4, glm::vec3(1.0f * i, 1.0f * i, 1.0f * i));
            model4 = glm::rotate(model4, glm::radians(90.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
            HouseShader.setMat4("model", model4);
            HouseShader.setMat4("projection", projection);
            HouseShader.setMat4("view", view);
            HouseShader.setVec3("viewPos", camera.Position);
            setMaterialUniforms(HouseShader, HouseMaterial);
            setPointLightUniforms(HouseShader, Sphere);
            setDirLightUniforms(HouseShader, Sun);
            House.Draw(HouseShader);
        }

        for (int i = 1; i < 3; i++)
        {
            glm::mat4 model4 = glm::mat4(1.0f);
            model4 = glm::translate(model4, glm::vec3(-2.0f * (i % 2 ? -i : i) * 5, 1.5f * i, -15.0f));
            model4 = glm::scale(model4, glm::vec3(1.0f * i, 1.0f * i, 1.0f * i));
            model4 = glm::rotate(model4, glm::radians(90.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
            HouseShader.setMat4("model", model4);
            HouseShader.setMat4("projection", projection);
            HouseShader.setMat4("view", view);
            HouseShader.setVec3("viewPos", camera.Position);
            setMaterialUniforms(HouseShader, HouseMaterial);
            setPointLightUniforms(HouseShader, Sphere);
            setDirLightUniforms(HouseShader, Sun);
            House.Draw(HouseShader);
        }

        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clear all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// actions that happen as long as the key is pressed
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);   
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        UFO1.ProcessKeyboard(GLFW_KEY_A);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        UFO1.ProcessKeyboard(GLFW_KEY_D);
    
}

// actions that happen once per key press
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    UFO1.KeyboardCallback(window, key, action);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.calculatePitch(yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.calculateDistance(static_cast<float>(yoffset));
}

