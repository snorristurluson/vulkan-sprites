//
// Created by snorri on 12.10.2018.
//

#include "BreakOutApp.h"
#include <Logger.h>
#include <Renderer.h>
#include <Texture.h>
#include <telemetry.h>

static auto logger = GetLogger("main");

namespace {
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        BreakOutApp* app = reinterpret_cast<BreakOutApp*>(glfwGetWindowUserPointer(window));
        app->HandleKey(window, key, scancode, action, mods);
    }

    void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
        BreakOutApp* app = reinterpret_cast<BreakOutApp*>(glfwGetWindowUserPointer(window));
        app->HandleCursorPosition(window, xpos, ypos);
    }
}

BreakOutApp::BreakOutApp() : m_splashScreen(this), m_mainMenu(this), m_gameplay(this), m_gameOver(this)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_states.resize(NUM_STATES);
    m_states[SPLASH_SCREEN] = &m_splashScreen;
    m_states[MAIN_MENU] = &m_mainMenu;
    m_states[GAMEPLAY] = &m_gameplay;
    m_states[GAME_OVER] = &m_gameOver;
}

void BreakOutApp::CreateWindow(int width, int height)
{
    m_window = glfwCreateWindow(width, height, "Vulkan BreakOut", nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetCursorPosCallback(m_window, cursorPositionCallback);
}

void BreakOutApp::Run()
{
    ConnectTelemetry();

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

    while (!glfwWindowShouldClose(m_window) && m_currentState) {
        glfwPollEvents();
        if(r.StartFrame()) {
            while(m_nextState != m_currentState) {
                m_currentState->Exit(r);
                if(m_nextState) {
                    m_nextState->Enter(r);
                }
                m_currentState = m_nextState;
            }
            if(m_currentState) {
                m_currentState->Render(r);
            }
            r.EndFrame();
        }
        tmTick(0);
    }
    r.WaitUntilDeviceIdle();
}

void BreakOutApp::ChangeState(int newState) {
    if(newState == QUIT) {
        m_nextState = nullptr;
    } else {
        m_nextState = m_states[newState];
    }
}

void BreakOutApp::HandleKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
    m_currentState->HandleKey(key, scancode, action, mods);
}

void BreakOutApp::HandleCursorPosition(GLFWwindow *window, double xpos, double ypos) {
    m_currentState->HandleCursorPosition(xpos, ypos);
}

std::shared_ptr<Font> BreakOutApp::GetFont(int pt) {
    return m_fontManager->GetFont("resources/montserrat/Montserrat-Bold.ttf", pt);
}

GLFWwindow *BreakOutApp::GetWindow() {
    return m_window;
}
