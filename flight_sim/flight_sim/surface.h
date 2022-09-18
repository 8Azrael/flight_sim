#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"
#include "shader.h"

class Surface {
	public:
        unsigned int VAO;
        unsigned int texture1;

        float vertices[32] = {
            // positions          // colors           // texture coords
             50.0f,  0.0f,  50.0f,   1.0f, 0.0f, 0.0f,   32.0f, 32.0f, // top right
             50.0f,  0.0f, -50.0f,   0.0f, 1.0f, 0.0f,   32.0f, 0.0f, // bottom right
            -50.0f,  0.0f, -50.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -50.0f,  0.0f,  50.0f,   1.0f, 1.0f, 0.0f,   0.0f, 32.0f  // top left 
        };
        unsigned int indices[6] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        Surface() {
            SetupSurface();
            loadTexture();
        }		

        void Draw(Shader& shader) {                   
            glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
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
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            // texture coord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

        }

        void loadTexture() {
            glGenTextures(1, &texture1);
            glBindTexture(GL_TEXTURE_2D, texture1);
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
            unsigned char* data = stbi_load("resources/textures/grass/grass.jpg", &width, &height, &nrChannels, 0);
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
        }
};