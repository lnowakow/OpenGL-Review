//
// Created by lukasz on 2021-12-10.
//
#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>

// Automatically resizes OpenGL render window with glfw window size
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// takes the window as input together with a key
void processInput(GLFWwindow *window);
// Checks if shader compilation was succesful
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
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader= glCreateShader(GL_VERTEX_SHADER);
    // attach shader source code and compile the shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Source - arg1: shader obj to compile to | arg2: how many strings in source | arg3: source obj
    glCompileShader(vertexShader);
    shaderProgramStatus(vertexShader, "VERTEX");

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    shaderProgramStatus(fragmentShader, "FRAGMENT");

    // Create shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // attach previously compiled shaders to the program and link them
    glAttachShader(shaderProgram, vertexShader); // order is critical
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    shaderProgramStatus(shaderProgram, "PROGRAM");
    // delete shader objects once they're linked. They're no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
             0.5f, 0.5f, 0.0f, // top right
             0.5f,-0.5f, 0.0f, // bottom right
            -0.5f,-0.5f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f // top left
    };
    unsigned int indices[] = {
            0,1,3, // first triangle
            1,2,3 // second triangle
    };
    // Concept of creating buffers is to load all this data into the GPU memory so we don't have slow load with CPU
    // during program execute

    unsigned int VAO, VBO, EBO; // Vertex Array Object (which will hold a VBO+EBO)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // ..:: Initialization code (done once (unless your object frequently changes)) ::..
    // 1. bind Vertex Array Object first
    glBindVertexArray(VAO);
    // 2. then bind and set vertex buffer(s) + Element Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind buffer ID to unique buffer type (this is for vertices)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy vertex data into buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // copy index data into buffer

    /* arg1: type of bufffer | arg2: size of data in bytes | arg3: data | arg4: how gpu manages data
     * GL_STREAM_DRAW: data set once, used maybe couple times
     * GL_STATIC_DRAW: data set once, used many times
     * GL_DYNAMIC_DRAW: data chages a lot, used many times
     */

    //  3. configure vertex attributes(s)
    // Tell GPU how to read vertex attribute (in this case vertex position since its the only attribute)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Viewport: tell OpenGL the size of the rendering window
    // first 2 args -> coordinates of lower left corner | last 2 args -> width and height, respectively
    glViewport(0,0,800,600);
    // processed coordinates in OpenGL are processed between -1 to 1.
    // width direction: (-1, 1) => (0,800) | OpenGL 0 -> pixel 400
    // height direction: (-1, 1) => (0,600) | OpenGL 0 -> pixel 300

    // Use GLFW window size callback to auto resize render window with glfw window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



    // Create render loop: each iteration of loop is called a "frame"
    while(!glfwWindowShouldClose(window)) {
        // start of frame -> clear the screen with rgba value
        // glClearColor is a state setting function
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // glClear is a state-using function
        glClear(GL_COLOR_BUFFER_BIT);

        // check if escape key has been pressed
        processInput(window);

        /*! rendering commands here... */
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

        // will swap the color buffer: a large 2D buffer that contains color values for each pixel in GLFW window
        glfwSwapBuffers(window);
        // Checks if any events have triggered
        glfwPollEvents();
    }

    // deallocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

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
