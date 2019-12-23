cmake_minimum_required(VERSION 3.12)
project(Task2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
add_subdirectory(third-party)


convertIntoHeader(shaders/ObjectRenderFragment.glsl include/shaders/ObjectRenderFragment.h objectRenderFragment)
convertIntoHeader(shaders/ObjectRenderVertex.glsl include/shaders/ObjectRenderVertex.h objectRenderVertex)
convertIntoHeader(objects/cube.obj include/objects/cube.h cube)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)


add_executable(Task2 main.cpp src/Shader.cpp  include/Shader.h src/ObjectRenderer.cpp include/ObjectRenderer.h include/ObjParser.h src/Mesh.cpp include/Mesh.h
        include/shaders/ObjectRenderFragment.h include/shaders/ObjectRenderVertex.h include/objects/cube.h include/Camera.h src/Camera.cpp)

# Adding GLFW3
target_include_directories(Task2 PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(Task2 glfw ${OPENGL_LIBRARY} GLEW imgui stb_image glm)

