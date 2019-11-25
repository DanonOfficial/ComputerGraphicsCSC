//
// Created by roundedglint585 on 11/22/19.
//

#ifndef TASK1_OBJECTRENDERER_H
#define TASK1_OBJECTRENDERER_H


#include <cstdint>
#include <string_view>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/detail/type_quat.hpp>
#include "Mesh.h"
#include "Camera.h"

class ObjectRenderer {
public:
    ObjectRenderer(uint32_t width, uint32_t height);
    void init();
    void loadObject(std::string_view path);
    void run();
private:
    void glfwInitialization();
    void shaderInitialization();
    void mouseScrollCallback(double yOffset);
    void framebufferSizeCallback(int width, int height);
    void callbackInitialization();
    void mouseDragCallback();
    void mouseButtonCallback(int button, int action);
    glm::vec2 affineTransformToNDC(glm::vec2 vector);
private:
    GLFWwindow *window_;
    uint32_t width_, height_;
    Shader shader_;
    Mesh currentMesh_;
    Camera camera_;
    glm::dvec2 mousePosition_;
    glm::quat tranQuat_;
};


#endif //TASK1_OBJECTRENDERER_H
