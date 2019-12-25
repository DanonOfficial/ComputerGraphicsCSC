//
// Created by roundedglint585 on 12/23/19.
//

#ifndef TASK2_FREECAMERA_H
#define TASK2_FREECAMERA_H

#include <glm_lib/glm/glm.hpp>

class FreeCamera{
public:
    FreeCamera();
    FreeCamera(glm::vec3 pos);
    enum Direction{FORWARD, BACKWARD, LEFT, RIGHT};
    [[nodiscard]] glm::mat4 getView() const;
    void moveHandler(Direction direction);
    void viewHandler(float xOffset, float yOffset);
    [[nodiscard]] const glm::vec3 &getPosition() const;

    const glm::vec3 &getUp() const;

    void setUp(const glm::vec3 &up);

    const glm::vec3 &getRight() const;

    void setRight(const glm::vec3 &right);

private:
    void update();
private:
    glm::vec3 position_;
    glm::vec3 up_;
    glm::vec3 front_;
    glm::vec3 right_;
    glm::vec3 globalUp_;
    float speed_;
    float mouseSensetivity_;
    float yaw_;
    float pitch_;

};
#endif //TASK2_FREECAMERA_H
