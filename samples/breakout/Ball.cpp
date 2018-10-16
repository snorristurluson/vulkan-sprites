//
// Created by snorri on 15.10.2018.
//

#include "Ball.h"

void Ball::SetRadius(float r) {
    m_radius = r;
}

float Ball::GetRadius() {
    return m_radius;
}

void Ball::CollideWithPaddle(const Paddle &p) {
    auto paddlePos = p.GetPosition();
    auto paddleHalfWidth = p.GetWidth() / 2.0f;
    auto paddleHalfHeight = p.GetHeight() / 2.0f;
    if(m_position.x + m_radius < paddlePos.x - paddleHalfWidth) {
        return;
    }
    if(m_position.x - m_radius > paddlePos.x + paddleHalfWidth) {
        return;
    }
    if(m_position.y + m_radius < paddlePos.y + paddleHalfHeight) {
        return;
    }
    if(m_position.y > paddlePos.y && m_velocity.y < 0.0f) {
        return;
    }
    m_velocity.y *= -1.0f;
}

void Ball::Render(Renderer &r) {
    r.SetTexture(nullptr);
    r.SetColor({0.5f, 1.0f, 0.5f, 1.0f});
    r.DrawSprite(m_position.x - m_radius, m_position.y - m_radius, m_radius, m_radius);
}
