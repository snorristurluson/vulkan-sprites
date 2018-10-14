//
// Created by snorri on 14.10.2018.
//

#ifndef VULKAN_SPRITES_GAMEPLAY_H
#define VULKAN_SPRITES_GAMEPLAY_H


#include "AppState.h"

class Gameplay : public AppState {
public:
    Gameplay(App *a);

    void Init(Renderer &r) override;

    void Enter(Renderer &r) override;

    void Render(Renderer &r) override;

    void Exit(Renderer &r) override;

    void HandleKey(int key, int scancode, int action, int mods) override;

protected:
    int m_frameCounter;
    std::shared_ptr<Font> m_titleFont;
};


#endif //VULKAN_SPRITES_GAMEPLAY_H
