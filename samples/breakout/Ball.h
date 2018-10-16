//
// Created by snorri on 15.10.2018.
//

#ifndef VULKAN_SPRITES_BALL_H
#define VULKAN_SPRITES_BALL_H


#include "Movable.h"
#include "Paddle.h"

class Ball : public Movable {
public:
    void SetRadius(float r);
    float GetRadius();

    void CollideWithPaddle(const Paddle& p);

    void Render(Renderer &r);

protected:
    float m_radius;
};


#endif //VULKAN_SPRITES_BALL_H
