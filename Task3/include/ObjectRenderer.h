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
#include "FreeCamera.h"
#include "ParticlesEmmiter.h"

class ObjectRenderer {
public:
    ObjectRenderer(uint32_t width, uint32_t height);
    void init();
    void loadObjectFromFile(std::string_view path);
    void loadObjectFromData(const std::string &data);
    void addEmmiter(uint32_t particlesAmount, const glm::vec3 &position);
    void run();
private:
    void glfwInitialization();
    void shaderInitialization();
    void framebufferSizeCallback(int width, int height);
    void callbackInitialization();
    void mousePosCallback(double xPos, double yPos);
    void keyboardCallback(int key, int action);
    void imguiInitialization();
    void renderGUI();
    glm::vec2 affineTransformToNDC(glm::vec2 vector);
private:
    GLFWwindow *window_;
    uint32_t width_, height_;
    Shader objectShader_;
    Shader particlesShader_;
    Mesh currentMesh_;
    glm::vec2 prevPos;
    FreeCamera camera_;
    std::vector<ParticlesEmmiter> emmiters_;
    bool isFirst_;
};


#endif //TASK1_OBJECTRENDERER_H
