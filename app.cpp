//
// Created by Snorri Sturluson on 20/08/2018.
//

#include "app.h"
#include "Renderer.h"

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
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        r.StartFrame();
        r.EndFrame();
    }
    r.WaitUntilDeviceIdle();
}

GLFWwindow *App::GetWindow()
{
    return m_window;
}
