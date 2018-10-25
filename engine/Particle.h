//
// Created by snorri on 25.10.2018.
//

#ifndef VULKAN_SPRITES_PARTICLE_H
#define VULKAN_SPRITES_PARTICLE_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

struct Particle {
    float timeRemaining;
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color;
};


#endif //VULKAN_SPRITES_PARTICLE_H
