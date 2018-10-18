//
// Created by snorri on 14.10.2018.
//

#ifndef VULKAN_SPRITES_GAMEOVER_H
#define VULKAN_SPRITES_GAMEOVER_H


#include "AppState.h"

class GameOver : public AppState {
public:
    GameOver(App *a);

    void Init(Renderer &r) override;

    void Enter(Renderer &r) override;

    void Render(Renderer &r) override;

    void Exit(Renderer &r) override;

    void HandleKey(int key, int scancode, int action, int mods) override;

    void HandleCursorPosition(double xpos, double ypos) override;

protected:
    int m_frameCounter;
    std::shared_ptr<Font> m_titleFont;
};


#endif //VULKAN_SPRITES_GAMEOVER_H
