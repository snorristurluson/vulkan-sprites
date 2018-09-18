//
// Created by snorri on 17.9.2018.
//

#include "SecondApp.h"
#include "../../engine/Renderer.h"

#include <string>
#include <iostream>

namespace {
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        SecondApp* app = reinterpret_cast<SecondApp*>(glfwGetWindowUserPointer(window));
        app->HandleKey(window, key, scancode, action, mods);
    }
}

SecondApp::SecondApp() :
    m_window(nullptr),
    m_x(0),
    m_y(0.0f),
    m_dx(0.0f),
    m_vx(0.2f)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void SecondApp::CreateWindow(int width, int height)
{
    m_window = glfwCreateWindow(width, height, "Second", nullptr, nullptr);

    glfwGetFramebufferSize(m_window, &m_width, &m_height);

    m_width *= 2;
    m_height *= 2;

    m_x = m_width / 2;
    m_y = m_height - 16;
    m_dx = 0.0f;

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
    glfwSetKeyCallback(m_window, keyCallback);
}

void SecondApp::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<SecondApp *>(glfwGetWindowUserPointer(window));
    app->m_framebufferResized = true;
}

void SecondApp::Run()
{
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    r.SetClearColor({0.0f, 0.0f, 1.0f, 1.0f});
    while (!glfwWindowShouldClose(m_window)) {
        m_x += m_dx * m_vx;
        if(m_x < 0.0f) {
            m_x = 0.0f;
        }
        if(m_x > m_width) {
            m_x = m_width;
        }
        if(r.StartFrame()) {
            r.SetColor({1.0f, 0.0f, 0.0f, 1.0f});
            r.DrawSprite(64, 32, 64, 32);
            r.SetColor({1.0f, 1.0f, 1.0f, 1.0f});
            r.DrawSprite(m_x - 32, m_y, 64, 16);
            r.EndFrame();
        }

        glfwPollEvents();
    }
    r.WaitUntilDeviceIdle();
}

GLFWwindow *SecondApp::GetWindow()
{
    return m_window;
}

void SecondApp::HandleKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
    switch(key) {
        case GLFW_KEY_Z:
            if(action == GLFW_PRESS) {
                m_dx = -1.0f;
            } else if(action == GLFW_RELEASE) {
                m_dx = 0.0f;
            }
            break;

        case GLFW_KEY_X:
            if(action == GLFW_PRESS) {
                m_dx = 1.0f;
            } else if(action == GLFW_RELEASE) {
                m_dx = 0.0f;
            }
            break;
    }
}
