cmake_minimum_required(VERSION 3.12)
project(Task2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
add_subdirectory(third-party)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_executable(Task2 main.cpp src/Shader.cpp include/Shader.h src/ObjectRenderer.cpp include/ObjectRenderer.h include/ObjParser.h src/Mesh.cpp include/Mesh.h src/Camera.cpp include/Camera.h)

# Adding GLFW3
target_include_directories(Task2 PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(Task2 glfw ${OPENGL_LIBRARY} GLEW imgui stb_image)

