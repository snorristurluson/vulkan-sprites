//
// Created by snorri on 15.10.2018.
//

#ifndef VULKAN_SPRITES_PADDLE_H
#define VULKAN_SPRITES_PADDLE_H


#include <Renderer.h>
#include "Movable.h"

class Paddle : public Movable {
public:
    void SetWidth(float w);
    float GetWidth() const;

    void SetHeight(float h);
    float GetHeight() const;

    void Render(Renderer &r);

    void Update(float td) override;

    void SetBounds(float min, float max);

protected:
    float m_width = 1;
    float m_height = 1;
    float m_minX = -10000;
    float m_maxX = 10000;
};


#endif //VULKAN_SPRITES_PADDLE_H
