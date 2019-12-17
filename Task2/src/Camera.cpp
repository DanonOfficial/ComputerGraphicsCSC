
#include <algorithm>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/Camera.h"


float pi = 3.1415;

void Camera::rotate(float dTheta, float dPhi) {
    viewNeedsUpdate_ = true;

    if (up_ > 0.0f) {
        theta_ += dTheta;
    } else {
        theta_ -= dTheta;
    }

    phi_ += dPhi;
    if (phi_ > 2.f * pi) {
        phi_ -= 2.f * pi;
    } else if (phi_ < -2.f * pi) {
        phi_ += 2.f * pi;
    }

    if ((phi_ > 0 && phi_ < pi) || (phi_ < -pi && phi_ > -pi)) {
        up_ = 1.0f;
    } else {
        up_ = -1.0f;
    }
}

void Camera::zoom(float distance) {
    viewNeedsUpdate_ = true;

    radius_ *= distance;

    // Don't let the radius go negative
    // If it does, re-project our target down the look vector
    if (radius_ <= 0.0f) {
        radius_ = 30.0f;
        glm::vec4 look = glm::normalize(target_ - getCameraPosition());
        target_ = (target_ + 30.f * look);
    }
}


void Camera::updateViewMatrix() {
    view_ = glm::lookAt(glm::vec3(getCameraPosition()), glm::vec3(target_), {0.0f, up_, 0.0f});
}

