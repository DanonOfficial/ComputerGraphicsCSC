
#include <glm/gtc/matrix_transform.hpp>
#include "../include/Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 up) : pos_(pos),up_(up), destination_({0.0f,0.0f,0.0f}){

}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(pos_,glm::normalize(transformToCartesian(pos_) - transformToCartesian(destination_)), glm::normalize(up_));
}

glm::vec3 Camera::getPos() const {
    return pos_;
}



glm::vec3 Camera::transformToCartesian(const glm::vec3 &pos) const {
    return {pos.x*glm::sin(pos.y)*glm::cos(pos.z),
            pos.x*glm::sin(pos.y)*glm::sin(pos.z),
            pos.x*glm::cos(pos.y)};
}

void Camera::setPos(const glm::vec3 &pos) {
    pos_ = pos;
}

void Camera::update() {

    right_ = glm::normalize(glm::cross(glm::normalize(transformToCartesian(pos_) - transformToCartesian(destination_)), worldUp_));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up_    = glm::normalize(glm::cross(right_, glm::normalize(transformToCartesian(pos_) - transformToCartesian(destination_))));
}
