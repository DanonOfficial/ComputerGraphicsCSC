//
// Created by roundedglint585 on 10/4/19.
//
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "include/ObjectRenderer.h"
#include "include/objects/cube.h"
constexpr uint32_t width = 800;
constexpr uint32_t height = 600;


int main() {
    ObjectRenderer objectRenderer(width, height);
    objectRenderer.init();
    objectRenderer.loadObjectFromFile("objects/sponza.obj");
    objectRenderer.addEmmiter(150000, {480.f, 120.f, -215.f});
    objectRenderer.addEmmiter(150000, {480.f, 120.f, 140.f});
    objectRenderer.run();
    return 0;
}
