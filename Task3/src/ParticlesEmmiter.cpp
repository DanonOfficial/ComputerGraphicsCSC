//
// Created by roundedglint585 on 12/25/19.
//

#include <glm_lib/glm/gtx/norm.hpp>
#include "../include/ParticlesEmmiter.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <algorithm>

ParticlesEmmiter::ParticlesEmmiter(uint32_t maxParticlesAmount, glm::vec3 position) : position_(position),
                                                                                      maxParticlesAmount_(
                                                                                              maxParticlesAmount),
                                                                                      particles_(maxParticlesAmount),
                                                                                      particlesPosition_(
                                                                                              maxParticlesAmount * 4),
                                                                                      particlesColor_(
                                                                                              maxParticlesAmount * 4),
                                                                                      lastUsedParticle_(0) {
    createBuffers();
}

void ParticlesEmmiter::updateBuffers() {
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO_);
    glBufferData(GL_ARRAY_BUFFER, maxParticlesAmount_ * 4 * sizeof(GLfloat), nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount_ * sizeof(GLfloat) * 4, &particlesPosition_[0]);

    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO_);
    glBufferData(GL_ARRAY_BUFFER, maxParticlesAmount_ * 4 * sizeof(GLubyte), nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount_ * sizeof(GLubyte) * 4, &particlesColor_[0]);
}

void ParticlesEmmiter::createBuffers() {
    ;
    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    static const GLfloat particle[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
    };
    glGenBuffers(1, &particleVBO_);
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle), particle, GL_STATIC_DRAW);

    glGenBuffers(1, &positionVBO_);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO_);
    glBufferData(GL_ARRAY_BUFFER, maxParticlesAmount_ * 4 * sizeof(GLfloat), nullptr, GL_STREAM_DRAW);

    glGenBuffers(1, &colorsVBO_);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO_);
    glBufferData(GL_ARRAY_BUFFER, maxParticlesAmount_ * 4 * sizeof(GLubyte), nullptr, GL_STREAM_DRAW);


}

uint32_t ParticlesEmmiter::findFirstUnused() {
    for (uint32_t i = lastUsedParticle_; i < maxParticlesAmount_; i++) {
        if (particles_[i].lifeTime < 0) {
            return i;
        }
    }

    for (uint32_t i = 0; i < lastUsedParticle_; i++) {
        if (particles_[i].lifeTime < 0) {
            return i;
        }
    }

    return 0;
}

void ParticlesEmmiter::sort() {
    std::sort(&particles_[0], &particles_[maxParticlesAmount_ - 1],
              [](const Particle &a, const Particle &b) { return a.distanceToCamera > b.distanceToCamera; });
}

void ParticlesEmmiter::createNewParticles(float delta) {
    uint32_t amountOfNewParticles = (int) (delta * 10000.0);
    if (amountOfNewParticles > (int) (0.016f * 10000.0))
        amountOfNewParticles = (int) (0.016f * 10000.0);

    for (uint32_t i = 0; i < amountOfNewParticles; i++) {
        uint32_t particleIndex = findFirstUnused();
        particles_[particleIndex].lifeTime = 4.0f;
        particles_[particleIndex].pos = position_;

        float spread = 0.05f;
        glm::vec3 globalDirection = glm::vec3(0.f, 10.f, 0.f);
        glm::vec3 offsetDirection = glm::vec3(
                (rand() % 3000 - 1000.0f) / 50.0f,
                (rand() % 3000 - 1000.0f) / 15.0f,
                (rand() % 3000 - 1000.0f) / 15.0f
        );

        particles_[particleIndex].speed = globalDirection + offsetDirection * spread;


        particles_[particleIndex].red = (int) (255/4.f * particles_[particleIndex].lifeTime); //to set red color
        particles_[particleIndex].alpha = (rand() % 256) / 3;

        particles_[particleIndex].size = (rand() % 10000) / 1000.0f + 0.1f;

    }
}

void ParticlesEmmiter::simulate(const glm::vec3 &cameraPosition, float delta) {
    particlesCount_ = 0;
    for (int i = 0; i < maxParticlesAmount_; i++) {
        if (particles_[i].lifeTime > 0.0f) {

            particles_[i].lifeTime -= delta;
            if (particles_[i].lifeTime > 0.0f) {

                particles_[i].pos += particles_[i].speed * delta;
                particles_[i].distanceToCamera = glm::length2(particles_[i].pos - cameraPosition);

                particlesPosition_[4 * particlesCount_ + 0] = particles_[i].pos.x;
                particlesPosition_[4 * particlesCount_ + 1] = particles_[i].pos.y;
                particlesPosition_[4 * particlesCount_ + 2] = particles_[i].pos.z;
                particlesPosition_[4 * particlesCount_ + 3] = particles_[i].size;

                particlesColor_[4 * particlesCount_ + 0] = particles_[i].red;
                particlesColor_[4 * particlesCount_ + 1] = particles_[i].green;
                particlesColor_[4 * particlesCount_ + 2] = particles_[i].blue;
                particlesColor_[4 * particlesCount_ + 3] = particles_[i].alpha;
            } else {
                particles_[i].distanceToCamera = -1.0f;
            }

            particlesCount_++;

        }
    }
}

void ParticlesEmmiter::render(Shader shader, const glm::vec3 &cameraPosition, float delta) {
    shader.use();
    glBindVertexArray(VAO_);
    createNewParticles(delta);
    simulate(cameraPosition, delta);
    sort();
    updateBuffers();
    setBuffers();

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesCount_);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

}

void ParticlesEmmiter::setBuffers() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO_);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO_);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, nullptr);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
}
