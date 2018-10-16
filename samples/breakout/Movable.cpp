//
// Created by snorri on 15.10.2018.
//

#include "Movable.h"

Movable::Movable() : m_position({0, 0}), m_velocity({0, 0}) {}

void Movable::SetPosition(glm::vec2 pos) {
    m_position = pos;
}

glm::vec2 Movable::GetPosition() const {
    return m_position;
}

void Movable::SetVelocity(glm::vec2 v) {
    m_velocity = v;
}

glm::vec2 Movable::GetVelocity() const {
    return m_velocity;
}

void Movable::Update(float td) {
    m_position += m_velocity * td;
}
