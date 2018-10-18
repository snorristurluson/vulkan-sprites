//
// Created by snorri on 18.10.2018.
//

#include "BrickCollection.h"

int BrickCollection::GetBrickCount() {
    return static_cast<int>(m_bricks.size());
}

void BrickCollection::AddBrick(glm::vec2 pos, float width, float height) {
    m_bricks.emplace_back(Brick {pos, width, height});
}

void BrickCollection::CollideBallWithBricks(Ball &ball) {
    for(auto it = m_bricks.begin(); it != m_bricks.end(); ) {
        if(ball.CollideWithBrick(*it)) {
            it = m_bricks.erase(it);
        } else {
            ++it;
        }
    }
}

void BrickCollection::Render(Renderer &r) {
    r.SetColor({1.0f, 0.0f, 0.0f, 1.0f});

    for(auto brick: m_bricks) {
        r.DrawSprite(
                brick.pos.x - brick.width/2.0f,
                brick.pos.y - brick.height/2.0f,
                brick.width,
                brick.height);
    }
}
