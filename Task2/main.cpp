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
    objectRenderer.loadObjectFromData(cube);
    objectRenderer.run();
    return 0;
}


