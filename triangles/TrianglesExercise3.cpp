//
// Created by lukasz on 2021-12-11.
//

#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void shaderProgramStatus(const unsigned int &ID, const std::string &type);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main() {\n"
                                   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
                                   "}\0";

const char *yellowFragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main() {\n"
                                   "    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);"
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

    // build and compile our shader program
    // vertex shader
    unsigned int vertexShader= glCreateShader(GL_VERTEX_SHADER);
    // attach shader source code and compile the shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Source - arg1: shader obj to compile to | arg2: how many strings in source | arg3: source obj
    glCompileShader(vertexShader);
    shaderProgramStatus(vertexShader, "VERTEX");

    // orange fragment shader
    unsigned int orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(orangeFragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(orangeFragmentShader);
    shaderProgramStatus(orangeFragmentShader, "FRAGMENT");

    // yellow fragment shader
    unsigned int yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSource, NULL);
    glCompileShader(yellowFragmentShader);
    shaderProgramStatus(yellowFragmentShader, "FRAGMENT");

    // Create orange shader program
    unsigned int orangeShaderProgram;
    orangeShaderProgram = glCreateProgram();
    glAttachShader(orangeShaderProgram, vertexShader); // order is critical
    glAttachShader(orangeShaderProgram, orangeFragmentShader);
    glLinkProgram(orangeShaderProgram);
    shaderProgramStatus(orangeShaderProgram, "PROGRAM");
    // Create yellow shader program
    unsigned int yellowShaderProgram;
    yellowShaderProgram = glCreateProgram();
    glAttachShader(yellowShaderProgram, vertexShader); // order is critical
    glAttachShader(yellowShaderProgram, yellowFragmentShader);
    glLinkProgram(yellowShaderProgram);
    shaderProgramStatus(yellowShaderProgram, "PROGRAM");
    // delete shader objects once they're linked. They're no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(orangeFragmentShader);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    float triangleOne[] = {
            0.5f, 0.5f, 0.0f, // top right
            0.5f,-0.5f, 0.0f, // bottom right
            0.0f,0.0f, 0.0f // center
    };
    float triangleTwo[] = {
            -0.5f, 0.5f, 0.0f, // top left
            -0.5f,-0.5f, 0.0f, // bottom left
            0.0f, 0.0f, 0.0f // cetner
    };
    unsigned int VAOs[2], VBOs[2]; // Vertex Array Object (which will hold a VBO+EBO)
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    // Bind VAO1
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // bind buffer ID to unique buffer type (this is for triangleOne)
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleOne), triangleOne, GL_STATIC_DRAW); // copy vertex data into buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Bind VAO2
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleTwo), triangleTwo, GL_STATIC_DRAW); // copy vertex data into buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // de-bind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



    // Create render loop: each iteration of loop is called a "frame"
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        /*! rendering commands here... */
        glUseProgram(orangeShaderProgram);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(yellowShaderProgram);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // will swap the color buffer: a large 2D buffer that contains color values for each pixel in GLFW window
        glfwSwapBuffers(window);
        // Checks if any events have triggered
        glfwPollEvents();
    }

    // deallocate all resources
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(orangeShaderProgram);

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

