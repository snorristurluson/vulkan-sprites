//
// Created by snorri on 12.10.2018.
//

#ifndef VULKAN_SPRITES_MAINMENU_H
#define VULKAN_SPRITES_MAINMENU_H


#include "AppState.h"

class MainMenu : public AppState {
public:
    MainMenu(App *a);

    void Init(Renderer &r) override;
    void Enter(Renderer &r) override;
    void Render(Renderer &r) override;
    void Exit(Renderer &r) override;
    void HandleKey(int key, int scancode, int action, int mods) override;

    void HandleCursorPosition(double xpos, double ypos) override;

protected:
    int m_frameCounter;

    std::shared_ptr<Font> m_titleFont;
    std::shared_ptr<Font> m_itemFont;
};


#endif //VULKAN_SPRITES_MAINMENU_H
