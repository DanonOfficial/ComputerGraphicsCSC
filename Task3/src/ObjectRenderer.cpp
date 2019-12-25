//
// Created by roundedglint585 on 11/22/19.
//

#include "../include/ObjectRenderer.h"
#include "../include/ObjParser.h"
#include "../include/TrackballCamera.h"
#include "../include/shaders/ObjectRenderVertex.h"
#include "../include/shaders/ObjectRenderFragment.h"
#include "../include/shaders/ParticlesFragment.h"
#include "../include/shaders/ParticlesVertex.h"

ObjectRenderer::ObjectRenderer(uint32_t width, uint32_t height) : width_(width), height_(height), window_(nullptr), camera_({-10.f, 120.f, -10.f}), isFirst_(true){

}

void ObjectRenderer::init() {
    glfwInitialization();
    glewInit();
    shaderInitialization();
    callbackInitialization();
    imguiInitialization();
}
glm::vec2 ObjectRenderer::affineTransformToNDC(glm::vec2 vector) {
    return {vector.x / width_ * 2.f - 1.f, (-vector.y / height_ + 1.f) * 2.f - 1.f};
}

void ObjectRenderer::loadObjectFromFile(std::string_view path) {
    auto cube = objParser::parseFile(path);
    currentMesh_ = Mesh(std::get<1>(cube), std::get<0>(cube));
}
void ObjectRenderer::loadObjectFromData(const std::string &data) {
    auto cube = objParser::parseData(data);
    currentMesh_ = Mesh(std::get<1>(cube), std::get<0>(cube));
}
void ObjectRenderer::imguiInitialization() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
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
void ObjectRenderer::renderGUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // GUI
    ImGui::Begin("Dissolve window");
    //ImGui::SliderFloat("Threshold Count", &dissolveThreshold_, 0.f, 1.f);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}
void ObjectRenderer::run() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window_)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double currentTime = glfwGetTime();
        double delta = currentTime - lastTime;
        lastTime = currentTime;
        glm::mat4 view_ = camera_.getView();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) width_ / (float) height_, 0.1f, 5000.0f);
        objectShader_.use();
        objectShader_.setMat4("model", model);
        objectShader_.setMat4("view", view_ );
        objectShader_.setMat4("projection", projection);
        objectShader_.setVec3("lightColor", {1.0f, 1.0f, 1.0f});
        objectShader_.setVec3("lightPos", camera_.getPosition());
        currentMesh_.draw(objectShader_);
        particlesShader_.use();
        particlesShader_.setMat4("view", view_ );
        particlesShader_.setMat4("projection", projection);
        particlesShader_.setVec3("cameraUp", camera_.getUp());
        particlesShader_.setVec3("cameraRight", camera_.getRight());
        for(auto &i: emmiters_){
            i.render(particlesShader_, camera_.getPosition(), delta);
        }
        renderGUI();
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void ObjectRenderer::shaderInitialization() {
    objectShader_ = Shader(objectRenderVertex, objectRenderFragment);
    particlesShader_ = Shader(particlesVertex, particlesFragment);
}

void ObjectRenderer::callbackInitialization() {
    glfwSetWindowUserPointer(window_, reinterpret_cast<void *>(this));

    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow *window, int width, int height) {
        auto *objectRenderer = reinterpret_cast<ObjectRenderer *>(glfwGetWindowUserPointer(window));
        objectRenderer->framebufferSizeCallback(width, height);
    });


    glfwSetCursorPosCallback(window_, [](GLFWwindow *window, double xPos, double yPos) {
        auto *objectRenderer = reinterpret_cast<ObjectRenderer *>(glfwGetWindowUserPointer(window));
        objectRenderer->mousePosCallback(xPos, yPos);
    });

    glfwSetKeyCallback(window_, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto *objectRenderer = reinterpret_cast<ObjectRenderer *>(glfwGetWindowUserPointer(window));
        objectRenderer->keyboardCallback(key, action);
    });

}

void ObjectRenderer::framebufferSizeCallback(int width, int height) {
    width_ = width;
    height_ = height;
    glViewport(0, 0, width_, height_);
}


void ObjectRenderer::mousePosCallback(double xPos, double yPos) {
    if(isFirst_){
        prevPos = {xPos, yPos};
        isFirst_ = false;
    }else {
        glm::vec2 curPos = {xPos, yPos};
        glm::vec2 diff = curPos - prevPos;
        std::cout << diff.x << " " << diff.y << std::endl;
        camera_.viewHandler(diff.x, -diff.y);
        prevPos = curPos;
    }


}

void ObjectRenderer::keyboardCallback(int key, int action) {
    std::cout << camera_.getPosition().x << " " << camera_.getPosition().y << " " << camera_.getPosition().z << std::endl;
    if(((action == GLFW_PRESS && key == GLFW_KEY_W))){
        camera_.moveHandler(camera_.FORWARD);
    }
    if(((action == GLFW_PRESS && key == GLFW_KEY_S))){
        camera_.moveHandler(camera_.BACKWARD);
    }
    if(((action == GLFW_PRESS && key == GLFW_KEY_D))){
        camera_.moveHandler(camera_.RIGHT);
    }
    if(((action == GLFW_PRESS && key == GLFW_KEY_A))){
        camera_.moveHandler(camera_.LEFT);
    }
}

void ObjectRenderer::addEmmiter(uint32_t particlesAmount, const glm::vec3 &position) {
    emmiters_.emplace_back(particlesAmount, position);
}
