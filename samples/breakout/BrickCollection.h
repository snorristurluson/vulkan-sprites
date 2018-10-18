//
// Created by snorri on 18.10.2018.
//

#ifndef VULKAN_SPRITES_BRICKCOLLECTION_H
#define VULKAN_SPRITES_BRICKCOLLECTION_H

#include <list>
#include "Ball.h"

class Renderer;

struct Brick {
    glm::vec2 pos;
    float width;
    float height;
};

class BrickCollection {
public:
    int GetBrickCount();

    void AddBrick(glm::vec2 pos, float width, float height);

    void CollideBallWithBricks(Ball &ball);

    void Render(Renderer& r);

protected:
    std::list<Brick> m_bricks;
};


#endif //VULKAN_SPRITES_BRICKCOLLECTION_H
