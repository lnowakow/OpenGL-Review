cmake_minimum_required(VERSION 3.21)
project(openGL_Review)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_library(GLAD glad.c)

# Hello World Executable
add_executable(HelloWorld HelloWorld.cpp)
target_link_libraries(HelloWorld ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

# CreatingAWindow Executable
add_executable(CreatingAWindow CreatingAWindow.cpp)
target_link_libraries(CreatingAWindow ${OPENGL_LIBRARIES} glfw GLAD)

## TRIANGLES
# Shaders_Vertices_Elements Executable
add_executable(Shaders_Vertices_Elements triangles/Shaders_Vertices_Elements.cpp)
target_link_libraries(Shaders_Vertices_Elements ${OPENGL_LIBRARIES} glfw GLAD)

# TrianglesExercise1 Executable
add_executable(TrianglesExercise1 triangles/TrianglesExercise1.cpp)
target_link_libraries(TrianglesExercise1 ${OPENGL_LIBRARIES} glfw GLAD)
# TrianglesExercise2 Executable
add_executable(TrianglesExercise2 triangles/TrianglesExercise2.cpp)
target_link_libraries(TrianglesExercise2 ${OPENGL_LIBRARIES} glfw GLAD)
# TrianglesExercise3 Executable
add_executable(TrianglesExercise3 triangles/TrianglesExercise3.cpp)
target_link_libraries(TrianglesExercise3 ${OPENGL_LIBRARIES} glfw GLAD)

## SHADERS
add_executable(glsl_shader shaders/glsl_shader.cpp)
target_link_libraries(glsl_shader ${OPENGL_LIBRARIES} glfw GLAD)

## TEXTURES
add_executable(textures textures/textures.cpp)
target_link_libraries(textures ${OPENGL_LIBRARIES} glfw GLAD)
# texture_example1
add_executable(textures_ex1 textures/textures_ex1.cpp)
target_link_libraries(textures_ex1 ${OPENGL_LIBRARIES} glfw GLAD)
# texture_example2
add_executable(textures_ex2 textures/textures_ex2.cpp)
target_link_libraries(textures_ex2 ${OPENGL_LIBRARIES} glfw GLAD)
# texture_example3
add_executable(textures_ex3 textures/textures_ex3.cpp)
target_link_libraries(textures_ex3 ${OPENGL_LIBRARIES} glfw GLAD)

## TRANSFORVMATIONS
add_executable(transformations transformations/transformations.cpp)
target_link_libraries(transformations ${OPENGL_LIBRARIES} glfw GLAD)
# transforms_ex
add_executable(transforms_ex transformations/transforms_ex.cpp)
target_link_libraries(transforms_ex ${OPENGL_LIBRARIES} glfw GLAD)

## COORDINATE SYSTEMS
add_executable(coordsys coordinate-systems/coordsys.cpp)
target_link_libraries(coordsys ${OPENGL_LIBRARIES} glfw GLAD)
# coordinate moving box with arrows
add_executable(coordsys_ex2 coordinate-systems/coordsys_ex2.cpp)
target_link_libraries(coordsys_ex2 ${OPENGL_LIBRARIES} glfw GLAD)

## CAMERAS
add_executable(cameras cameras/cameras.cpp)
target_link_libraries(cameras ${OPENGL_LIBRARIES} glfw GLAD)
# cameras_wsad
add_executable(cameras_wsad cameras/cameras_wsad.cpp)
target_link_libraries(cameras_wsad ${OPENGL_LIBRARIES} glfw GLAD)
