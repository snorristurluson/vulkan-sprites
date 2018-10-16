//
// Created by snorri on 15.10.2018.
//

#ifndef VULKAN_SPRITES_MOVABLE_H
#define VULKAN_SPRITES_MOVABLE_H


#include <glm/vec2.hpp>

class Movable {
public:
    Movable();

    void SetPosition(glm::vec2 pos);
    glm::vec2 GetPosition() const;

    void SetVelocity(glm::vec2 v);
    glm::vec2 GetVelocity() const;

    virtual void Update(float td);

protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
};


#endif //VULKAN_SPRITES_MOVABLE_H
