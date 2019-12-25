cmake_minimum_required(VERSION 3.12)
project(Task2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
add_subdirectory(third-party)


convertIntoHeader(shaders/ObjectRenderFragment.glsl include/shaders/ObjectRenderFragment.h objectRenderFragment)
convertIntoHeader(shaders/ObjectRenderVertex.glsl include/shaders/ObjectRenderVertex.h objectRenderVertex)
convertIntoHeader(shaders/ParticlesFragment.glsl include/shaders/ParticlesFragment.h particlesFragment)
convertIntoHeader(shaders/ParticlesVertex.glsl include/shaders/ParticlesVertex.h particlesVertex)
convertIntoHeader(objects/cube.obj include/objects/cube.h cube)
convertIntoHeader(objects/sponza.obj include/objects/sponza.h sponza)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)


add_executable(Task3 main.cpp src/Shader.cpp  include/Shader.h src/ObjectRenderer.cpp include/ObjectRenderer.h include/ObjParser.h src/Mesh.cpp include/Mesh.h
        include/shaders/ObjectRenderFragment.h include/shaders/ObjectRenderVertex.h include/shaders/ParticlesFragment.h include/shaders/ParticlesVertex.h include/objects/cube.h include/FreeCamera.h src/FreeCamera.cpp
        include/Particle.h include/ParticlesEmmiter.h src/ParticlesEmmiter.cpp )

# Adding GLFW3
target_include_directories(Task3 PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(Task3 glfw ${OPENGL_LIBRARY} GLEW imgui stb_image glm)

