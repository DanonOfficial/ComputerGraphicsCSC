//
// Created by roundedglint585 on 11/22/19.
//

#ifndef TASK2_CAMERA_H
#define TASK2_CAMERA_H



#include <glm/glm.hpp>

class Camera {
public:
    explicit Camera(glm::vec3 pos = {10.0f,0.0f,0.0f} , glm::vec3 up = {0.0f,1.0f,0.0f} );
    [[nodiscard]] glm::mat4 getViewMatrix() const;
    [[nodiscard]] glm::vec3 getPos() const;
    void setPos(const glm::vec3 &pos);

private:
    glm::vec3 transformToCartesian(const glm::vec3 &pos) const;
    void update();
    glm::vec3 worldUp_={0.f, 1.f, 0.f};
    glm::vec3 pos_; // radius, theta(radians) and phi(radians)
    glm::vec3 destination_; // radius, theta(radians) and phi(radians)
    glm::vec3 up_;
    glm::vec3 right_;
};

#endif //TASK2_CAMERA_H
