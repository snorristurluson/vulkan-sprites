//
// Created by Snorri Sturluson on 20/08/2018.
//

#include "BlendModesApp.h"
#include "Renderer.h"
#include "Texture.h"

#include <string>
#include <cstdio>

BlendModesApp::BlendModesApp() : m_window(nullptr)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void BlendModesApp::CreateWindow(int width, int height)
{
    m_window = glfwCreateWindow(width, height, "Vulkan Sprites", nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
}

void BlendModesApp::Run()
{
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    r.SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});

    auto tex = r.CreateTexture("resources/2.png");
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        if(r.StartFrame()) {
            r.SetTexture(tex);
            r.SetColor({0.5f, 0.7f, 0.3f, 0.7f});
            r.DrawSprite(32, 32, 256, 256);
            r.SetColor({0.7f, 0.5f, 0.3f, 0.7f});
            r.DrawSprite(64, 32, 256, 256);
            r.SetColor({0.5f, 0.3f, 0.7f, 0.7f});
            r.DrawSprite(64, 64, 256, 256);
            r.SetColor({0.7f, 0.3f, 0.5f, 0.7f});
            r.DrawSprite(32, 64, 256, 256);
            r.EndFrame();
        }
    }
    r.WaitUntilDeviceIdle();
}

GLFWwindow *BlendModesApp::GetWindow()
{
    return m_window;
}
