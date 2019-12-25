//
// Created by roundedglint585 on 12/23/19.
//

#ifndef TASK2_PARTICLE_H
#define TASK2_PARTICLE_H

#include <glm_lib/glm/glm.hpp>

struct Particle{
    glm::vec3 pos, speed;
    uint8_t red,green,blue,alpha;
    float size;
    float lifeTime = -1.f;
    float distanceToCamera = -1.f;
};
#endif //TASK2_PARTICLE_H
