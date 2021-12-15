//
// Created by lukasz on 2021-12-12.
//

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <math.h>
#include <glad.h>
#include <GLFW/glfw3.h>

#include "../Shader.h"
#include "../stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void shaderProgramStatus(const unsigned int &ID, const std::string &type);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char** argv) {

    // Instatiate GLFW Window
    glfwInit();
    // We want to use glfw version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window object
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Initialize GLAD so we can use OpenGL functions
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    // SHADER
    // ex1
    Shader ourShader("../shaders/texture_shader.glsl", "../shaders/fragment_shader_tex_ex1.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
            // position                      // colors                        // texture1 coords
              0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f,// top right
             -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top left
             0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,// bottom right
            -0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f // bottom left
    };
    unsigned int indices[] = {
            0,2,3,
            3,1,0
    };
    unsigned int VAO, VBO, EBO; // Vertex Array Object (which will hold a VBO+EBO)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind buffer ID to unique buffer type (this is for vertices)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy vertex data into buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind buffer ID to unique buffer type (this is for indices)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // copy index data into buffer
    // vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // shader color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture1 attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // TEXTURE
    // Create texture1
    unsigned int texture1; // texture1 ID
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture1 wrapping/filtering options (on currently bound texture1)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image for to be used as texture1
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture1..." << std::endl;
    }
    // Create texture1 2
    unsigned int texture2; // texture1 ID
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture1 wrapping/filtering options (on currently bound texture1)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image for to be used as texture1
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture1..." << std::endl;
    }
    stbi_image_free(data); // free image data from memory

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // The number of attributes we can have in a vertex
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum number of vertex attributes: " << nrAttributes << std::endl; // output: 16

    // Create render loop: each iteration of loop is called a "frame"
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        /*! using Shader Class */
        // Learning Shader
        ourShader.use();
        ourShader.setInt("texture1", 0);
        ourShader.setInt("texture2", 1);
        glActiveTexture(GL_TEXTURE0); // this 0 matches the int value in setInt
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1); // this 1 matches the int value in setInt
        glBindTexture(GL_TEXTURE_2D, texture2);

        /*! bind the buffer and draw the shape you want... */
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // will swap the color buffer: a large 2D buffer that contains color values for each pixel in GLFW window
        glfwSwapBuffers(window);
        // Checks if any events have triggered
        glfwPollEvents();
    }

    // deallocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // terminate GLFW
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    /// Automatically resizes OpenGL render window with glfw window size
    glViewport(0, 0, width, height);
    printf("Window resized to (%i, %i)\n", width, height);
}

void processInput(GLFWwindow *window) {
    /// Takes the window as input together with a key
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void shaderProgramStatus(const unsigned int &ID, const std::string &type) {
    ///
    /// Check if compile was successful
    int success;
    char infoLog[512];
    if (type == "VERTEX" || type == "FRAGMENT") {
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    } else if (type == "PROGRAM") {
        glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << type << "::LINKING_FAILED\n" << infoLog << std::endl;
        }

    }
}

