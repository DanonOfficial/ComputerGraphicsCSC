//
// Created by roundedglint585 on 11/22/19.
//

#include "../include/ObjectRenderer.h"
#include "../include/ObjParser.h"
#include "../include/Camera.h"
#include "../include/shaders/ObjectRenderVertex.h"
#include "../include/shaders/ObjectRenderFragment.h"
#include <glm/gtc/quaternion.hpp>


ObjectRenderer::ObjectRenderer(uint32_t width, uint32_t height) : width_(width), height_(height), window_(nullptr), camera_(20.f, 1.f, 5.f){

}

void ObjectRenderer::init() {
    glfwInitialization();
    glewInit();
    shaderInitialization();
    callbackInitialization();
}

void ObjectRenderer::loadObjectFromFile(std::string_view path) {
    auto cube = objParser::parseFile(path);
    currentMesh_ = Mesh(std::get<1>(cube), std::get<0>(cube));
}
void ObjectRenderer::loadObjectFromData(const std::string &data) {
    auto cube = objParser::parseData(data);
    currentMesh_ = Mesh(std::get<1>(cube), std::get<0>(cube));
}


void ObjectRenderer::glfwInitialization() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window_ = glfwCreateWindow(width_, height_, "Object", nullptr, nullptr);
    glfwMakeContextCurrent(window_);
    glViewport(0, 0, width_, height_);
}

void ObjectRenderer::run() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    while (!glfwWindowShouldClose(window_)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 view_ = camera_.getView();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) width_ / (float) height_, 0.1f, 100.0f);
        shader_.use();
        shader_.setMat4("model", model);
        shader_.setMat4("view", view_ );
        shader_.setMat4("projection", projection);
        shader_.setVec3("lightColor", {1.0f, 1.0f, 1.0f});
        shader_.setVec3("lightPos", {4.f, 4.f, 6.f});
        currentMesh_.draw(shader_);
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void ObjectRenderer::shaderInitialization() {
    shader_ = Shader(objectRenderVertex, objectRenderFragment);
}

void ObjectRenderer::mouseScrollCallback(double yOffset) {
    if (yOffset == -1) {
        camera_.zoom(1.1);
    } else {
        camera_.zoom(0.9);
    }
}

void ObjectRenderer::callbackInitialization() {
    glfwSetWindowUserPointer(window_, reinterpret_cast<void *>(this));

    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow *window, int width, int height) {
        auto *objectRenderer = reinterpret_cast<ObjectRenderer *>(glfwGetWindowUserPointer(window));
        objectRenderer->framebufferSizeCallback(width, height);
    });

    glfwSetScrollCallback(window_, [](GLFWwindow *window, double xoffset, double yoffset) {
        auto *objectRenderer = reinterpret_cast<ObjectRenderer *>(glfwGetWindowUserPointer(window));
        objectRenderer->mouseScrollCallback(yoffset);
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow *window, double xpos, double ypos) {
        auto *objectRenderer = reinterpret_cast<ObjectRenderer *>(glfwGetWindowUserPointer(window));
        objectRenderer->mouseDragCallback();
    });

    glfwSetMouseButtonCallback(window_, [](GLFWwindow *window, int button, int action, int mods) {
        auto *objectRenderer = reinterpret_cast<ObjectRenderer *>(glfwGetWindowUserPointer(window));
        objectRenderer->mouseButtonCallback(button, action);
    });

}

void ObjectRenderer::framebufferSizeCallback(int width, int height) {
    width_ = width;
    height_ = height;
    glViewport(0, 0, width_, height_);
}


void ObjectRenderer::mouseDragCallback() {
    double x,y;
    glfwGetCursorPos(window_, &x, &y);
    if(isDragging){
        prevPos = curPos;
        curPos = {x,y};
        auto temp = -(curPos - prevPos)/300.f;
        camera_.rotate(temp.x, temp.y);
    }

}

void ObjectRenderer::mouseButtonCallback(int button, int action) {
    double x,y;
    glfwGetCursorPos(window_, &x, &y);

    if(((action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT))){
        isDragging = true;
        curPos = {x,y};
        prevPos = {x,y};
    }else{
        isDragging = false;
    }

}

