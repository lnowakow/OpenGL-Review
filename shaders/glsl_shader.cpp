//
// Created by lukasz on 2021-12-12.
//

#include <iostream>
#include <math.h>
#include <glad.h>
#include <GLFW/glfw3.h>

#include "../Shader.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void shaderProgramStatus(const unsigned int &ID, const std::string &type);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "out vec4 vertexColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 // 1. "  vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                         "out vec4 FragColor;\n"
                                         //"in vec4 vertexColor;\n"
                                         "uniform vec4 ourColor;\n"
                                         "void main() {\n"
                                         // 1. "    FragColor = vertexColor;"
                                         "    FragColor = ourColor;"
                                         "}\0";
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

    //Shader ourShader("../shaders/vertex_shader.glsl", "../shaders/fragment_shader_tex.glsl");
    //Shader ex1Shader("../shaders/vertex_shader_ex1.glsl", "../shaders/fragment_shader_tex.glsl");
    //Shader ex2Shader("../shaders/vertex_shader_ex2.glsl", "../shaders/fragment_shader_tex.glsl");
    Shader ex3Shader("../shaders/vertex_shader_ex3.glsl", "../shaders/fragment_shader_ex3.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
            // position                   // colors
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top center
            0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f // bottom left
    };
    unsigned int VAO, VBO; // Vertex Array Object (which will hold a VBO+EBO)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind buffer ID to unique buffer type (this is for vertices)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy vertex data into buffer

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);


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

        /*! using shader uniform to change color over time */
        /*
        float greenValue = (sin(glfwGetTime())/ 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        */

        /*! using Shader Class */
        // Learning Shader
        //ourShader.use();
        // Example 1
        //ex1Shader.use();
        /* Example 2
        ex2Shader.use();
        float xOffset = 0.33f*sin(glfwGetTime()/1.0f);
        ex2Shader.setFloat("x_offset", xOffset);
         */
        ex3Shader.use();
        float xOffset = 0.33f*sin(glfwGetTime()/0.1f);
        ex3Shader.setFloat("x_offset", xOffset);

        /*! bind the buffer and draw the shae you want... */
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // will swap the color buffer: a large 2D buffer that contains color values for each pixel in GLFW window
        glfwSwapBuffers(window);
        // Checks if any events have triggered
        glfwPollEvents();
    }

    // deallocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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

