//
// Created by lukasz on 2021-12-11.
//

#include <iostream>
/*
 * Since there are different versions of OpenGL drivers, the location of most functions is not known at compile-time.
 * Needs to be queried at run-time.  Therefore, it's the task of the developer to locate them and store them as function
 * pointers. Retrieving the locations is OS-specific.  In comes GLAD.
 *
 * <GLAD> is an open source library that manages this task.  You download these files from https://glad.dav1d.de/
 * Linux: add .h files to /usr/include and .c to project folder
 * CMake: add_library(GLAD glad.c)
 *        target_link_libraries(CreatingAWindow GLAD)
 */
#include <glad.h>
/*
 * <glfw> is a library that contains the bare necessities for rendering objects on a screen.
 * Allows features like:
 *  > create an OpenGL context
 *  > define window parameters
 *  > handle user input
 *
 *  CMake: find_package(glfw3 3.3 REQUIRED)
 *         target_link_libraries(CreatingAWindow glfw)
 */
#include <GLFW/glfw3.h>


// Automatically resizes OpenGL render window with glfw window size
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// takes the window as input together with a key
void processInput(GLFWwindow *window);

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CreatingAWindow", NULL, NULL);
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

        // will swap the color buffer: a large 2D buffer that contains color values for each pixel in GLFW window
        glfwSwapBuffers(window);
        // Checks if any events have triggered
        glfwPollEvents();
    }

    // deallocate all resources
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

