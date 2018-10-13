//
// Created by snorri on 12.10.2018.
//

#ifndef VULKAN_SPRITES_SPLASHSCREEN_H
#define VULKAN_SPRITES_SPLASHSCREEN_H

#include "AppState.h"
#include "Font.h"

class SplashScreen : public AppState {
public:
    SplashScreen(App *a);

    void Init(Renderer &r) override;
    void Enter(Renderer &r) override;
    void Render(Renderer &r) override;
    void Exit(Renderer &r) override;
    void HandleKey(int key, int scancode, int action, int mods) override;

protected:
    int m_frameCounter;

    std::shared_ptr<Font> m_font;
};

#endif //VULKAN_SPRITES_SPLASHSCREEN_H
