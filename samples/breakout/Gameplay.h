//
// Created by snorri on 14.10.2018.
//

#ifndef VULKAN_SPRITES_GAMEPLAY_H
#define VULKAN_SPRITES_GAMEPLAY_H


#include "AppState.h"
#include "Paddle.h"
#include "Ball.h"
#include "BrickCollection.h"

class Gameplay : public AppState {
public:
    Gameplay(App *a);

    void Init(Renderer &r) override;
    void Enter(Renderer &r) override;
    void Render(Renderer &r) override;
    void Exit(Renderer &r) override;
    void HandleKey(int key, int scancode, int action, int mods) override;

    void HandleCursorPosition(double xpos, double ypos) override;

protected:
    int m_frameCounter;
    std::shared_ptr<Font> m_titleFont;

    Paddle m_paddle;
    float m_paddleSpeed;

    Ball m_ball;
    
    BrickCollection m_bricks;

    bool m_leftPressed;
    bool m_rightPressed;

    float m_fieldWidth;
    float m_fieldHeight;
};


#endif //VULKAN_SPRITES_GAMEPLAY_H
