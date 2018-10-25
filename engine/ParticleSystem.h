//
// Created by snorri on 25.10.2018.
//

#ifndef VULKAN_SPRITES_PARTICLESYSTEM_H
#define VULKAN_SPRITES_PARTICLESYSTEM_H

#include <memory>
#include <vector>
#include "ParticleEmitter.h"

class ParticleSystem {
public:
    int GetNumParticles();
    int GetNumEmitters();

    std::shared_ptr<ParticleEmitter> AddEmitter();

    void Update(float td);

    void Render(Renderer &r);

protected:
    std::vector<std::shared_ptr<ParticleEmitter>> m_emitters;
};


#endif //VULKAN_SPRITES_PARTICLESYSTEM_H
