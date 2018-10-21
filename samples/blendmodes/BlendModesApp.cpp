//
// Created by Snorri Sturluson on 20/08/2018.
//

#include "BlendModesApp.h"
#include "Renderer.h"
#include "Texture.h"

#include <string>
#include <cstdio>
#include <glm/vec2.hpp>

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

    auto tex = r.CreateTexture("resources/circle.png");
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        if(r.StartFrame()) {
            r.SetTexture(tex);

            r.SetBlendMode(BM_NONE);
            DrawOverlappingSprites(r, {0.0f, 0.0f});

            r.SetBlendMode(BM_BLEND);
            DrawOverlappingSprites(r, {300.0f, 0.0f});

            r.SetBlendMode(BM_ADD);
            DrawOverlappingSprites(r, {0.0f, 300.0f});

            r.SetBlendMode(BM_ADDX2);
            DrawOverlappingSprites(r, {300.0f, 300.0f});

            r.EndFrame();

            if(r.GetNumDrawCommands() > 1) {
                throw std::runtime_error("Too many draw commands");
            }
        }
    }
    r.WaitUntilDeviceIdle();
}

void BlendModesApp::DrawOverlappingSprites(Renderer &r, glm::vec2 offset) const {
    r.SetColor({1.0f, 0.0f, 0.0f, 0.5f});
    r.DrawSprite(32 + offset.x, 32 + offset.y, 256, 256);
    r.SetColor({0.0f, 1.0f, 0.0f, 0.5f});
    r.DrawSprite(64 + offset.x, 32 + offset.y, 256, 256);
    r.SetColor({0.0f, 0.0f, 1.0f, 0.5f});
    r.DrawSprite(64 + offset.x, 64 + offset.y, 256, 256);
    r.SetColor({0.5f, 0.5f, 0.5f, 0.5f});
    r.DrawSprite(32 + offset.x, 64 + offset.y, 256, 256);
}

GLFWwindow *BlendModesApp::GetWindow()
{
    return m_window;
}
