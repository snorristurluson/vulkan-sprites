//
// Created by snorri on 25.10.2018.
//

#include "ParticleSystem.h"
#include "Renderer.h"
#include "telemetry.h"

int ParticleSystem::GetNumParticles() {
    int n = 0;
    for(auto& em: m_emitters) {
        n += em->GetNumParticles();
    }

    return n;
}

int ParticleSystem::GetNumEmitters() {
    return static_cast<int>(m_emitters.size());
}

std::shared_ptr<ParticleEmitter> ParticleSystem::AddEmitter() {
    auto em = std::make_shared<ParticleEmitter>();
    m_emitters.emplace_back(em);
    return em;
}

void ParticleSystem::Update(float td) {
    tmFunction(0, 0);

    for(auto& em: m_emitters) {
        em->Update(td);
    }
}

void ParticleSystem::Render(Renderer &r) {
    tmFunction(0, 0);

    for(auto& em: m_emitters) {
        em->Render(r);
    }
}

