#ifndef SURFACE_H
#define SURFACE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"
#include "shader.h"

class Surface {
	public:
        unsigned int VAO;
        unsigned int texture_diffuse;
        unsigned int texture_specular;
        unsigned int texture_normal;
        unsigned int texture_height;

        float vertices[32] = {
            // positions             // normals         // texture coords
             100.0f,  0.0f,  100.0f, 0.0f, 1.0f, 0.0f,  128.0f, 128.0f, // top right
             100.0f,  0.0f, -100.0f, 0.0f, 1.0f, 0.0f,  128.0f, 0.0f, // bottom right
            -100.0f,  0.0f, -100.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, // bottom left
            -100.0f,  0.0f,  100.0f, 0.0f, 1.0f, 0.0f,  0.0f, 128.0f  // top left 
        };
        unsigned int indices[6] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        Surface() {
            SetupSurface();
            texture_diffuse = loadTexture("resources/textures/grass/diffuse.jpg");
            texture_specular = loadTexture("resources/textures/grass/specular.jpg");
            /*loadTexture("resources/textures/grass/normal.jpg", texture_normal);
            loadTexture("resources/textures/grass/height.jpg", texture_height);*/
        }		

        void Draw(Shader& shader) {  
            shader.use();

            glActiveTexture(GL_TEXTURE0);
            glUniform1i(glGetUniformLocation(shader.ID, "material.texture_diffuse1"), 0);
            glBindTexture(GL_TEXTURE_2D, texture_diffuse);

            glActiveTexture(GL_TEXTURE1);
            glUniform1i(glGetUniformLocation(shader.ID, "material.texture_specular1"), 0);
            glBindTexture(GL_TEXTURE_2D, texture_specular);

            //glActiveTexture(GL_TEXTURE2);
            //glUniform1i(glGetUniformLocation(shader.ID, "texture_normal1"), 2);

            //glActiveTexture(GL_TEXTURE3);
            //glUniform1i(glGetUniformLocation(shader.ID, "texture_height1"), 3);
            //glActiveTexture(GL_TEXTURE0);
            
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

    private:
        unsigned int VBO, EBO;

        void SetupSurface() {
            
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // position attribute
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            // normal attribute
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            // texture coord attribute
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        }

        unsigned int loadTexture(const char* path) {
            unsigned int textureID;

            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16);
            // load image, create texture and generate mipmaps
            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }
            stbi_image_free(data);

            return textureID;
        }
};

#endif