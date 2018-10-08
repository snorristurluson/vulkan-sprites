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
    auto t1 = r.CreateTexture("resources/1.png");
    auto t2 = r.CreateTexture("resources/2.png");
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        if(r.StartFrame()) {
            for(int i = 0; i < 33; ++i) {
                for(int j = 0; j < 66; ++j) {
                    r.SetTexture(t1);
                    r.DrawSprite(i*50, j*50, 40, 40);
                    r.SetTexture(t2);
                    r.DrawSprite(i*45, j*45, 80, 80);
                }
            }
            r.EndFrame();
        }
    }
    r.WaitUntilDeviceIdle();
}

GLFWwindow *FirstApp::GetWindow()
{
    return m_window;
}
