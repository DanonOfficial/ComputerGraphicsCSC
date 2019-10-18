//
// Created by roundedglint585 on 10/5/19.
//

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <functional>
#include <stb_image.h>
#include "Shader.h"

class Fractal {
public:
    Fractal(uint32_t width, uint32_t height);

    void init();

    void run();

    ~Fractal();

private:
    void glfwInitialization();

    void callbackInitialization();

    void imguiInitialization();

    void loadVertices();

    void loadTextures();

    void render();

    void renderGUI();

    void mouseScrollCallback(double yoffset);

    void mouseButtonCallback(int button, int action);

    void cursorPositionCallback();

    void framebufferSizeCallback(int width, int height);

    glm::vec2 affineTransformToNDC(glm::vec2 vector);

private:
    GLFWwindow *window_;
    unsigned int VBO_, VAO_;
    unsigned int texture_;
    uint32_t width_, height_;
    float scale_ = 1.f;
    int iteration_ = 20;
    glm::vec2 center_ = {0.f, 0.f};
    Shader shader_;
    glm::dvec2 mousePosition_;
};



