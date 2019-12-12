cmake_minimum_required(VERSION 3.12)
project(Task1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
add_subdirectory(third-party)


convertIntoHeader(shaders/FractalFragment.glsl include/shaders/FractalFragment.h fractalFragmentShader)
convertIntoHeader(shaders/FractalVertex.glsl include/shaders/FractalVertex.h fractalVertexShader)
convertIntoHeader(textures/texture.png include/textures/texure.h mainTexture)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_executable(Task1 main.cpp include/shaders/FractalFragment.h include/textures/texure include/shaders/FractalVertex.h  src/Shader.cpp include/Shader.h src/Fractal.cpp include/Fractal.h)



# Adding GLFW3
target_include_directories(Task1 PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(Task1 glfw ${OPENGL_LIBRARY} GLEW imgui stb_image )


