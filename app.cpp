//
// Created by Snorri Sturluson on 20/08/2018.
//

#include "app.h"
#include "Renderer.h"

#include <string>

App::App() : m_window(nullptr)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void App::CreateWindow(int width, int height)
{
    m_window = glfwCreateWindow(width, height, "Vulkan Sprites", nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}

void App::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<App *>(glfwGetWindowUserPointer(window));
    app->m_framebufferResized = true;
}

void App::Run()
{
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    r.SetClearColor({0.0f, 0.0f, 1.0f, 1.0f});
    auto t = r.CreateTexture("resources/texture.jpg");
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        if(r.StartFrame()) {
            r.DrawSprite(0, 0, 100, 100);
            r.DrawSprite(200, 200, 200, 200);
            r.SetTexture(t);
            r.DrawSprite(300, 300, 300, 300);
            r.EndFrame();
        }
    }
    r.WaitUntilDeviceIdle();
}

GLFWwindow *App::GetWindow()
{
    return m_window;
}
