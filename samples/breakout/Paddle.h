//
// Created by snorri on 15.10.2018.
//

#ifndef VULKAN_SPRITES_PADDLE_H
#define VULKAN_SPRITES_PADDLE_H


#include <Renderer.h>
#include "Movable.h"

class Paddle : public Movable {
public:
    Paddle() : m_minX(-10000), m_maxX(10000) {}
    void SetWidth(float w);
    float GetWidth() const;

    void SetHeight(float h);
    float GetHeight() const;

    void Render(Renderer &r);

    void Update(float td) override;

    void SetBounds(float min, float max);

protected:
    float m_width;
    float m_height;
    float m_minX;
    float m_maxX;
};


#endif //VULKAN_SPRITES_PADDLE_H
