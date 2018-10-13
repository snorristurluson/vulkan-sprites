//
// Created by Snorri Sturluson on 20/08/2018.
//

#include "FirstApp.h"
#include "../../engine/Renderer.h"
#include "../../engine/Texture.h"

#include <string>
#include <cstdio>

FirstApp::FirstApp() : m_window(nullptr)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void FirstApp::CreateWindow(int width, int height)
{
    m_window = glfwCreateWindow(width, height, "Vulkan Sprites", nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}

void FirstApp::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<FirstApp *>(glfwGetWindowUserPointer(window));
    app->m_framebufferResized = true;
}

void FirstApp::Run()
{
    Renderer r;
    r.Initialize(m_window, Renderer::ENABLE_VALIDATION);
    r.SetClearColor({0.0f, 0.0f, 1.0f, 1.0f});
    auto t = r.CreateTexture("resources/texture.jpg");
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        if(r.StartFrame()) {
            r.SetTexture(t);
            r.DrawSprite(32, 32, t->GetWidth(), t->GetHeight());
            r.EndFrame();
        }
    }
    r.WaitUntilDeviceIdle();
}

GLFWwindow *FirstApp::GetWindow()
{
    return m_window;
}
