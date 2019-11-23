//
// Created by roundedglint585 on 11/22/19.
//

#include "../include/ObjectRenderer.h"
#include "../include/ObjParser.h"
#include "../include/Camera.h"

ObjectRenderer::ObjectRenderer(uint32_t width, uint32_t height): width_(width), height_(height), window_(nullptr){

}

void ObjectRenderer::init() {
    glfwInitialization();
    glewInit();
    shaderInitialization();
    camera_ = Camera({1.0f, 0.0f, 0.f});
    callbackInitialization();
}

void ObjectRenderer::loadObject(std::string_view path) {
    auto teapot = objParser::parseFile(path);
    currentMesh_ = Mesh(std::get<1>(teapot), std::get<0>(teapot));
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
    glm::mat4 model = glm::mat4(1.0f);
    while (!glfwWindowShouldClose(window_)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 view =  camera_.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) width_ / (float) height_, 0.1f, 100.0f);
        shader_.use();
        shader_.setMat4("model", model);
        shader_.setMat4("view", view);
        shader_.setMat4("projection", projection);
        shader_.setVec3("lightColor", {1.0f,1.0f,1.0f});
        shader_.setVec3("lightPos", camera_.getPos());
        currentMesh_.draw(shader_);
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void ObjectRenderer::shaderInitialization() {
    shader_=Shader("shaders/ObjectRenderVertex.glsl", "shaders/ObjectRenderFragment.glsl");
}

void ObjectRenderer::mouseScrollCallback(double yOffset) {
        glm::dvec2 mousePos;
        glfwGetCursorPos(window_, &mousePos.x, &mousePos.y);
        glm::vec3 sphericalCoords = camera_.getPos();
        if (yOffset == -1) {
            glm::vec3 updated = camera_.getPos();
            sphericalCoords.x += 0.1;
            camera_.setPos(sphericalCoords);
        } else {
            glm::vec3 updated = camera_.getPos();
            sphericalCoords.x -= 0.1;
            camera_.setPos(sphericalCoords);
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
        glViewport(0,0, width_, height_);
}

void ObjectRenderer::mouseDragCallback(){
    int state = glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS) {
        glm::dvec2 mousePos;
        glfwGetCursorPos(window_, &mousePos.x, &mousePos.y);
        glm::dvec2 aligmentVector = mousePosition_ - mousePos;
        glm::vec3 sphericalCoords = camera_.getPos();
        std::cout << sphericalCoords.x << " " << sphericalCoords.y << " " << sphericalCoords.z << std::endl;
        sphericalCoords.y += 0.001*aligmentVector.x;
        sphericalCoords.z += 0.001*aligmentVector.y;
        camera_.setPos(sphericalCoords);
        mousePosition_ = mousePos;
    }
}

void ObjectRenderer::mouseButtonCallback(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window_, &mousePosition_.x, &mousePosition_.y);
    }
}