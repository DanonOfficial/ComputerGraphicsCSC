//
// Created by roundedglint585 on 11/22/19.
//

#include "../include/ObjectRenderer.h"
#include "../include/ObjParser.h"
#include "../include/Camera.h"
#include <glm/gtc/quaternion.hpp>

namespace details {
    glm::vec3 transformToSpherical(glm::vec3 pos) {
        return {glm::sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z),
                glm::acos(pos.z / glm::sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z)),
                glm::atan(pos.y, pos.x)};
    }

    glm::vec3 transformToCartesian(glm::vec3 pos) {
        return {pos.x * glm::sin(pos.y) * glm::cos(pos.z),
                pos.x * glm::sin(pos.y) * glm::sin(pos.z),
                pos.x * glm::cos(pos.y)
        };
    }

    glm::vec3 translateFromNDCtoArcball(glm::vec2 pos) {
        float dot = pos.x * pos.x + pos.y * pos.y;
        glm::vec3 result = {pos, 0};
        if (dot <= 1.0f) {
            result.z = glm::sqrt(1.0f - dot);
        } else {
            result = glm::normalize(result);
        }
        return result;
    }

    glm::quat transformQuatFromTo(glm::vec3 from, glm::vec3 to) {
        glm::quat q;
        q.x = from.y * to.z - from.z * to.y;
        q.y = from.z * to.x - from.x * to.z;
        q.z = from.x * to.y - from.y * to.x;
        q.w = from.x * to.x + from.y * to.y + from.z * to.z;
        return q;
    }
}

ObjectRenderer::ObjectRenderer(uint32_t width, uint32_t height) : width_(width), height_(height), window_(nullptr) {

}

void ObjectRenderer::init() {
    glfwInitialization();
    glewInit();
    shaderInitialization();
    camera_ = Camera({0.0f, 0.0f, 5.f});
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
    glEnable(GL_CULL_FACE);
    glm::mat4 model = glm::mat4(1.0f);
    while (!glfwWindowShouldClose(window_)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 view = camera_.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) width_ / (float) height_, 0.1f, 100.0f);
        shader_.use();
        shader_.setMat4("model", model);
        shader_.setMat4("view", view);
        shader_.setMat4("projection", projection);
        shader_.setVec3("lightColor", {1.0f, 1.0f, 1.0f});
        shader_.setVec3("lightPos", camera_.getPos());
        currentMesh_.draw(shader_);
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void ObjectRenderer::shaderInitialization() {
    shader_ = Shader("shaders/ObjectRenderVertex.glsl", "shaders/ObjectRenderFragment.glsl");
}

void ObjectRenderer::mouseScrollCallback(double yOffset) {
    glm::dvec2 mousePos;
    glfwGetCursorPos(window_, &mousePos.x, &mousePos.y);
    glm::vec3 sphericalCoords = details::transformToSpherical(camera_.getPos());
    if (yOffset == -1) {
        glm::vec3 updated = camera_.getPos();
        sphericalCoords.x *= 1.1;
        camera_.setPos(details::transformToCartesian(sphericalCoords));
    } else {
        glm::vec3 updated = camera_.getPos();
        sphericalCoords.x *= 0.9;
        camera_.setPos(details::transformToCartesian(sphericalCoords));
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
    int state = glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS) {
        glm::dvec2 mousePos;
        glfwGetCursorPos(window_, &mousePos.x, &mousePos.y);
        glm::vec3 spherical = camera_.getPos();
        glm::quat tr = details::transformQuatFromTo(
                details::translateFromNDCtoArcball(affineTransformToNDC(mousePosition_)),
                details::translateFromNDCtoArcball(affineTransformToNDC(mousePos)));

        glm::mat4 translate = glm::mat4_cast(tr);
        glm::vec4 pos = {camera_.getPos(), 1.f};
        pos = pos * translate;
        camera_.setPos({pos.x, pos.y, pos.z});
        mousePosition_ = mousePos;
    }
}

void ObjectRenderer::mouseButtonCallback(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window_, &mousePosition_.x, &mousePosition_.y);
        tranQuat_ = {1.0f, 0.0f, 0.0f, 0.f};
    }
}

glm::vec2 ObjectRenderer::affineTransformToNDC(glm::vec2 vector) {
    return {vector.x / width_ * 2.f - 1.f, (-vector.y / height_ + 1.f) * 2.f - 1.f};
}
