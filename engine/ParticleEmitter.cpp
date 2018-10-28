//
// Created by snorri on 25.10.2018.
//

#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "Renderer.h"

float ParticleEmitter::GetEmissionRate() const {
    return m_emissionRate;
}

void ParticleEmitter::SetEmissionRate(float rate) {
    m_emissionRate = rate;
}

int ParticleEmitter::GetNumParticles() const {
    return static_cast<int>(m_particles.size());
}

float ParticleEmitter::GetLifespan() const {
    return m_lifespan;
}

void ParticleEmitter::SetLifespan(float ls) {
    m_lifespan = ls;
}

glm::vec2 ParticleEmitter::GetPosition() const {
    return m_position;
}

void ParticleEmitter::SetPosition(glm::vec2 pos) {
    m_position = pos;
}

void ParticleEmitter::Update(float td) {
    std::vector<std::vector<Particle>::iterator> dead;

    for(auto pIt = m_particles.begin(); pIt != m_particles.end(); ++pIt) {
        pIt->timeRemaining -= td;
        if(pIt->timeRemaining <= 0.0f) {
            pIt->timeRemaining = 0.0f;
            dead.emplace_back(pIt);
        } else {
            pIt->position += pIt->velocity*td;
            pIt->color = glm::vec4 {1.0f, 1.0f, 1.0f, 1.0f} * pIt->timeRemaining;
        }
    }

    if(m_timeToEmit <= 0.0f) {
        auto deadIt = dead.begin();
        while(m_timeToEmit < td) {
            Particle* p;
            if(deadIt != dead.end()) {
                p = &(**deadIt++);
            } else {
                auto& newParticle = m_particles.emplace_back();
                p = &newParticle;
            }
            p->timeRemaining = m_lifespan;
            p->position = m_position;
            float x = cosf(m_direction + m_directionRange * m_rng(m_random_engine));
            float y = sinf(m_direction + m_directionRange * m_rng(m_random_engine));
            p->velocity = glm::vec2 {m_speed * x, m_speed * y};
            p->color = glm::vec4 {1.0f, 1.0f, 1.0f, 1.0f};

            m_timeToEmit += 1.0f/m_emissionRate;
        }
    }
    m_timeToEmit -= td;
}

void ParticleEmitter::Render(Renderer &r) {
    r.SetBlendMode(BM_BLEND);
    for(auto& p: m_particles) {
        if(p.timeRemaining > 0.0f) {
            r.SetColor(p.color);
            r.DrawSprite(p.position.x - 8.0f, p.position.y - 8.0f, 16, 16);
        }
    }
}

float ParticleEmitter::GetDirection() const {
    return m_direction;
}

void ParticleEmitter::SetDirection(float a) {
    m_direction = a;
}

float ParticleEmitter::GetDirectionRange() const {
    return m_directionRange;
}

void ParticleEmitter::SetDirectionRange(float r) {
    m_directionRange = r;
}

float ParticleEmitter::GetSpeed() const {
    return m_speed;
}

void ParticleEmitter::SetSpeed(float s) {
    m_speed = s;
}

