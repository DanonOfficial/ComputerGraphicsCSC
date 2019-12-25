//
// Created by roundedglint585 on 12/23/19.
//

#include <glm_lib/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../include/FreeCamera.h"

FreeCamera::FreeCamera():front_({0.f, 0.f, -1.f}), position_({0.f, 0.f,0.f}),  globalUp_({0.f, 1.f, 0.f}), yaw_(0.f), pitch_(0.f), speed_(10.f), mouseSensetivity_(0.1f){
    update();
}

FreeCamera::FreeCamera(glm::vec3 pos): front_({0.f, 0.f, -1.f}), position_(pos),  globalUp_({0.f, 1.f, 0.f}), yaw_(0.f), pitch_(0.f), speed_(10.f), mouseSensetivity_(0.1f){
update();
}
glm::mat4 FreeCamera::getView() const {
    return glm::lookAt(position_, position_+front_, up_);
}



void FreeCamera::update() {
    front_ = {(cos(glm::radians(yaw_)) * cos(glm::radians(pitch_))),
              sin(glm::radians(pitch_)),
              sin(glm::radians(yaw_)) * cos(glm::radians(pitch_))};
    front_ = glm::normalize(front_);
    right_ = glm::normalize(glm::cross(front_,globalUp_));
    up_ = glm::normalize(glm::cross(right_, front_));
}

void FreeCamera::moveHandler(Direction direction) {
    switch(direction){
        case Direction::FORWARD:
            position_ += glm::normalize(front_)*speed_;
            break;
        case Direction::BACKWARD:
            position_ -= glm::normalize(front_)*speed_;
            break;
        case Direction::LEFT:
            position_ -= glm::normalize(right_)*speed_;
            break;
        case Direction::RIGHT:
            position_ += glm::normalize(right_)*speed_;
            break;
    }
}

void FreeCamera::viewHandler(float xOffset, float yOffset) {
    yaw_ += xOffset*mouseSensetivity_;
    pitch_ += yOffset*mouseSensetivity_;
    std::cout << yaw_ << " " << pitch_ << std::endl;
    pitch_ = pitch_ < -89.9f ? -89.9f: pitch_;
    pitch_ = pitch_ > 89.9f ? 89.9f: pitch_;
    update();
}

const glm::vec3 &FreeCamera::getPosition() const {
    return position_;
}

const glm::vec3 &FreeCamera::getUp() const {
    return up_;
}

void FreeCamera::setUp(const glm::vec3 &up) {
    up_ = up;
}

const glm::vec3 &FreeCamera::getRight() const {
    return right_;
}

void FreeCamera::setRight(const glm::vec3 &right) {
    right_ = right;
}
