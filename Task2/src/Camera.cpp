
#include <glm_lib/glm/ext/scalar_constants.hpp>
#include <glm_lib/glm/gtc/matrix_transform.hpp>
#include "../include/Camera.h"



void Camera::rotate(float dTheta, float dPhi) {
    viewNeedsUpdate_ = true;

    if (up_ > 0.0f) {
        theta_ += dTheta;
    } else {
        theta_ -= dTheta;
    }

    phi_ += dPhi;
    if (phi_ > 2.f * glm::pi<float>()) {
        phi_ -= 2.f *  glm::pi<float>();
    } else if (phi_ < -2.f * glm::pi<float>()) {
        phi_ += 2.f *  glm::pi<float>();
    }

    if ((phi_ > 0 && phi_ <  glm::pi<float>()) || (phi_ < - glm::pi<float>() && phi_ > - glm::pi<float>())) {
        up_ = 1.0f;
    } else {
        up_ = -1.0f;
    }
}

void Camera::zoom(float distance) {
    viewNeedsUpdate_ = true;

    radius_ *= distance;

    if (radius_ <= 0.0f) {
        radius_ = 30.0f;
        glm::vec4 look = glm::normalize(target_ - getCameraPosition());
        target_ = (target_ + 30.f * look);
    }
}


void Camera::updateViewMatrix() {
    view_ = glm::lookAt(glm::vec3(getCameraPosition()), glm::vec3(target_), {0.0f, up_, 0.0f});
}

