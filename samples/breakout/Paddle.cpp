//
// Created by snorri on 15.10.2018.
//

#include "Paddle.h"

void Paddle::SetWidth(float w) {
    m_width = w;
}

float Paddle::GetWidth() const {
    return m_width;
}

void Paddle::SetHeight(float h) {
    m_height = h;
}

float Paddle::GetHeight() const {
    return m_height;
}

void Paddle::Render(Renderer &r) {
    r.SetColor({1,1,1,1});
    r.SetTexture(nullptr);
    r.DrawSprite(m_position.x - m_width/2.0f, m_position.y - m_height/2.0f, m_width, m_height);
}

void Paddle::Update(float td) {
    Movable::Update(td);

    auto halfWidth = m_width / 2.0f;
    if(m_position.x < m_minX + halfWidth) {
        m_position.x = m_minX + halfWidth;
    }
    if(m_position.x > m_maxX - halfWidth) {
        m_position.x = m_maxX - halfWidth;
    }
}

void Paddle::SetBounds(float min, float max) {
    m_minX = min;
    m_maxX = max;
}

