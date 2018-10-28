//
// Created by snorri on 15.10.2018.
//

#ifndef VULKAN_SPRITES_BALL_H
#define VULKAN_SPRITES_BALL_H


#include <ParticleSystem.h>
#include "Movable.h"
#include "Paddle.h"

class Brick;

class Ball : public Movable {
public:
    Ball();

    void SetRadius(float r);
    float GetRadius();

    void SetBounds(float minX, float minY, float maxX, float maxY);

    void CollideWithPaddle(const Paddle& p);
    bool CollideWithBrick(const Brick& brick);

    void Render(Renderer &r);

    void Update(float td) override;

protected:
    float m_radius;
    float m_minX;
    float m_minY;
    float m_maxX;
    float m_maxY;
    ParticleSystem m_particleSystem;
    std::shared_ptr<ParticleEmitter> m_emitter;
};


#endif //VULKAN_SPRITES_BALL_H
