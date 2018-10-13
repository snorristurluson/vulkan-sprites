//
// Created by snorri on 12.10.2018.
//

#include "BreakOutApp.h"
#include <Logger.h>
#include <Renderer.h>
#include <Texture.h>

static auto logger = GetLogger("main");

namespace {
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        BreakOutApp* app = reinterpret_cast<BreakOutApp*>(glfwGetWindowUserPointer(window));
        app->HandleKey(window, key, scancode, action, mods);
    }
}

BreakOutApp::BreakOutApp() : m_splashScreen(this), m_mainMenu(this)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_states.resize(NUM_STATES);
    m_states[SPLASH_SCREEN] = &m_splashScreen;
    m_states[MAIN_MENU] = &m_mainMenu;

}

void BreakOutApp::CreateWindow(int width, int height)
{
    m_window = glfwCreateWindow(width, height, "Vulkan BreakOut", nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, keyCallback);
}

void BreakOutApp::Run()
{
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    r.SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});

    auto ta = r.CreateTextureAtlas(1024, 1024);
    m_fontManager = std::make_unique<FontManager>(ta);

    for(auto state: m_states) {
        state->Init(r);
    }

    m_currentState = &m_splashScreen;
    m_nextState = m_currentState;
    m_currentState->Enter(r);

    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        if(r.StartFrame()) {
            while(m_nextState != m_currentState) {
                m_currentState->Exit(r);
                m_nextState->Enter(r);
                m_currentState = m_nextState;
            }
            m_currentState->Render(r);
            r.EndFrame();
        }
    }
    r.WaitUntilDeviceIdle();
}

void BreakOutApp::ChangeState(int newState) {
    m_nextState = m_states[newState];
}

void BreakOutApp::HandleKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
    m_currentState->HandleKey(key, scancode, action, mods);
}

std::shared_ptr<Font> BreakOutApp::GetFont(const std::string &name, int pt) {
    return m_fontManager->GetFont(name, pt);
}
