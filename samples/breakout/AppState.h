//
// Created by snorri on 12.10.2018.
//

#ifndef VULKAN_SPRITES_APPSTATE_H
#define VULKAN_SPRITES_APPSTATE_H

#include <string>
#include <memory>

class Renderer;
class Font;
class GLFWwindow;

class App {
public:
    virtual void ChangeState(int newState) = 0;
    virtual std::shared_ptr<Font> GetFont(int pt) = 0;
    virtual GLFWwindow *GetWindow() = 0;
};

class AppState {
public:
    AppState(App* a) : m_app(a) {}

    virtual void Init(Renderer& r) = 0;
    virtual void Enter(Renderer& r) = 0;
    virtual void Render(Renderer& r) = 0;
    virtual void Exit(Renderer& r) = 0;
    virtual void HandleKey(int key, int scancode, int action, int mods) = 0;
    virtual void HandleCursorPosition(double xpos, double ypos) = 0;

protected:
    App* m_app;
};

typedef enum {
    SPLASH_SCREEN,
    MAIN_MENU,
    GAMEPLAY,
    GAME_OVER,

    NUM_STATES,

    QUIT
} State;

#endif //VULKAN_SPRITES_APPSTATE_H
