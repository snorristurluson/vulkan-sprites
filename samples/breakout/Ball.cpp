//
// Created by snorri on 15.10.2018.
//

#include "Ball.h"

Ball::Ball() :
    m_radius(1.0f),
    m_minX(0.0f),
    m_minY(0.0f),
    m_maxX(1.0f),
    m_maxY(1.0f)
{
}

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

void Ball::SetBounds(float minX, float minY, float maxX, float maxY) {
    m_minX = minX;
    m_minY = minY;
    m_maxX = maxX;
    m_maxY = maxY;
}

void Ball::Update(float td) {
    Movable::Update(td);

    if(m_position.x < m_minX) {
        m_position.x = m_minX;
        m_velocity.x *= -1.0f;
    }

    if(m_position.x > m_maxX) {
        m_position.x = m_maxX;
        m_velocity.x *= -1.0f;
    }

    if(m_position.y < m_minY) {
        m_position.y = m_minY;
        m_velocity.y *= -1.0f;
    }

    if(m_position.y > m_maxY) {
        m_position.y = m_maxY;
        m_velocity.y *= -1.0f;
    }
}
