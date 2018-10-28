//
// Created by snorri on 25.10.2018.
//

#ifndef VULKAN_SPRITES_PARTICLEEMITTER_H
#define VULKAN_SPRITES_PARTICLEEMITTER_H

#include "Particle.h"

#include <vector>
#include <random>

class Renderer;
class ParticleEmitter {
public:
    ParticleEmitter() : m_rng(-1.0f, 1.0f) {}

    int GetNumParticles() const;

    float GetEmissionRate() const;
    void SetEmissionRate(float rate);

    float GetLifespan() const;
    void SetLifespan(float ls);

    glm::vec2 GetPosition() const;
    void SetPosition(glm::vec2 pos);

    float GetDirection() const;
    void SetDirection(float a);

    float GetDirectionRange() const;
    void SetDirectionRange(float r);

    float GetSpeed() const;
    void SetSpeed(float s);

    void Update(float td);
    void Render(Renderer &renderer);

protected:
    float m_timeToEmit = 0.0f;
    float m_emissionRate = 0.0f;
    float m_lifespan = 1.0f;

    // Emission direction, as an angle (in radians) from the positive
    // x-axis (0 means to the right, pi/2 is up, pi is to left...
    float m_direction = 0.0f;

    // Range of emission direction - and individual particle's direction
    // will be the in the range [m_direction - m_directionRange, m_direction + m_directionRange]
    float m_directionRange = 0.0f;

    float m_speed = 0.0f;

    glm::vec2 m_position;
    std::vector<Particle> m_particles;

    std::default_random_engine m_random_engine;
    std::uniform_real_distribution<float> m_rng;
};

#endif //VULKAN_SPRITES_PARTICLEEMITTER_H
