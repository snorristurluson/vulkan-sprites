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

SecondApp::SecondApp() : m_window(nullptr)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void SecondApp::CreateWindow(int width, int height)
{
    m_window = glfwCreateWindow(width, height, "Second", nullptr, nullptr);
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
        glfwPollEvents();
        if(r.StartFrame()) {
            r.EndFrame();
        }
    }
    r.WaitUntilDeviceIdle();
}

GLFWwindow *SecondApp::GetWindow()
{
    return m_window;
}

void SecondApp::HandleKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
    std::cout << "HandleKey" << key << scancode << action << mods << std::endl;
}
