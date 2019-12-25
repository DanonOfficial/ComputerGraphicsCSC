//
// Created by roundedglint585 on 12/25/19.
//

#ifndef PARTICLES_PARTICLESEMMITER_H
#define PARTICLES_PARTICLESEMMITER_H


#include <cstdint>
#include <vector>
#include "Particle.h"
#include "Shader.h"

class ParticlesEmmiter {
public:
    ParticlesEmmiter() = default;
    ParticlesEmmiter(uint32_t maxParticlesAmount, glm::vec3 Position);
    void render(Shader shader,const glm::vec3 &cameraPosition, float delta);
private:
    [[nodiscard]] uint32_t findFirstUnused();
    void createNewParticles(float delta);
    void sort();
    void simulate(const glm::vec3 &cameraPosition, float delta);
    void updateBuffers();
    void setBuffers();
    void createBuffers();
private:
    uint32_t VAO_;
    uint32_t particleVBO_, positionVBO_, colorsVBO_;
    uint32_t maxParticlesAmount_, lastUsedParticle_;
    uint32_t particlesCount_;
    std::vector<Particle> particles_;
    std::vector<float> particlesPosition_;
    std::vector<uint8_t> particlesColor_;
    glm::vec3 position_;
};


#endif //PARTICLES_PARTICLESEMMITER_H
