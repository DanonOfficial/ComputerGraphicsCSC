
#pragma once

#include <glm_lib/glm/glm.hpp>

class TrackballCamera {
public:
    TrackballCamera(float theta, float phi, float radius) : theta_(theta), phi_(phi), radius_(radius), up_(1.0f),
                                                            target_(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)),
                                                            view_((glm::mat4(1.f))), viewNeedsUpdate_(true) {
    }

    void rotate(float dTheta, float dPhi);

    void zoom(float distance);

    glm::vec4 getCameraPosition() const {
        return target_ + transformToPolar();
    }

    glm::mat4 getView() {
        if (viewNeedsUpdate_) {
            updateViewMatrix();
            viewNeedsUpdate_ = false;
        }
        return view_;
    }


private:
    void updateViewMatrix();

    [[nodiscard]] glm::vec4 transformToPolar() const {
        float x = radius_ * sinf(phi_) * sinf(theta_);
        float y = radius_ * cosf(phi_);
        float z = radius_ * sinf(phi_) * cosf(theta_);
        float w = 1.0f;
        return glm::vec4(x, y, z, w);
    }

private:
    float theta_;
    float phi_;
    float radius_;
    float up_;

    glm::vec4 target_;

    glm::mat4 view_;

    bool viewNeedsUpdate_;
};

