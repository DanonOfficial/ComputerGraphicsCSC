//
// Created by roundedglint585 on 10/5/19.
//

#include "../include/Fractal.h"

glm::vec2 Fractal::affineTransformToNDC(glm::vec2 vector) {
    return {vector.x / width_ * 2.f - 1.f, (-vector.y / height_ + 1.f) * 2.f - 1.f};
}

void Fractal::glfwInitialization() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window_ = glfwCreateWindow(width_, height_, "Fractals", nullptr, nullptr);
    glfwMakeContextCurrent(window_);
    glViewport(0, 0, width_, height_);
}

void Fractal::callbackInitialization() {

    glfwSetWindowUserPointer(window_, reinterpret_cast<void *>(this));

    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow *window, int width, int height) {
        auto *fractal = reinterpret_cast<Fractal *>(glfwGetWindowUserPointer(window));
        fractal->framebufferSizeCallback(width, height);
    });

    glfwSetScrollCallback(window_, [](GLFWwindow *window, double xoffset, double yoffset) {
        auto *fractal = reinterpret_cast<Fractal *>(glfwGetWindowUserPointer(window));
        fractal->mouseScrollCallback(yoffset);
    });

    glfwSetMouseButtonCallback(window_, [](GLFWwindow *window, int button, int action, int mods) {
        auto *fractal = reinterpret_cast<Fractal *>(glfwGetWindowUserPointer(window));
        fractal->mouseButtonCallback(button, action);
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow *window, double xpos, double ypos) {
        auto *fractal = reinterpret_cast<Fractal *>(glfwGetWindowUserPointer(window));
        fractal->cursorPositionCallback();
    });
}

void Fractal::imguiInitialization() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
}

void Fractal::loadVertices() {
    float vertices[] = {-1.f, 1.f, 0.0f,
                        1.f, 1.f, 0.0f,
                        1.f, -1.f, 0.0f,
                        -1.f, 1.f, 0.0f,
                        -1.f, -1.f, 0.0f,
                        1.f, -1.f, 0.0f,
    };

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Fractal::loadTextures() {
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_1D,
                  texture_);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/texture.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, width, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
}

void Fractal::init() {
    glfwInitialization();
    glewInit();
    callbackInitialization();

    imguiInitialization();
    loadVertices();
    loadTextures();

    shader_ = Shader("shaders/FractalVertex.glsl", "shaders/FractalFragment.glsl");
}

void Fractal::render() {
    glBindTexture(GL_TEXTURE_1D, texture_);

    shader_.use();
    shader_.setFloat("scale", scale_);
    shader_.setInt("iter", iteration_);
    shader_.setVec2("center", center_);

    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

Fractal::Fractal(uint32_t width, uint32_t height) : width_(width), height_(height), window_(nullptr), VBO_(-1),
                                                    VAO_(-1), texture_(-1), shader_(), mousePosition_({0, 0}) {
}

void Fractal::renderGUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // GUI
    ImGui::Begin("Coefficient window");
    ImGui::SliderInt("Iteration Count", &iteration_, 0, 1000);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}

void Fractal::run() {
    while (!glfwWindowShouldClose(window_)) {
        glClearColor(1.f, 1.f, 1.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        render();
        renderGUI();
        glfwSwapBuffers(window_);
        glfwPollEvents();

    }
}

Fractal::~Fractal() {
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void Fractal::mouseScrollCallback(double yoffset) {
    float scaleFactor = scale_ / 10.0f;


    glm::dvec2 mousePos;
    glfwGetCursorPos(window_, &mousePos.x, &mousePos.y);
    mousePos = affineTransformToNDC(mousePos);
    glm::dvec2 mousePosScaled = mousePos;
    mousePos.x *= (1.f + scaleFactor);
    mousePos.y *= (1.f + scaleFactor);
    glm::dvec2 aligmentVector = mousePos - mousePosScaled;

    if (yoffset == 1) {
        scale_ -= scaleFactor;
        center_ -= aligmentVector;
    } else {
        scale_ += scaleFactor;
        center_ += aligmentVector;
    }


}

void Fractal::mouseButtonCallback(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window_, &mousePosition_.x, &mousePosition_.y);
        mousePosition_ = affineTransformToNDC(mousePosition_) * scale_;
    }
}

void Fractal::cursorPositionCallback() {

    int state = glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS && !ImGui::IsAnyItemActive()) {
        glm::dvec2 mousePos;
        glfwGetCursorPos(window_, &mousePos.x, &mousePos.y);
        mousePos = affineTransformToNDC(mousePos) * scale_;
        glm::dvec2 aligmentVector = mousePosition_ - mousePos;
        center_ -= aligmentVector;
        mousePosition_ = mousePos;
    }
}

void Fractal::framebufferSizeCallback(int width, int height) {
    width_ = width;
    height_ = height;
    glViewport(0,0, width_, height_);
}
