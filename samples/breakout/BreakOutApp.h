//
// Created by snorri on 12.10.2018.
//

#ifndef VULKAN_SPRITES_BREAKOUTAPP_H
#define VULKAN_SPRITES_BREAKOUTAPP_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "SplashScreen.h"
#include "MainMenu.h"
#include "Gameplay.h"
#include "GameOver.h"

#include <vector>
#include <FontManager.h>

class BreakOutApp : public App {
public:
    BreakOutApp();

    void CreateWindow(int width, int height);
    void Run();

    void ChangeState(int newState) override;

    std::shared_ptr<Font> GetFont(const std::string &name, int pt) override;

    void HandleKey(GLFWwindow *window, int key, int scancode, int action, int mods);

protected:
    GLFWwindow *m_window;
    std::unique_ptr<FontManager> m_fontManager;

    SplashScreen m_splashScreen;
    MainMenu m_mainMenu;
    Gameplay m_gameplay;
    GameOver m_gameOver;

    std::vector<AppState*> m_states;
    AppState* m_currentState;
    AppState* m_nextState;
};


#endif //VULKAN_SPRITES_BREAKOUTAPP_H
