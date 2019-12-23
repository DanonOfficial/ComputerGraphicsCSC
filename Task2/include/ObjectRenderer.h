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
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "Mesh.h"
#include "Camera.h"

class ObjectRenderer {
public:
    ObjectRenderer(uint32_t width, uint32_t height);
    void init();
    void loadObjectFromFile(std::string_view path);
    void loadObjectFromData(const std::string &data);
    void run();
private:
    void glfwInitialization();
    void shaderInitialization();
    void mouseScrollCallback(double yOffset);
    void framebufferSizeCallback(int width, int height);
    void callbackInitialization();
    void mouseDragCallback();
    void mouseButtonCallback(int button, int action);
    void imguiInitialization();
    void renderGUI();
private:
    GLFWwindow *window_;
    uint32_t width_, height_;
    Shader shader_;
    Mesh currentMesh_;
    glm::vec2 prevPos, curPos;
    Camera camera_;
    bool isDragging = false;
    float dissolveThreshold_;
};


#endif //TASK1_OBJECTRENDERER_H
